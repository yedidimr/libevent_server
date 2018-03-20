import sys
import requests
import math
if __name__ == "__main__":
	assert len(sys.argv) == 2
	k = int(sys.argv[1])
	r = requests.post("http://localhost:5556", data={'k': k}) # send post requesr

	print(r.status_code, r.reason)
	print(r.text[:300] + '...')

	print "------------for testing-----------"
	print "pi is", math.pi
	pi_str = str(math.pi)
	if k < len(pi_str)-1:
		print "sep  ", pi_str[:k+1], pi_str[k+1], pi_str[k+2:]  
