#pragma once
#include "config/server_conf.hpp"
#include "handler.hpp"

class cgiw_handler: public handler
{
private:
	void _action(short);
	cgiw_handler();//not use
	cgiw_handler(const cgiw_handler&);//not use
	const cgiw_handler &operator=(const cgiw_handler&);//not use
protected:
	void callback(std::string);
public:
	cgiw_handler(int);
	~cgiw_handler();
};
