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
    // Todo...
    std::string str(recvbuf);

    if(!isActive && str != "RESUME")
        return;

    std::cout << "Processing..." << std::endl;
    std::cout << "msg: " << recvbuf << std::endl;

    if(str == "PAUSE")
        proceedPause();
    if(str == "RESUME")
        proceedResume();
    else if(str == "STOP")
        proceedStop();
    else if(str == "SHOW")
        proceedShow();
    else if(str == "HIDE")
        proceedHide();
    else if(str == "UNDO")
        proceedUndo();
    else if(str == "SAVE")
        proceedSave();
    else proceedDefault(recvbuf);
}

void Controller::proceedPause()
{
    // TODO: set isActive flag to False
    isActive = false;
}

void Controller::proceedResume()
{
    // TODO: set isActive flag to True
    isActive = true;
}

void Controller::proceedStop()
{
    // TODO: We should end this program here
}

void Controller::proceedShow()
{
    // TODO:    consideration needed about what to do here
    //          could be forcing the popup to be seen
    //          even thought it has (0,0) coordinates;
    //          Or it should be just another flag for
    //          visibility.
}

void Controller::proceedHide()
{
    // TODO: Same consideration needed but in reverse.
}

void Controller::proceedUndo()
{
    // TODO: Not clear yet what to do here.
}

void Controller::proceedSave()
{
    // TODO: We want to be able to save the last input into a file
}

void Controller::proceedDefault(char* recvbuf)
{
    // TODO: this code should be replaced
    int size_needed;
    std::wstring ws = s2ws(recvbuf, &size_needed);
    POINT p = adjustPopUp();
    view->clearPopup(1);
    view->drawStringOnPopUp(ws, ws.size(), p);
}

std::wstring Controller::s2ws(const std::string& str, int *size_needed)
{
    //int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    *size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo( *size_needed, 0 );
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], *size_needed);
    return wstrTo;
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

