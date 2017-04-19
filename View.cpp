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

    extern bool isRunning;

    extern std::wstring s2ws(const std::string&, int*);
}


View::View(HINSTANCE *hInst)
{
    ViewNS::on = true;
    ln = 0;
    popupWidth  = 300;
    popupHeight = 300;


    hInstance = hInst;

    initWincl(hInst);

    if(!register_Class())
        return;

    createWindow();
}

View::View() {}

View::~View() {}

void View::initWincl(HINSTANCE *hInst)
{
    wincl.hInstance = *hInst;
    wincl.lpszClassName = ViewNS::szClassName;
    wincl.lpfnWndProc = ViewNS::WindowProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof (WNDCLASSEX);
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
}

bool View::register_Class()
{
    if (!RegisterClassEx (&wincl))
        return false;
    return true;
}

void View::createWindow()
{
    hwnd = CreateWindowEx ( WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
                            ViewNS::szClassName,
                            _T("Code::Blocks Template Windows App"),
                            WS_POPUP,
                            0,
                            0,
                            popupWidth,
                            popupHeight,
                            NULL,
                            NULL,
                            *hInstance,
                            NULL);
}

void View::showPopup()
{
    ShowWindow(hwnd, SW_SHOW);
    show = true;
}

void View::hidePopup()
{
    ShowWindow(hwnd, SW_HIDE);
    show = false;
}

bool View::getShow()
{
    return show;
}

void View::movePopup(int x, int y, int width, int height)
{
	MoveWindow(hwnd, x, y, width, height, true);
}

void View::drawStringOnPopUp(std::wstring ws, unsigned int length, int mult)
{
	PAINTSTRUCT ps;
	HDC hDC = GetDC(hwnd);

	SetTextColor(hDC, RGB(0, 0, 0));
	SetBkColor(hDC, RGB(200, 200, 200));

	RECT rect = { 5, 5 + ln * 20, 305, 5 + (ln + 1) * 20 };
	DrawText(hDC, ws.c_str(), ws.length(), &rect, 0);

    int size_needed;
    std::ostringstream ss;
    ss << mult;
    std::string s = ss.str();
    std::wstring wsmult = global::s2ws(s, &size_needed);
    rect = { 250, 5 + ln * 20, 305, 5 + (ln + 1) * 20 };
    DrawText(hDC, wsmult.c_str(), wsmult.length(), &rect, 0);

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

void View::closeView()
{
    SendMessage(hwnd, WM_DESTROY, 0, 0);
}

void View::adjustPopUp(int enrties)
{
	POINT p = getCaretPosition();
	if (p.y < 35)
	{
		hidePopup();
		return;
	}
	popupHeight = enrties * 20 + 5;
	movePopup(p.x - 150, p.y + 25, popupWidth, popupHeight);
    showPopup();

	return;
}

POINT View::getCaretPosition()
{
    POINT *point = new POINT();
    point->x = 0;
    point->y = 0;

	HWND Wnd = NULL;
	HWND Result = NULL;
	DWORD TId, PId;

    Result = GetFocus();
    Wnd = GetForegroundWindow();
	if (Result || !Wnd)
        return *point;

    TId = GetWindowThreadProcessId(Wnd, &PId);
    if (   !AttachThreadInput(GetCurrentThreadId(), TId, TRUE)
        || GetCaretPos(point) == 0)
        return *point;

    Result = GetFocus();
    ClientToScreen(Result, point);
    AttachThreadInput(GetCurrentThreadId(), TId, FALSE);
	return *point;
}

void View::displaySuggestions(std::vector<Suggestion> suggestions)
{
    if(suggestions.size() == 0)
    {
        hidePopup();
        return;
    }

    adjustPopUp(suggestions.size());
    clearPopup(15);
    displayBestTenSuggestion(suggestions);
}

void View::displayBestTenSuggestion(std::vector<Suggestion> suggestions)
{
    for(int i = suggestions.size() - 1; i >= 0 && suggestions.size() - i < 10; --i)
    {
        drawStringOnPopUp(suggestions[i].getWText(), suggestions[i].getWText().size(), suggestions[i].getMultiplicity());
    }
}


// ----------------------------------------------------- //
/* ***************************************************** */
// ----------------------------------------------------- //



LRESULT CALLBACK ViewNS::WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_DESTROY:
            PostQuitMessage (0);
            break;
        case WM_CHAR:
            if (wParam == VK_ESCAPE) {
                global::hgView->hidePopup();
            }
            break;
        default: return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

void* ViewNS::wThreadMethod(void* hInst)
{
    View view((HINSTANCE*)hInst);
    view.showPopup();

    global::hgView = &view;

    MSG messages;
    while (GetMessage (&messages, NULL, 0, 0) && global::isRunning)
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    return (void*)messages.wParam;
}

