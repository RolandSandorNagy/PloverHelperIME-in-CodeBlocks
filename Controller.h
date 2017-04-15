#ifndef CONTROLLER_H_INCLUDED
#define CONTROLLER_H_INCLUDED

#include "includes.h"
#include "View.h"
//#include "Server.h"

class Controller
{

public: /* members */


private: /* members */
    View* view;
    bool isActive = true;
    FILE file;

public:  /* methods */
    Controller(View*);
    ~Controller();
    void processMessage(char*, int, unsigned int);

private: /* methods */
    POINT adjustPopUp();
    POINT getCaretPosition();
    std::wstring s2ws(const std::string&, int *size_needed);
    void proceedPause();
    void proceedResume();
    void proceedStop();
    void proceedShow();
    void proceedHide();
    void proceedUndo();
    void proceedSave();
    void proceedDefault(char*);

};


#endif // CONTROLLER_H_INCLUDED
