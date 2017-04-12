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

public:  /* methods */
    Controller(View*);
    ~Controller();
    void processMessage(char*, int, unsigned int);

private: /* methods */
    POINT adjustPopUp();
    POINT getCaretPosition();
    std::wstring s2ws(const std::string&, int *size_needed);
};


#endif // CONTROLLER_H_INCLUDED
