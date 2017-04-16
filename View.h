#ifndef VIEW_H_INCLUDED
#define VIEW_H_INCLUDED

#include "defines.h"
#include "includes.h"


class View
{

public: /* members */


private: /* members */
    HINSTANCE *hInstance;
    HWND hwnd;
    WNDCLASSEX wincl;
    bool show;
    int ln;


public: /* methods */
    View(HINSTANCE*);
    View();
    ~View();
    void showPopup();
    void hidePopup();
    bool getShow();
    void movePopup(int, int, int, int);
    void drawStringOnPopUp(std::wstring, unsigned int);
    void clearPopup(int);
    void closeView();
    void adjustPopUp();
    void displayMessage(std::wstring);

private: /* methods */
    void initWincl(HINSTANCE*);
    bool register_Class();
    void createWindow();
    void handleNextLine(HDC);
    POINT getCaretPosition();

};


namespace ViewNS
{
    extern void* wThreadMethod(void*);
}


#endif // VIEW_H_INCLUDED
