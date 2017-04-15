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
    bool show;
    int ln;

/* Public methods */
public:
    View(HINSTANCE*);
    ~View();
    void showPopup();
    void hidePopup();
    bool getShow();
    void movePopup(int, int, int, int);
    void drawStringOnPopUp(std::wstring, unsigned int, POINT);
    void clearPopup(int);

/* Private methods */
private:
    void initWincl(HINSTANCE*);
    bool register_Class();
    void createWindow();
    void handleNextLine(HDC);

};


namespace ViewNS
{
    extern void* wThreadMethod(void*);
}


#endif // VIEW_H_INCLUDED
