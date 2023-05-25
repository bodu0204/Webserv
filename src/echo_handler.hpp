#pragma once
#include <unistd.h>
#include "handler.hpp"
#include "cgir_handler.hpp"
#include "cgiw_handler.hpp"

class echo_handler: public handler
{
private:
	std::string _res_buff;
	cgiw_handler *_cgi_w;
	cgir_handler *_cgi_r;
	pid_t _cgi_pid;
	void _action(short);
	void callback_end(handler *);
	void callback(std::string);
	echo_handler();//not use
	echo_handler(const echo_handler&);//not use
	const echo_handler &operator=(const echo_handler&);//not use
public:
	echo_handler(handler*, int);
	~echo_handler();
};
