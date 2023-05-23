#pragma once
#include "port_conf.hpp"
#include "handler.hpp"

class accept_handker: public handler
{
private:
	const port_conf &conf;
	accept_handker();
public:
	accept_handker(int, const port_conf &);
	accept_handker(const accept_handker&);
	const accept_handker &operator=(const accept_handker&);
	void action(short);
	~accept_handker();
};
