#ifndef VIEW_H_INCLUDED
#define VIEW_H_INCLUDED

#include "defines.h"
#include "includes.h"


class View
{
/* Public members*/
public:

/* Private members */
private:
    HINSTANCE *hInstance;    /* This is the handle for our program */
    HWND hwnd;               /* This is the handle for our window */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

/* Public methods */
public:
    View(HINSTANCE*);
    ~View();
    void show();
    void hide();

/* Private methods */
private:
    void initWincl(HINSTANCE*);
    bool register_Class();
    void createWindow();

};


namespace ViewNS
{
    extern void* wThreadMethod(void*);
}


#endif // VIEW_H_INCLUDED
