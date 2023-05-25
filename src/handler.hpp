#pragma once
#include <set>
#include <time.h>
#include <string>

class handler
{
private:
	const unsigned _life;
	time_t _limit;
	std::set<handler *> _child;
	std::set<handler *> _add;
	std::set<handler *> _del;
	virtual void _action(short) = 0;//readが先
	handler();//not use
	handler(const handler&);//not use
	const handler &operator=(const handler&);//not use
protected:
	handler *parent;
	handler(handler *,int, unsigned, long life = LONG_MAX);
	void set_add(handler *);
	void set_add(std::set<handler *>);
	void set_del(handler *);
	void set_del(std::set<handler *>);
	virtual void callback_end(handler *);
public:
	const int descriptor;
	const short events;
	std::set<handler *> get_add_handler();
	std::set<handler *> get_del_handler();
	std::set<handler *> all_child() const;
	time_t limit() const;
	virtual void callback(std::string);
	void action(short);
	virtual ~handler(); //必ず呼ぶ
};
