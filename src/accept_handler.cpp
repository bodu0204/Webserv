#include "accept_handler.hpp"
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <poll.h>
#include "echo_handler.hpp"
#include "http_handler.hpp"

#include <unistd.h>

accept_handler::accept_handler(int descriptor, const port_conf &conf):\
handler(NULL, descriptor, POLL_IN), _conf(conf){}

accept_handler::~accept_handler(){}

void accept_handler::_action(short){
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	socklen_t addrlen = sizeof(addr);
	int desc = accept(this->descriptor, (struct sockaddr *)&addr, &addrlen);
	switch (this->_conf.protocol())
	{
	case tcp_echo:
		this->set_add(new echo_handler(this, desc));
		break;
	case http:
		this->set_add(new http_handler(this,desc,this->_conf,addr));
		break;
	case https:
		//this->set_add(/* code */);
		close(desc);//test
		break;
	default :
		close(desc);//test
		break;
	}
}

accept_handler::accept_handler():handler(NULL, -1, 0), _conf(port_conf::error){}//not use
accept_handler::accept_handler(const accept_handler&):handler(NULL, -1, 0), _conf(port_conf::error){}//not use
const accept_handler &accept_handler::operator=(const accept_handler&){return (*this);}//not use
