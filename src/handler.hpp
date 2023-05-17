#pragma once
#include <vector>

class handler
{
private:
	int desc;
	handler();
protected:
	std::vector<handler &> add;
	std::vector<handler &> del;
	handler(int);
	handler(const handler&);
	const handler &operator=(const handler&);
public:
	std::vector<handler &> get_add_handler();
	std::vector<handler &> get_del_handler();
	int get_descriptor() const;
	virtual void action(int) = 0;
	virtual ~handler() = 0;
};
