#include "Config.h"


Config::Config()
{
    std::ifstream config_file;
    config_file.open("C:\\Users\\Rol\\AppData\\Local\\plover\\plover\\plover.cfg");

    if(config_file.is_open())
    {
        std::cout << "File is opened" << std::endl;

        std::string line;
        bool start = false;
        bool end = false;
        while(getline(config_file, line))
        {
            if(line == "[Ime Configuration]")
                start = true;
            if(start && !end && line != "[Ime Configuration]" && line[0] == '[')
                end = true;

            if(start)
            {
                if(line.find("popup_timeout") != std::string::npos)
                {
                    std::string str = line.substr(16, line.length());
                    popupTimeout = atoi(str.c_str());
                }
                else if(line.find("suggest_by") != std::string::npos)
                {
                    std::string str = line.substr(13, line.length());
                    suggest_by = atoi(str.c_str());
                }
                else if(line.find("host") != std::string::npos)
                {
                    host = line.substr(7, line.length());
                    std::cout << "host read from file: " << host << std::endl;
                }
                else if(line.find("port") != std::string::npos)
                {
                    std::string str = line.substr(7, line.length());
                    port = str;
                    //port = atoi(str.c_str());
                }
            }
        }
        file_not_found = false;
        config_file.close();
        return;
    }
    std::cout << "file was not found" << std::endl;
    file_not_found = true;
}

Config::~Config()
{

}

std::string Config::getHost()
{
    return host;
}

std::string Config::getPort()
//int Config::getPort()
{
    return port;
}

int Config::getPopupTimeout()
{
    return popupTimeout;
}

