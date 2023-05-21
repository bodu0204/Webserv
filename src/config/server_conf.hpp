#pragma once
#include <string>
#include <vector>
#include <map>
#include "location_conf.hpp"

class server_conf{
private:
	bool is_faile;
	std::vector<const location_conf>locations;
	server_conf();
public:
	server_conf(std::string);
	server_conf(const server_conf &);
	const server_conf &operator=(const server_conf &);
	void set_location(std::string);
	std::string name() const;
	const location_conf &location(const std::map<std::string, std::string>&) const;
	bool faile() const;
	~server_conf();

	static const server_conf error;
};
