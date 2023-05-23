#include "handler.hpp"

handler::handler():descriptor(0), events(0),_life(0),_limit(time(NULL)){}//not call

const handler &handler::operator=(const handler& src){return *this;}//not call

handler::handler(int _descriptor, unsigned _events ,unsigned life):\
	descriptor(_descriptor),events(_events),_life(life),_limit(time(NULL) + life){}

handler::handler(const handler& src):descriptor(src.descriptor),events(src.events),_life(src._life),_limit(src._limit){}

handler::~handler(){}

std::vector<handler *> handler::get_add_handler(){
	std::vector<handler *> ret = this->add;
	this->add.clear();
	return (ret);
}

std::vector<handler *> handler::get_del_handler(){
	std::vector<handler *> ret = this->del;
	this->del.clear();
	return (ret);
}

void handler::set_time(){this->_limit = time(NULL) + this->_life; return ;}

std::vector<handler *> handler::all_child() const{
	std::vector<handler *> ret;
	for (std::vector<handler *>::const_iterator i = this->child.begin(); i != this->child.end(); i++)
	{
		std::vector<handler *> cv = (*i)->all_child();
		ret.insert(ret.end(), cv.begin(), cv.end());
	}
	ret.insert(ret.end(), this->child.begin(), this->child.end());
	return (ret);
}

time_t handler::limit(time_t now)const{return(now - this->_limit);}
