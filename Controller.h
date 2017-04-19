#ifndef CONTROLLER_H_INCLUDED
#define CONTROLLER_H_INCLUDED

#include "includes.h"

class View;
class Server;


class Controller
{

public: /* members */


private: /* members */
    View* view;
    Server* server;
    std::ofstream file;
    bool isActive;
    std::wstring lastInput;
    std::vector<std::wstring> inputHistory;

public:  /* methods */
    Controller(View*, Server*);
    Controller();
    ~Controller();

    void processMessage(char*, int, unsigned int);

private: /* methods */
    POINT getCaretPosition();
    std::wstring s2ws(const std::string&, int *size_needed);
    void proceedPause();
    void proceedResume();
    void proceedStop();
    void proceedShow();
    void proceedHide();
    void proceedUndo();
    void proceedSave();
    bool commandReceived(char*);
    void messageReceived(char*, int , unsigned int);
    void processCommand(std::string);


};


#endif // CONTROLLER_H_INCLUDED
