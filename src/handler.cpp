#include "handler.hpp"


handler::handler(handler *_parent, int _descriptor, unsigned _events ,long life):\
_life(life),_limit(life==LONG_MAX ? life : time(NULL) + life),parent(_parent),descriptor(_descriptor),events(_events){
	if (_parent)
		parent->_child.insert(this);
}

void handler::set_add(handler *target){
	this->_add.insert(target);
}

void handler::set_add(std::set<handler *> target){
	this->_add.insert(target.begin(), target.end());
}

void handler::set_del(handler *target){
	this->_del.insert(target);
}

void handler::set_del(std::set<handler *> target){
	this->_del.insert(target.begin(), target.end());
}

std::set<handler *> handler::get_add_handler(){
	std::set<handler *> ret;
	ret.swap(this->_add);
	return (ret);
}

std::set<handler *> handler::get_del_handler(){
	std::set<handler *> ret;
	ret.swap(this->_del);
	return (ret);
}

void handler::action(short event){
	if (event & ~this->events){
		this->set_del(this->all_child());
		this->set_del(this);
		return ;
	}
	this->_limit = this->_life==LONG_MAX ? this->_life : time(NULL) + this->_life;
	this->_action(event);
	return ;
}

std::set<handler *> handler::all_child() const{
	std::set<handler *> ret;
	for (std::set<handler *>::const_iterator i = this->_child.begin(); i != this->_child.end(); i++)
	{
		std::set<handler *> cv = (*i)->all_child();
		ret.insert(cv.begin(), cv.end());
	}
	ret.insert(this->_child.begin(), this->_child.end());
	return (ret);
}

time_t handler::limit()const{return(this->_limit);}

void handler::callback_end(handler *child){
	this->_child.erase(child);
	return ;
}

handler::~handler(){
	if (parent)
		this->parent->callback_end(this);
	for (std::set<handler *>::iterator i = this->_child.begin(); i != this->_child.end(); i++){
		(*i)->parent = NULL;
	}
	return ;
}

void handler::callback(std::string){}

handler::handler():_life(0),_limit(time(NULL)),parent(0),descriptor(0),events(0){}//not call
const handler &handler::operator=(const handler&){return *this;}//not call
handler::handler(const handler&):_life(0),_limit(time(NULL)),parent(0),descriptor(0),events(0){}//not call
