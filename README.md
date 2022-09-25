# serving local file (json) with a http-server:
http-server -p 9000 --cors -c-1
## serves files from directory where executed, in port 9000, with cors to allow script access and no cache


# requirement:
npm i -g http-server

# config arduino para develop 
http://localhost:9000/result.json