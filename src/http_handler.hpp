#pragma once
#include <unistd.h>
#include "handler.hpp"
#include "cgir_handler.hpp"
#include "cgiw_handler.hpp"
#include "config/port_conf.hpp"
#include <map>

class http_handler: public handler
{
private:
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
	void _to_res(std::string);
	http_handler();//not use
	http_handler(const http_handler&);//not use
	const http_handler &operator=(const http_handler&);//not use
public:
	http_handler(handler*, int, const port_conf &);
	~http_handler();
};
