#!/bin/bash
export AUTH_TYPE=
export CONTENT_LENGTH=0
export CONTENT_TYPE=
export GATEWAY_INTERFACE=CGI/1.1
export PATH_INFO=/$1
export PATH_TRANSLATED=$PWD$PATH_INFO
export QUERY_STRING=
export REMOTE_ADDR=42.42.42.42
export REMOTE_HOST=chrome
export REMOTE_IDENT=
export REMOTE_USER=
export REQUEST_METHOD=GET
export SCRIPT_NAME=$PWD/$1
export SERVER_NAME=Webserv
export SERVER_PORT=80
export SERVER_PROTOCOL=HTTP/1.1
export SERVER_SOFTWARE=Webserv/1.0
export REDIRECT_STATUS=
php-cgi