#pragma once
#include <string>
#include <vector>
#include "location_conf.hpp"

class server_conf{
private:
	bool is_faile;
	std::string _name;
	std::vector<location_conf>locations;
	server_conf();
public:
	server_conf(std::string);
	server_conf(const server_conf &);
	const server_conf &operator=(const server_conf &);
	bool faile() const;
	~server_conf();
};
