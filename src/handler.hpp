#pragma once
#include <set>
#include <time.h>

class handler
{
private:
	const unsigned _life;
	time_t _limit;
	handler();
	const handler &operator=(const handler&);
protected:
	std::set<handler *> child;
	std::set<handler *> add;
	std::set<handler *> del;
	handler(int, unsigned, long life = LONG_MAX);
	handler(const handler&);
	void set_time();
public:
	const int descriptor;
	const short events;
	std::set<handler *> get_add_handler();
	std::set<handler *> get_del_handler();
	std::set<handler *> all_child() const;
	time_t limit() const;
	virtual void action(short) = 0;//readが先
	virtual ~handler() = 0;
};
