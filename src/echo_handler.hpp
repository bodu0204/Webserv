#pragma once
#include "config/server_conf.hpp"
#include "handler.hpp"

class echo_handler: public handler
{
private:
	void _action(short);
	echo_handler();//not use
	echo_handler(const echo_handler&);//not use
	const echo_handler &operator=(const echo_handler&);//not use
protected:
	void callback(std::string);
public:
	echo_handler(int);
	~echo_handler();
};
