#pragma once
#include "config/port_conf.hpp"
#include "handler.hpp"

class accept_handler: public handler
{
private:
	const port_conf &_conf;
	void _action(short);
	accept_handler();//not use
	accept_handler(const accept_handler&);//not use
	const accept_handler &operator=(const accept_handler&);//not use
public:
	accept_handler(int, const port_conf &);
	~accept_handler();
};
