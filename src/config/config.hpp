#pragma once
#include <string>
#include <vector>
#include "port_conf.hpp"

class config{
private:
	bool is_faile;
	std::vector<port_conf> ports;
	config();
public:
	config(std::string);
	config(const config &);
	const config &operator=(const config &);
	std::vector<port_conf> port_confs() const;
	bool faile() const;
	~config();
};
