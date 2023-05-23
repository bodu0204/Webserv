#pragma once
#include <vector>
#include <time.h>

class handler
{
private:
	const unsigned _life;
	time_t _limit;
	handler();
	virtual const handler &operator=(const handler&);
protected:
	std::vector<handler *> child;
	std::vector<handler *> add;
	std::vector<handler *> del;
	handler(int, unsigned, long life = LONG_MAX);
	handler(const handler&);
	void set_time();
public:
	const int descriptor;
	const short events;
	std::vector<handler *> get_add_handler();
	std::vector<handler *> get_del_handler();
	std::vector<handler *> all_child() const;
	time_t limit(time_t) const;
	virtual void action(short) = 0;//readが先
	virtual ~handler() = 0;
};
