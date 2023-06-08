#!/bin/bash
export GATEWAY_INTERFACE=CGI/1.1
export PATH_INFO=/_php/test.php
export PATH_TRANSLATED=/Users/blyu/Desktop/Webserv/src$PATH_INFO
export SCRIPT_NAME=/Users/blyu/Desktop/Webserv/src/_php/test.php
export REMOTE_ADDR=127.0.0.1
export REMOTE_IDENT='Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/113.0.0.0 Safari/537.36'
export REQUEST_METHOD=GET
export SERVER_NAME=
export SERVER_PORT=80
export SERVER_PROTOCOL=HTTP/1.1
export SERVER_SOFTWARE=Webserv/1.0
export HTTP_ACCEPT=text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8
export HTTP_ACCEPT_ENCODING=gzip, deflate, br
export HTTP_ACCEPT_LANGUAGE=ja
export HTTP_CACHE_CONTROL=max-age=0
export HTTP_CONNECTION=keep-alive
export HTTP_HOST=localhost
export HTTP_SEC_CH_UA="Brave";v="113", "Chromium";v="113", "Not-A.Brand";v="24"
export HTTP_SEC_CH_UA_MOBILE=?0
export HTTP_SEC_CH_UA_PLATFORM="macOS"
export HTTP_SEC_FETCH_DEST=document
export HTTP_SEC_FETCH_MODE=navigate
export HTTP_SEC_FETCH_SITE=none
export HTTP_SEC_FETCH_USER=?1
export HTTP_SEC_GPC=1
export HTTP_SERVER=
export HTTP_UPGRADE_INSECURE_REQUESTS=1
export REDIRECT_STATUS=
cat | php-cgi