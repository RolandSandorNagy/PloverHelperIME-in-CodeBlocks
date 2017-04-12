#include "Controller.h"



Controller::Controller(View* v)
{
    view = v;
}

Controller::~Controller()
{

}

void Controller::processMessage(char* recvbuf, int recvbuflen, unsigned int iResult)
{
    std::cout << "Processing..." << std::endl;
    std::cout << "msg: " << recvbuf << std::endl << std::endl;
    // Todo...

	POINT p = adjustPopUp();
	//drawStringOnPopUp(ws, iResult, p);
}

POINT Controller::adjustPopUp()
{
	POINT p = getCaretPosition();
	if (p.y < 35)
	{
		view->hidePopup();
		return p;
	}
	view->movePopup(p.x - 150, p.y + 25, 300, 300);
    view->showPopup();

	return p;
}

POINT Controller::getCaretPosition()
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


