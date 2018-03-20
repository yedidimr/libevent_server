
import requests
r = requests.post("http://localhost:5556", data={'k': 12223})

print(r.status_code, r.reason)
print(r.text[:300] + '...')
