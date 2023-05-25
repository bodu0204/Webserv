#pragma once
#include "config/server_conf.hpp"
#include "handler.hpp"

class cgir_handler: public handler
{
private:
	void _action(short);
	cgir_handler();//not use
	cgir_handler(const cgir_handler&);//not use
	const cgir_handler &operator=(const cgir_handler&);//not use
public:
	cgir_handler(handler *,int);
	~cgir_handler();
};
