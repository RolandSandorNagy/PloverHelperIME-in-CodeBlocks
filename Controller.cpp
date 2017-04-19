#include "Controller.h"
#include "Server.h"
#include "View.h"


namespace global
{
    extern View* hgView;

    extern bool isRunning;
}



Controller::Controller(View* v, Server *s)
{
    isActive = global::isRunning;
    view = v;
    server = s;
}

Controller::Controller() {}

Controller::~Controller() {}

void Controller::processMessage(char* recvbuf, int recvbuflen, unsigned int iResult)
{
    std::cout << "recvbuf: " << recvbuf << std::endl;
    if(commandReceived(recvbuf))
        return;

    messageReceived(recvbuf, recvbuflen, iResult);
}

bool Controller::commandReceived(char* recvbuf)
{
    std::string str(recvbuf);
    if(str.substr(0,5) == "CMD::")
    {
        std::cerr << "processing: " << str.substr(5, str.size()) << std::endl;
        processCommand(str.substr(5, str.size()));
        return true;
    }
    return false;
}

void Controller::messageReceived(char* recvbuf, int recvbuflen, unsigned int iResult)
{
    int size_needed;
    std::string s(recvbuf);
    std::wstring ws = s2ws(s, &size_needed);
    //std::wstring ws = s2ws(recvbuf, &size_needed);

    std::cout << "recvbuf: " << recvbuf << std::endl;
    std::cout << "s: " << s << std::endl;

    view->displayMessage(ws);
}

void Controller::processCommand(std::string str)
{
    if(!isActive && str != "RESUME")
        return;
    else if(str == "PAUSE")
        proceedPause();
    else if(str == "RESUME")
        proceedResume();
    else if(str == "STOP")
        proceedStop();
    else if(str == "SHOW")
        proceedShow();
    else if(str == "HIDE")
        proceedHide();
    else if(str == "UNDO")
        proceedUndo();
    else if(str == "SAVE")
        proceedSave();

    // std::cout << "it didnt return!" << std::endl;
}

void Controller::proceedPause()
{
    isActive = false;
}

void Controller::proceedResume()
{
    isActive = true;
}

void Controller::proceedStop()
{
    global::isRunning = false;
    view->closeView();
}

void Controller::proceedShow()
{
    // TODO:    consideration needed about what to do here
    //          could be forcing the popup to be seen
    //          even thought it has (0,0) coordinates;
    //          Or it should be just another flag for
    //          visibility.
    view->showPopup();
}

void Controller::proceedHide()
{
    view->hidePopup();
}

void Controller::proceedUndo()
{
    // TODO: Not clear yet what to do here.
}

void Controller::proceedSave()
{
    // TODO: We want to be able to save the last input into a file
}

std::wstring Controller::s2ws(const std::string& str, int *size_needed)
{
    *size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo( *size_needed, 0 );
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], *size_needed);
    return wstrTo;
}


