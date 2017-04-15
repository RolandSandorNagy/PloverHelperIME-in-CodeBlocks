#include "View.h"



namespace ViewNS
{
    bool on;
    TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

    LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
    void* wThreadMethod(void*);
}


namespace global
{
    extern View* hgView;
}



View::View(HINSTANCE *hInst)
{
    ViewNS::on = true;
    ln = 0;

    hInstance = hInst;

    initWincl(hInst);

    if(!register_Class())
        return;

    createWindow();
}

View::~View()
{

}

void View::initWincl(HINSTANCE *hInst)
{
    /* The Window structure */
    wincl.hInstance = *hInst;
    wincl.lpszClassName = ViewNS::szClassName;
    wincl.lpfnWndProc = ViewNS::WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */

    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
}

bool View::register_Class()
{
    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return false;
    return true;
}

void View::createWindow()
{
    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx ( WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
                            ViewNS::szClassName,
                            _T("Code::Blocks Template Windows App"),
                            WS_POPUP,
                            0,
                            0,
                            300,
                            300,
                            NULL,
                            NULL,
                            *hInstance,
                            NULL);
}

void View::showPopup()
{
    ShowWindow(hwnd, SW_SHOW);
    show = true;
    //UpdateWindow(hwnd);
}

void View::hidePopup()
{
    ShowWindow(hwnd, SW_HIDE);
    show = false;
    //UpdateWindow(hwnd);
}

bool View::getShow()
{
    return show;
}

void View::movePopup(int x, int y, int width, int height)
{
	MoveWindow(hwnd, x, y, width, height, true);
}

void View::drawStringOnPopUp(std::wstring ws, unsigned int length, POINT p)
{
	PAINTSTRUCT ps;
	HDC hDC = GetDC(hwnd);
	RECT rect = { 5, 5 + ln * 20, 305, 5 + (ln + 1) * 20 };

	SetTextColor(hDC, RGB(0, 0, 0));
	SetBkColor(hDC, RGB(200, 200, 200));

    std::cout << "WS TO DRAW IS: '" << ws.c_str() << "'" << std::endl;
	DrawText(hDC, ws.c_str(), ws.length(), &rect, 0);
	EndPaint(hwnd, &ps);
	handleNextLine(hDC);
}

void View::handleNextLine(HDC hDC)
{
	ln = (ln > 14 ? 0 : ln + 1);
	if (ln == 0)
    clearPopup(15);
}

void View::clearPopup(int l)
{
    if(ln + l <= 14)
        return;
    HDC hDC = GetDC(hwnd);
    RECT r = { 0, 0, 300, 300 };
    FillRect(hDC, &r, (HBRUSH)(LTGRAY_BRUSH));
    SetROP2(hDC, R2_NOTXORPEN);
    ln = 0;
}


// ----------------------------------------------------- //
/* ***************************************************** */
// ----------------------------------------------------- //



LRESULT CALLBACK ViewNS::WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;

        case WM_CHAR:
            if (wParam == VK_ESCAPE) {
                std::cout << "ESC was pressed on popup window." << std::endl;
                global::hgView->hidePopup();
                //PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            }
            break;
            //else if(wParam == VK_RETURN) {
                //std::cout << "Enter was pressed." << std::endl;
                //return DefWindowProc (hwnd, message, wParam, lParam);
            //} else {
                //std::cout << "A key was pressed: " << (char)wParam << std::endl;
                //return DefWindowProc (hwnd, message, wParam, lParam);
            //}
            //break;
        //case SW_SHOW:
            //std::cout << "Show msg received." << std::endl;
            //return DefWindowProc (hwnd, message, wParam, lParam);
        default:                      /* for messages that we don't deal with */
            //std::cout << "msg received: " << message << std::endl;
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

void* ViewNS::wThreadMethod(void* hInst)
{
    View view((HINSTANCE*)hInst);
    view.showPopup();

    global::hgView = &view;


    MSG messages;
    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return (void*)messages.wParam;
}

