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
	const std::vector<const port_conf>& port() const;
	bool faile() const;
	~config();
};
