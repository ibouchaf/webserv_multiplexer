#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <exception>
class Webserv
{
private:
    std::string config_path;
    std::string* config;
    int config_size;

public:
    // Construction and Destruction
    Webserv();
    Webserv(std::string conf);
    Webserv(const Webserv &other);
    Webserv &operator=(const Webserv &other);
    ~Webserv();

    // Config parsing
    int parse_config(const std::string& conf);
	std::string* get_config() const;
	int get_config_size() const;
};
