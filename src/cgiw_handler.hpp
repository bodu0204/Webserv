#pragma once
#include "config/server_conf.hpp"
#include "handler.hpp"
#include <string>

class cgiw_handler: public handler
{
private:
	std::string buf;

	void _action(short);
	cgiw_handler();//not use
	cgiw_handler(const cgiw_handler&);//not use
	const cgiw_handler &operator=(const cgiw_handler&);//not use
public:
	void set_write(std::string);
	cgiw_handler(handler *,int);
	~cgiw_handler();
};
