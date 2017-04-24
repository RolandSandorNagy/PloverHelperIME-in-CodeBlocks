#include "View.h"


#define WINDOW_NAME "MIME POPUP WINDOW"
#define MIME_APP_NAME "MIME Windows App"
#define DEFAULT_POPUP_HEIGHT 300
#define DEFAULT_POPUP_WIDTH 300
#define PLOVER_GRAY 240
#define MARGIN 5
#define LINE_HEIGHT 20
#define MULT_WIDTH 75
#define SUG_WIDTH 10
#define MAX_LINES 20
#define MAX_SUGGS 10
#define SECOND 1000


namespace ViewNS
{
    bool on;
    TCHAR szClassName[ ] = _T(WINDOW_NAME);

    LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
    void* wThreadMethod(void*);
    void* toThreadMethod(void* view);
}

namespace global
{
    extern View* hgView;

    extern bool isRunning;

    extern std::wstring s2ws(const std::string&, int*);
}


View::View(HINSTANCE* hInst)
{
    hInstance = hInst;

    popupHeight = DEFAULT_POPUP_HEIGHT;
    popupWidth  = DEFAULT_POPUP_WIDTH;
    ViewNS::on = true;
    maxThreadId = 0;
    ln = 0;

    bgColor = RGB(PLOVER_GRAY, PLOVER_GRAY, PLOVER_GRAY);
    fontColor = RGB(183, 183, 18); // some yellowish

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
    wincl.hbrBackground = CreateSolidBrush(bgColor);
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
                            _T(MIME_APP_NAME),
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

void View::showPopup(std::vector<Suggestion> suggestions)
{
    ShowWindow(hwnd, SW_SHOW);
    show = true;

    clearPopup(MAX_LINES + 1);
    displayBestTenSuggestion(suggestions);
}

void View::showPopup(std::vector<Suggestion> suggestions, std::wstring current)
{
    ShowWindow(hwnd, SW_SHOW);
    show = true;

    int num = current.size() / 2 + 1;

    clearPopup(MAX_LINES + 1);
    for(int i = suggestions.size() - 1; i >= 0 /*&& suggestions.size() - i < MAX_SUGGS*/; --i)
        drawStringOnPopUp(suggestions[i], num);
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
	RECT rect;

	HDC hDC = GetDC(hwnd);

	SetBkColor(hDC, bgColor);
	SetTextColor(hDC, fontColor);

    rect.left   = MARGIN;
    rect.top    = ln * LINE_HEIGHT;
    rect.right  = MARGIN + popupWidth;
    rect.bottom = (ln + 1) * LINE_HEIGHT;
	DrawText(hDC, ws.c_str(), ws.length(), &rect, 0);

    int size_needed;
    std::ostringstream ss;
    ss << mult;
    std::string s = ss.str();
    std::wstring wsmult = global::s2ws(s, &size_needed);

    rect.left   = popupWidth - MULT_WIDTH;
    DrawText(hDC, wsmult.c_str(), wsmult.length(), &rect, 0);

	EndPaint(hwnd, &ps);
	handleNextLine(hDC);
}

void View::drawStringOnPopUp(Suggestion s, int num)
{
	PAINTSTRUCT ps;
	RECT rect;

	HDC hDC = GetDC(hwnd);

	SetBkColor(hDC, bgColor);
	SetTextColor(hDC, fontColor);

    rect.left   = MARGIN;
    rect.top    = ln * LINE_HEIGHT;
    rect.right  = MARGIN + popupWidth;
    rect.bottom = (ln + 1) * LINE_HEIGHT;
    //if(num > 0)
    //{
        //std::wstring pre = getPre

    //} else
        DrawText(hDC, s.getWStroke().c_str(), s.getWStroke().length(), &rect, 0);

    rect.left   = popupWidth - MULT_WIDTH;
    DrawText(hDC, s.getWText().c_str(), s.getWText().length(), &rect, 0);

	EndPaint(hwnd, &ps);
	handleNextLine(hDC);
}

void View::handleNextLine(HDC hDC)
{
	ln = (ln > MAX_LINES ? 0 : ln + 1);
	if (ln == 0)
    clearPopup(MAX_LINES + 1);
}

void View::clearPopup(int l)
{
    if(ln + l <= MAX_LINES)
        return;

    RECT rect;
    HDC hDC = GetDC(hwnd);
    rect.left = 0;
    rect.top = 0;
    rect.right = DEFAULT_POPUP_WIDTH;
    rect.bottom = DEFAULT_POPUP_HEIGHT;
    FillRect(hDC, &rect, CreateSolidBrush(bgColor));
    SetROP2(hDC, R2_NOTXORPEN);
    ln = 0;
}

void View::closeView()
{
    SendMessage(hwnd, WM_DESTROY, 0, 0);
}

void View::adjustPopUp(int enrties, int maxOffset)
{
    Sleep(0);
	POINT p = getCaretPosition();
	avoidScreenEdges(&p);
	if(p.y < 35)
	{
		hidePopup();
		return;
	}
    popupWidth = maxOffset * SUG_WIDTH + MARGIN + (2 * MULT_WIDTH);
	popupHeight = enrties * LINE_HEIGHT;
	movePopup(p.x, p.y, popupWidth, popupHeight);
	return;
}

void View::avoidScreenEdges(POINT* p)
{
    RECT actualDesktop;
    GetWindowRect(GetDesktopWindow(), &actualDesktop);

    int W = actualDesktop.right;                // get screen width
    int H = GetSystemMetrics(SM_CYFULLSCREEN);  // get screen height
    int w = popupWidth;                         // get popup width
    int h = popupHeight;                        // get popup height
    int wb = W - w;                             // width border
    int hb = H - h;                             // height border

    if(p->x < 0 || p->x > W || p->y < 0 || p->y > actualDesktop.bottom)
        return;

    if(p->x <= wb && p->y <= hb)
        return;
    else if(p->x > wb && p->y <= hb) // Right side of screen
        p->x = wb;
    else if(p->x <= wb && p->y > hb) // Bottom side of screen
        p->y = hb;
    else if(p->x > wb && p->y > hb)  // Bottom right corner of screen
    {
        p->x = wb;
        p->y = hb - LINE_HEIGHT - h;
    }
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
    //if(suggestions.size() == 0)
        //return;

    adjustPopUp(suggestions.size(), getMaxOffset(suggestions));
    showPopup(suggestions);
    hideTimeout();
}

void View::displaySuggestions(std::vector<Suggestion> suggestions, Suggestion current)
{
    //if(suggestions.size() == 0)
        //return;
    suggestions.push_back(current);

    adjustPopUp(suggestions.size(), getMaxOffset(suggestions));
    showPopup(suggestions, current.getWStroke());
    hideTimeout();
}

int View::getMaxOffset(std::vector<Suggestion> suggestions)
{
    int max = 0;
    for(int i = suggestions.size() - 1; i >= 0 && suggestions.size() - i < MAX_SUGGS; --i)
        if((int)suggestions[i].getWText().size() > max)
            max = suggestions[i].getWText().size();
    return max;
}

void View::hideTimeout()
{
    pthread_t toThread;
    maxThreadId++;
    pthread_create(&toThread, NULL, ViewNS::toThreadMethod, (void*)maxThreadId);
    timeoutThread = &toThread;
}

void View::displayBestTenSuggestion(std::vector<Suggestion> suggestions)
{
    for(int i = suggestions.size() - 1; i >= 0 /*&& suggestions.size() - i < MAX_SUGGS*/; --i)
        //drawStringOnPopUp(suggestions[i]);
        drawStringOnPopUp(suggestions[i], i);
        //drawStringOnPopUp(suggestions[i].getWText(), suggestions[i].getWText().size(), suggestions[i].getMultiplicity());
}

int View::getPopupTimeout()
{
    return popupTimeout;
}

void View::setPopupTimeout(int val)
{
    popupTimeout = val;
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
    global::hgView = &view;

    MSG messages;
    while (GetMessage (&messages, NULL, 0, 0) && global::isRunning)
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    return (void*)messages.wParam;
}

void* ViewNS::toThreadMethod(void* id)
{
    int to = ((View*)global::hgView)->getPopupTimeout();
    Sleep(to * SECOND);
    if(((View*)global::hgView)->maxThreadId != (INT)id)
        return (void*) 1;
    ((View*)global::hgView)->hidePopup();
    ((View*)global::hgView)->maxThreadId = 0;
    return (void*) 0;
}
