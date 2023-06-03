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
	void _to_req();
	virtual ssize_t write(const std::string &);
	virtual ssize_t read(std::string &);
	inline void Bad_Request();
	http_handler();//not use
	http_handler(const http_handler&);//not use
	const http_handler &operator=(const http_handler&);//not use
public:
	http_handler(handler*, int, const port_conf &, struct sockaddr_in);
	~http_handler();
};

#define KEY_BODY ":body"
#define KEY_METHOD ":method"
#define KEY_TARGET ":target"
#define KEY_VERSION ":version"

#define RES_400 "HTTP/1.1 400 Bad Request\r\n\r\n"

