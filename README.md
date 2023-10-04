# webserv

## Description
This project is an introduction to the HTTP protocol and the basics of web server.
It's a simple web server that handles multiple clients at the same time.
It's built with C++ (std=c++98) and it's based on the RFC 7230-7235.

## Usage
```
make
./webserv [config_file]
```

## Config file
The config file is a JSON file that contains the configuration of the server.
It's composed of 3 parts:
- server: contains the configuration of the server
- location: contains the configuration of the location
- error_page: contains the configuration of the error pages

### Server
The server part contains the following fields:
- host: the host of the server
- port: the port of the server
- server_name: the name of the server
- client_max_body_size: the maximum size of the body of a request
- error_page: the path of the error page
- root: the root path of the server
- index: the index page of the server
- autoindex: the autoindex of the server (on/off)

### Location
The location part contains the following fields:
- path: the path of the location
- root: the root path of the location
- index: the index page of the location
- autoindex: the autoindex of the location (on/off)
- methods: the methods allowed in the location
- cgi: the path of the cgi script
- cgi_extension: the extension of the cgi script
- upload: the path of the upload directory
- upload_extension: the extension of the upload files

### Error page
The error page part contains the following fields:
- 400: the path of the 400 error page
- 401: the path of the 401 error page
- 403: the path of the 403 error page
- 404: the path of the 404 error page
- 405: the path of the 405 error page
- 413: the path of the 413 error page
- 500: the path of the 500 error page
- 501: the path of the 501 error page

## HTTP
The server handles the following methods:
- GET
- POST
- DELETE

## CGI
The server handles the following cgi scripts:
- php
- python
- shell

The server handles multiple cgi scripts at the same time.

## Multiplexing
The server handles multiple clients at the same time using the select system call.

## Error handling
The server handles the following errors:
- 400: Bad Request
- 401: Unauthorized
- 403: Forbidden
- 404: Not Found
- 405: Method Not Allowed
- 413: Payload Too Large
- 500: Internal Server Error
- 501: Not Implemented

## Autoindex
The server handles the autoindex of the server and the location.

## Upload
The server handles the upload of files in the server and the location.

## Cookies
The server handles the cookies.

## Examples
```
./webserv config.json
```
```
curl -v -X GET http://localhost:8080/
```
```
curl -v -X GET http://localhost:8080/index.html
```
```
curl -v -X GET http://localhost:8080/index.php
```
```
curl -v -X GET http://localhost:8080/index.py
```
```
curl -v -X GET http://localhost:8080/index.sh
```
```
curl -v -X GET http://localhost:8080/index.cgi
```
```
curl -v -X GET http://localhost:8080/index
```
```
curl -v -X GET http://localhost:8080/index.html?param1=value1&param2=value2
```

## Authors
- [**Astro**](https://intra.42.fr/users/aachhoub)
- [**Vyper**](https://intra.42.fr/users/ibouchaf)
- [**Venom**](https://intra.42.fr/users/ael-bako)