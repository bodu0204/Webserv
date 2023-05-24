#include "handler.hpp"

handler::handler():descriptor(0), events(0),_life(0),_limit(time(NULL)){}//not call

const handler &handler::operator=(const handler& src){return *this;}//not call

handler::handler(int _descriptor, unsigned _events ,long life):\
	descriptor(_descriptor),events(_events),_life(life),_limit(life==LONG_MAX ? life : time(NULL) + life){}

handler::handler(const handler& src):descriptor(src.descriptor),events(src.events),_life(src._life),_limit(src._limit){}

handler::~handler(){}

std::set<handler *> handler::get_add_handler(){
	std::set<handler *> ret = this->add;
	this->add.clear();
	return (ret);
}

std::set<handler *> handler::get_del_handler(){
	std::set<handler *> ret = this->del;
	this->del.clear();
	return (ret);
}

void handler::set_time(){
	this->_limit = this->_life==LONG_MAX ? this->_life : time(NULL) + this->_life;
	return ;
}

std::set<handler *> handler::all_child() const{
	std::set<handler *> ret;
	for (std::set<handler *>::const_iterator i = this->child.begin(); i != this->child.end(); i++)
	{
		std::set<handler *> cv = (*i)->all_child();
		ret.insert(cv.begin(), cv.end());
	}
	ret.insert(this->child.begin(), this->child.end());
	return (ret);
}

time_t handler::limit()const{return(this->_limit);}
