#pragma once
#include <string>
#include <map>
#include "location_conf.hpp"

class server_conf{
private:
	bool is_faile;
	std::map<std::string,location_conf>_locations;
	server_conf();
public:
	server_conf(std::string);
	server_conf(const server_conf &);
	const server_conf &operator=(const server_conf &);
	const location_conf &location(std::string, std::string &pattern = server_conf::buff) const;
	bool faile() const;
	~server_conf();

	static const server_conf error;
	static std::string buff;
};
