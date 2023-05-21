#pragma once
#include <string>
#include <vector>
#include "port_conf.hpp"

class config{
private:
	bool is_faile;
	std::vector<port_conf> ports;
public:
	config();
	config(std::string);
	config(const config &);
	const config &operator=(const config &);
	const std::vector<port_conf>& port_confs() const;
	bool faile() const;
	~config();
};
