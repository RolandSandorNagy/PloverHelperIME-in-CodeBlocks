#ifndef VIEW_H_INCLUDED
#define VIEW_H_INCLUDED

#include "defines.h"
#include "includes.h"
#include "Suggestion.h"


class View
{

public: /* members */
    int maxThreadId;


private: /* members */
    HINSTANCE *hInstance;
    WNDCLASSEX wincl;
    HWND hwnd;
    int popupTimeout;
    int popupWidth;
    int popupHeight;
    int ln;
    COLORREF greenColor;
    COLORREF fontColor;
    COLORREF bgColor;
    pthread_t *timeoutThread = NULL;


public: /* methods */
    View(HINSTANCE*);
    View();
    ~View();
    void showPopup(std::vector<Suggestion>, std::wstring);
    void hidePopup();
    void closeView();
    void displaySuggestions(std::vector<Suggestion>, Suggestion);
    int getPopupTimeout();
    void setPopupTimeout(int);


private: /* methods */
    void initWincl(HINSTANCE*);
    bool register_Class();
    void createWindow();
    void movePopup(int, int, int, int);
    void drawStringOnPopUp(Suggestion s, int);
    void handleNextLine(HDC);
    void clearPopup(int);
    void adjustPopUp(int, int);
    void avoidScreenEdges(POINT*);
    POINT getCaretPosition();
    int getMaxOffset(std::vector<Suggestion>);
    void hideTimeout();
    std::wstring getSubWString(std::wstring, int, int);

};


namespace ViewNS
{
    extern void* wThreadMethod(void*);
}


#endif // VIEW_H_INCLUDED
