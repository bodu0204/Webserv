#pragma once
#include <vector>

#define HANDLER_READ
#define HANDLER_WRITE

class handler
{
private:
	int desc;
	unsigned _rw;
	handler();
protected:
	std::vector<handler *> add;
	std::vector<handler *> del;
	handler(int, unsigned);
	handler(const handler&);
	const handler &operator=(const handler&);
public:
	const std::vector<handler *> get_add_handler();
	const std::vector<handler *> get_del_handler();
	int descriptor() const;
	unsigned descriptor_status() const;
	virtual void action() = 0;
	virtual ~handler() = 0;
};
