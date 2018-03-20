#include <memory>
#include <cstdint>
#include <iostream>
#include <evhttp.h>
#include <string.h>

#include <pthread.h> 

#include "calc_pi.hpp"     


// # request handler
void OnReq(evhttp_request *req, void *) {
    
    struct evhttp_uri *decoded = NULL;
 	  struct evbuffer *out_buf = NULL;
    
    const char *uri = evhttp_request_get_uri (req);

	/* We only handle POST requests. */
	if (evhttp_request_get_command (req) != EVHTTP_REQ_POST)
	{
	    evhttp_send_error (req, HTTP_BADREQUEST, "It's not a post\n");
	  	return;
	}

  	// check that the url is OK
  	decoded = evhttp_uri_parse (uri);
    if (! decoded)
    { 
        evhttp_send_error (req, HTTP_BADREQUEST, "It's not a good URI. Sending BADREQUEST\n");
        return;
    }

  
    // Parse the query string	
    struct evkeyvalq params;
    memset (&params, 0, sizeof (params));

    struct evbuffer *in_buf = evhttp_request_get_input_buffer(req);
    evbuffer_add (in_buf, "", 1);    /* NUL-terminate the buffer */

    char *data = (char *) evbuffer_pullup (in_buf, -1);

	if (0 != evhttp_parse_query_str (data, &params))
    { 
    	printf ("Malformed payload. Sending BADREQUEST\n");
      	evhttp_send_error (req, HTTP_BADREQUEST, "Malformed payload. Sending BADREQUEST");
      	return;
    }

	// extract the parameters 
	const char *value = evhttp_find_header(&params, "k");
	int k = atoi(value);  // todo , check for failure



  int digit =  CalculatePi::get_nth_digit(k);
	char  response[256]; // todo move 256 to const
	evutil_snprintf (response, sizeof (response),
	               "Hi! PI's %dth digit is %d!.\n", k, digit);
	evhttp_clear_headers (&params);   /* to free memory held by kv */


     // send a line to buffer
  	out_buf = evbuffer_new ();

	evbuffer_add (out_buf, response, strlen (response));

    // evbuffer_add_printf(OutBuf, "<html><body><center><h1>Hello World  %s!</h1></center></body></html>", data);
    // evbuffer_add_printf(out_buf, "<html><body><center><h1>Hello World |%s|, |%s|, size of input:%d!</h1></center></body></html>", value, data, sizeof(data));
    // sen buffer as reply
    evhttp_send_reply(req, HTTP_OK, "", out_buf);
    // free(data); // scould should handle exception properl
}

int main()
{
  char const SrvAddress[] = "127.0.0.1";
  std::uint16_t SrvPort = 5556;
  
  // Initialize a global object of the library using event_init 
  if (!event_init())
  {
    std::cerr << "Failed to init libevent." << std::endl;
    return -1;
  }

  // Create http server using evhttp_start
  std::unique_ptr<evhttp, decltype(&evhttp_free)> Server(evhttp_start(SrvAddress, SrvPort), &evhttp_free);
  if (!Server)
  {
    std::cerr << "Failed to init http server." << std::endl;
    return -1;
  }
 

  // setup a callback function to handle incoming requests 
  evhttp_set_gencb(Server.get(), OnReq, nullptr);
 

  // start the loop of event processing (requests) using event_dispatch
  if (event_dispatch() == -1)
  {
    std::cerr << "Failed to run messahe loop." << std::endl;
    return -1;
  }
  return 0;
}