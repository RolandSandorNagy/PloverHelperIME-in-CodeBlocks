#include "Controller.h"
#include "Server.h"
#include "View.h"


#define CMD_PREFIX "CMD::"
#define SAVE_FILE_NAME "lastinput.txt"
#define NO_SUGGESTION "none"
#define SUGG_DELIMETER_CHAR ';'
#define STROKE_DELIMETER_CHAR ':'
#define CURRENT_STROKE "c/u/r/r/e/n/t"


namespace global
{
    extern View* hgView;

    extern bool isRunning;

    std::wstring s2ws(const std::string& str, int* size_needed)
    {
        *size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
        std::wstring wstrTo(*size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], *size_needed);
        return wstrTo;
    }
}


Controller::Controller(View* v, Server *s)
{
    isActive = global::isRunning;
    view = v;
    server = s;
    inputHistory.clear();
    suggestions.clear();
}

Controller::Controller() {}

Controller::~Controller() {}

void Controller::processMessage(char* recvbuf, int recvbuflen, unsigned int iResult)
{
    if(commandReceived(recvbuf))
        return;
    if(isActive)
        messageReceived(recvbuf, recvbuflen, iResult);
}

bool Controller::commandReceived(char* recvbuf)
{
    std::string str(recvbuf);
    if(str.substr(0, strlen(CMD_PREFIX)) == CMD_PREFIX)
    {
        processCommand(str.substr(strlen(CMD_PREFIX), str.size()));
        return true;
    }
    return false;
}

void Controller::messageReceived(char* recvbuf, int recvbuflen, unsigned int iResult)
{
    int size_needed;
    std::string sv_str(recvbuf);
    suggestions = createSuggestionVector(sv_str);
    view->displaySuggestions(suggestions, current_stroke);
}

std::vector<Suggestion> Controller::createSuggestionVector(std::string sv_str)
{
    std::vector<Suggestion> suggs;
    suggs.clear();
    if(sv_str == NO_SUGGESTION)
        return suggs;
    return buildSuggestions(sv_str);
}

std::vector<Suggestion> Controller::buildSuggestions(std::string sv_str)
{
    std::vector<Suggestion> suggs;
    std::string s;
    std::stringstream s1(sv_str);
    while( getline(s1, s, SUGG_DELIMETER_CHAR) )
    {
        std::stringstream sparts(s);
        std::string stroke;
        std::string translation;
        getline(sparts, stroke, STROKE_DELIMETER_CHAR);
        getline(sparts, translation, STROKE_DELIMETER_CHAR);
        if(stroke == CURRENT_STROKE)
            storeCurrentStroke(&sparts, translation);
        else
            addSuggestionToSuggs(&suggs, stroke, translation);
    }
    return suggs;
}

void Controller::storeCurrentStroke(std::stringstream *sparts, std::string translation)
{
    std::string translation2;
    getline(*sparts, translation2, STROKE_DELIMETER_CHAR);
    int size_needed;
    current_stroke.setWStroke(global::s2ws(translation2, &size_needed));
    current_stroke.setWText(global::s2ws(translation, &size_needed));
}

void Controller::addSuggestionToSuggs(std::vector<Suggestion> *suggs, std::string stroke, std::string translation)
{
    int size_needed;
    std::wstring wstroke = global::s2ws(stroke, &size_needed);
    std::wstring wtranslation = global::s2ws(translation, &size_needed);
    Suggestion s(0, wstroke, wtranslation);
    suggs->push_back(s);
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
    else if(str == "SAVE")
        proceedSave();
}

void Controller::proceedPause()
{
    isActive = false;
    proceedHide();
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
    view->displaySuggestions(suggestions, current_stroke);
}

void Controller::proceedHide()
{
    view->hidePopup();
}


void Controller::proceedSave()
{
    // TODO
}


