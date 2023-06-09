#pragma once
#include <unistd.h>
#include "handler.hpp"
#include "cgir_handler.hpp"
#include "cgiw_handler.hpp"
#include "config/port_conf.hpp"
#include <map>
#include <netinet/in.h>

class http_handler: public handler
{
private:
	struct sockaddr_in _info;
	const port_conf &_conf;
	std::string _req_buff;
	ssize_t _body;
	std::map<std::string,std::string> _req;
	pid_t _cgi_pid;
	cgiw_handler *_cgi_w;
	cgir_handler *_cgi_r;
	std::map<std::string,std::string> _res;
	std::string _res_buff;
	void _action(short);
	void callback_end(handler *);
	void callback(std::string);
	void exec();
	void exec_CGI(const location_conf &);
	void exec_std(const location_conf &);
	void _to_req();
	virtual ssize_t _write(const std::string &);
	virtual ssize_t _read(std::string &);

	inline void Found(std::string);

	inline void Bad_Request();
	inline void Not_Found();
	inline void Method_Not_Allowed();
	inline void Not_Acceptable();
	inline void Length_Required();
	inline void I_m_a_teapot();

	inline void Internal_Server_Error();
	inline void Not_Implemented();//不要？

	http_handler();//not use
	http_handler(const http_handler&);//not use
	const http_handler &operator=(const http_handler&);//not use
public:
	http_handler(handler*, int, const port_conf &, struct sockaddr_in);
	virtual ~http_handler();
static const std::map<std::string, std::string>mime;
};

#define MAX_REQEST_SIZE 65536

#define KEY_BODY ":body"
#define KEY_METHOD ":method"
#define KEY_TARGET ":target"
#define KEY_VERSION ":version"
#define KEY_CGIBUFF ":CGIbuff"
#define KEY_CGIHEAD ":CGIhead"
#define KEY_CGIBODY ":CGIbody"
#define KEY_STATUS ":status"

#define STATUS_200 "HTTP/1.1 200 OK\r\n"

#define STATUS_302 "HTTP/1.1 302 Found\r\n"

#define STATUS_400 "HTTP/1.1 400 Bad Request\r\n"
#define STATUS_404 "HTTP/1.1 404 Not Found\r\n"
#define STATUS_405 "HTTP/1.1 405 Method Not Allowed\r\n"
#define STATUS_406 "HTTP/1.1 406 Not Acceptable\r\n"
#define STATUS_411 "HTTP/1.1 411 Length Required\r\n"
#define STATUS_418 "HTTP/1.1 418 I'm a teapot\r\n"

#define STATUS_500 "HTTP/1.1 500 Internal Server Error\r\n"
#define STATUS_501 "HTTP/1.1 501 Not Implemented\r\n"

#define CRLF "\r\n"
#define F_NAME_ABLE "!#$%&'*+-.^_`|~"
#define SERVER_TEAPOT "Server: teapot\r\n"
#define CONTENT_LENGTH_ZERO "Content-Length: 0\r\n"
#define CONNECTION_KEEP_ALIVE "Connection: keep-alive\r\n"



#define BODY_404 "<!DOCTYPE html>\
<html>\
<head>\
	<title>404 - Not Found</title>\
	<style>\
		body {\
			background-color: #E0F2F1;\
			font-family: Arial, sans-serif;\
			text-align: center;\
			padding-top: 100px;\
		}\
\
		h1 {\
			color: #00B0FF;\
			font-size: 48px;\
		}\
\
		p {\
			color: #424242;\
			font-size: 24px;\
		}\
\
		img {\
			width: 300px;\
			height: auto;\
			margin-bottom: 30px;\
		}\
	</style>\
</head>\
<body>\
	<h1>Oops!</h1>\
	<p>404 - Page Not Found</p>\
</body>\
</html>"

#define BODY_405 "<!DOCTYPE html>\
<html>\
<head>\
	<title>404 - Not Found</title>\
	<style>\
		body {\
			background-color: #E0F2F1;\
			font-family: Arial, sans-serif;\
			text-align: center;\
			padding-top: 100px;\
		}\
\
		h1 {\
			color: #00B0FF;\
			font-size: 48px;\
		}\
\
		p {\
			color: #424242;\
			font-size: 24px;\
		}\
\
		img {\
			width: 300px;\
			height: auto;\
			margin-bottom: 30px;\
		}\
	</style>\
</head>\
<body>\
	<h1>Oops!</h1>\
	<p>405 - Method Not Allowed</p>\
</body>\
</html>"

#define BODY_406 "<!DOCTYPE html>\
<html>\
<head>\
	<title>404 - Not Found</title>\
	<style>\
		body {\
			background-color: #E0F2F1;\
			font-family: Arial, sans-serif;\
			text-align: center;\
			padding-top: 100px;\
		}\
\
		h1 {\
			color: #00B0FF;\
			font-size: 48px;\
		}\
\
		p {\
			color: #424242;\
			font-size: 24px;\
		}\
\
		img {\
			width: 300px;\
			height: auto;\
			margin-bottom: 30px;\
		}\
	</style>\
</head>\
<body>\
	<h1>Oops!</h1>\
	<p>406 - Not Acceptable</p>\
</body>\
</html>"

#define BODY_411 "<!DOCTYPE html>\
<html>\
<head>\
	<title>404 - Not Found</title>\
	<style>\
		body {\
			background-color: #E0F2F1;\
			font-family: Arial, sans-serif;\
			text-align: center;\
			padding-top: 100px;\
		}\
\
		h1 {\
			color: #00B0FF;\
			font-size: 48px;\
		}\
\
		p {\
			color: #424242;\
			font-size: 24px;\
		}\
\
		img {\
			width: 300px;\
			height: auto;\
			margin-bottom: 30px;\
		}\
	</style>\
</head>\
<body>\
	<h1>Oops!</h1>\
	<p>411 - Length Required</p>\
</body>\
</html>"

#define BODY_418 "<!DOCTYPE html>\
<html>\
<head>\
	<title>404 - Not Found</title>\
	<style>\
		body {\
			background-color: #E0F2F1;\
			font-family: Arial, sans-serif;\
			text-align: center;\
			padding-top: 100px;\
		}\
\
		h1 {\
			color: #00B0FF;\
			font-size: 48px;\
		}\
\
		p {\
			color: #424242;\
			font-size: 24px;\
		}\
\
		img {\
			width: 300px;\
			height: auto;\
			margin-bottom: 30px;\
		}\
	</style>\
</head>\
<body>\
	<h1>Oops!</h1>\
	<p>418 - I'm a teapot</p>\
</body>\
</html>"

