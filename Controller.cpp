#include "Controller.h"
#include "Suggestion.h"
#include "Server.h"
#include "View.h"


#define CMD_PREFIX "CMD::"
#define SAVE_FILE_NAME "lastinput.txt"


namespace global
{
    extern View* hgView;

    extern bool isRunning;

    std::wstring s2ws(const std::string& str, int* size_needed)
    {
        *size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
        std::wstring wstrTo( *size_needed, 0 );
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
    std::string s(recvbuf);
    std::wstring ws = global::s2ws(s, &size_needed);

    inputHistory.push_back(ws);

    collectSuggestions(ws);
    sortSuggestions();

    view->displaySuggestions(suggestions);
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
    view->showPopup(suggestions);
}

void Controller::proceedHide()
{
    view->hidePopup();
}

void Controller::proceedUndo()
{
    if(inputHistory.size() == 1)
    {
        inputHistory.pop_back();
        return;
    }
    else if(inputHistory.size() == 0)
        return;
    handleSuggestions();
    inputHistory.pop_back();
    collectSuggestions(inputHistory[inputHistory.size() - 1]);
    sortSuggestions();
    view->displaySuggestions(suggestions);
}

void Controller::handleSuggestions()
{
    std::wstring ws = inputHistory[inputHistory.size() - 1];
    for(int i = 0; i < (int)suggestions.size(); ++i)
    {
        if(suggestions[i].getWText() == ws)
        {
            int mult = suggestions[i].getMultiplicity();
            mult--;
            suggestions[i].setMultiplicity(mult);
            return;
        }
    }
}

void Controller::proceedSave()
{
    if(inputHistory.size() == 0)
        return;
    file.open(SAVE_FILE_NAME);
    file << inputHistory[inputHistory.size() - 1].c_str() << std::endl;
    file.close();
}

void Controller::collectSuggestions(std::wstring ws)
{
    suggestions.clear();
    for(int i = 0; i < (int)inputHistory.size() - 1; ++i)
    {
        if(inputHistory[i] == ws)
        {
            if(suggestionsHasMember(inputHistory[i + 1]))
                continue;
            Suggestion s(1, inputHistory[i + 1]);
            suggestions.push_back(s);
        }
    }
}

bool Controller::suggestionsHasMember(std::wstring ws)
{
    for(int i = 0; i < (int)suggestions.size(); ++i)
    {
        if(suggestions[i].getWText() == ws)
        {
            int mult = suggestions[i].getMultiplicity();
            mult++;
            suggestions[i].setMultiplicity(mult);
            return true;
        }
    }
    return false;
}

void Controller::sortSuggestions()
{
    std::sort(suggestions.begin(), suggestions.end(), suggestion_compare_operator());
}


