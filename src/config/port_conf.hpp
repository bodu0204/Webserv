#pragma once
#include <string>
#include <map>
#include "server_conf.hpp"

enum protocol{
	tcp_echo,
	http,
	https,
};

class port_conf{
private:
	bool is_faile;
	int port;
	protocol proto;
	std::map<std::string,server_conf>servers;
public:
	port_conf();
	port_conf(const port_conf &);
	const port_conf &operator=(const port_conf &);
	void set_protocol(protocol);
	void set_port(int);
	void set_other(int);
	protocol protocol() const;
	bool faile() const;
	~port_conf();
};
