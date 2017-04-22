#include "Config.h"


#define CONFIG_FILE_LOCATION "\\plover\\plover\\plover.cfg"
#define IME_CONFIG_SECTION "[Ime Configuration]"
#define POPUP_TIMEOUT "popup_timeout = "
#define SUGGEST_BY "suggest_by = "
#define HOST "host = "
#define PORT "port = "
#define SECTION_START_CHAR '['


Config::Config()
{
    SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, szPath);
    //if(strlen(szPath) == 0)
        //szPath = "c:\\";
    std::string full_path(szPath);
    full_path += CONFIG_FILE_LOCATION;
    std::cout << "full_path: " << full_path << std::endl;
    std::ifstream config_file;
    config_file.open(full_path.c_str());
    file_not_found = true;

    if(!config_file.is_open())
        return;

    file_not_found = false;
    bool start = false;
    bool end = false;
    std::string line;

    while(getline(config_file, line))
    {
        if(line == IME_CONFIG_SECTION)
            start = true;
        if(start && !end && line != IME_CONFIG_SECTION && line[0] == SECTION_START_CHAR)
            end = true;

        if(start)
        {
            if(line.find(POPUP_TIMEOUT) != std::string::npos)
                popupTimeout = atoi(line.substr(strlen(POPUP_TIMEOUT), line.length()).c_str());
            else if(line.find(SUGGEST_BY) != std::string::npos)
                suggest_by = atoi(line.substr(strlen(SUGGEST_BY), line.length()).c_str());
            else if(line.find(HOST) != std::string::npos)
                host = line.substr(strlen(HOST), line.length());
            else if(line.find(PORT) != std::string::npos)
                port = line.substr(strlen(PORT), line.length());
        }
    }
    config_file.close();
}

Config::~Config()
{

}

std::string Config::getHost()
{
    return host;
}

std::string Config::getPort()
{
    return port;
}

int Config::getPopupTimeout()
{
    return popupTimeout;
}

