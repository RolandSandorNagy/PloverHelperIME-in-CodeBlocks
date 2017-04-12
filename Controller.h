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

};


#endif // CONTROLLER_H_INCLUDED
