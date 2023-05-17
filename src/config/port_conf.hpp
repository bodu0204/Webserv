#pragma once
#include <string>
#include <map>
#include "server_conf.hpp"

enum protocol{
	tcp_echo,
	http,
	https,
};

class port_conf
{
private:
	bool is_faile;
	int port;
	protocol proto;
	std::map<std::string,server_conf>servers;
	port_conf();
public:
	port_conf(std::string);
	port_conf(const port_conf &);
	const port_conf &operator=(const port_conf &);
	bool faile();
	~port_conf();
};
