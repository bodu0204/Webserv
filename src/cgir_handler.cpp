#include <string>
#include <poll.h>
#include <unistd.h>
#include "cgir_handler.hpp"
#define BUFFERSIZE 1024

void cgir_handler::_action(short event){
    if (event & ~this->events){
		this->set_del(this->all_child());
		this->set_del(this);
		return ;
	}
    //if (/* event & POLL_OUT &&  */this->buf.length())
	char buff[BUFFERSIZE];
	ssize_t r = read(this->descriptor, buff, BUFFERSIZE - 1);
	if (r < 0){
		this->set_del(this->all_child());
		this->set_del(this);
		return ;
	}
	buff[r] = '\0';
	if (this->parent)
		this->parent->callback(buff);
    return ;
}

cgir_handler::cgir_handler(handler *parent,int descriptor):handler(parent, descriptor, POLL_IN|POLL_OUT, 40){}

cgir_handler::~cgir_handler(){handler::~handler();}

cgir_handler::cgir_handler():handler(NULL, 0, 0){}//not use
cgir_handler::cgir_handler(const cgir_handler&):handler(NULL, 0, 0){}//not use
const cgir_handler &cgir_handler::operator=(const cgir_handler&){return (*this);}//not use
