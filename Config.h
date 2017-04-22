#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include "includes.h"


class Config
{
public: /* members */
    bool file_not_found;


private: /* members */
    int popupTimeout;
    int suggest_by;
    std::string host;
    std::string port;
    char szPath[MAX_PATH];


public: /* methods */
    Config();
    ~Config();
    std::string getHost();
    std::string getPort();
    int getPopupTimeout();


private: /* methods */

};


#endif // CONFIG_H_INCLUDED
