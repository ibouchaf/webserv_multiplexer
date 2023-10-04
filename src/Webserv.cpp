#include "../inc/Webserv.hpp"

Webserv::Webserv()
{
    config_path = "./conf/webserv.conf";
    if (parse_config(config_path))
         throw std::runtime_error("Config file not found");
}

Webserv::Webserv(std::string conf)
{
    config_path = conf;
    if (parse_config(config_path))
        throw std::runtime_error("Config file not found");
}

Webserv::Webserv(const Webserv &other)
{
    config_path = other.config_path;
    config_size = other.config_size;
    config = new std::string[config_size];
    for (int i = 0; i < config_size; i++)
        config[i] = other.config[i];
}

Webserv &Webserv::operator=(const Webserv &other)
{
    if (this == &other)
        return (*this);
    config_path = other.config_path;
    config_size = other.config_size;
    delete [] config;
    config = new std::string[config_size];
    for (int i = 0; i < config_size; i++)
        config[i] = other.config[i];
    return (*this);
}

Webserv::~Webserv()
{
    delete [] config;
}

// Config parsing
int Webserv::parse_config(const std::string& conf)
{
    int i = 0;
    std::string line;
    std::ifstream file(conf.c_str());

    if (!file.is_open())
        return 1;

    while (getline(file, line))
        i++;

    config_size = i;
    config = new std::string[i];
    file.close();
    file.open(conf.c_str());
    i = 0;

    while (getline(file, line))
    {
        config[i] = line;
        i++;
    }

    file.close();
    return 0;
}

std::string* Webserv::get_config() const
{
	return config;
}

int Webserv::get_config_size() const
{
	return config_size;
}
