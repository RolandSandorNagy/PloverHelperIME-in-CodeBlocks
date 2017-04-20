#ifndef VIEW_H_INCLUDED
#define VIEW_H_INCLUDED

#include "defines.h"
#include "includes.h"
#include "Suggestion.h"


class View
{

public: /* members */


private: /* members */
    HINSTANCE *hInstance;
    HWND hwnd;
    WNDCLASSEX wincl;
    bool show;
    int ln;
    int popupWidth;
    int popupHeight;
    COLORREF bgColor;


public: /* methods */
    View(HINSTANCE*);
    View();
    ~View();
    void showPopup();
    void hidePopup();
    bool getShow();
    void movePopup(int, int, int, int);
    void clearPopup(int);
    void closeView();
    void adjustPopUp(int);
    void displaySuggestions(std::vector<Suggestion>);

private: /* methods */
    void initWincl(HINSTANCE*);
    bool register_Class();
    void createWindow();
    void handleNextLine(HDC);
    POINT getCaretPosition();
    void drawStringOnPopUp(std::wstring, unsigned int, int);
    void displayBestTenSuggestion(std::vector<Suggestion>);


};


namespace ViewNS
{
    extern void* wThreadMethod(void*);
}


#endif // VIEW_H_INCLUDED
