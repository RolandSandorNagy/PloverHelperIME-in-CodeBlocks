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
    std::cout << "msg: " << recvbuf << std::endl;
    // Todo...
    std::cout << "spelling msg:"<< std::endl;
    for(unsigned int i = 0; i < iResult; ++i)
    {
        std::cout << recvbuf[i] << " ";
    }
    std::cout << std::endl << std::endl;

    std::stringstream ss(recvbuf);
    int c = 0;
    while(!ss.eof())
    {
        std::string s;
        ss >> s;
        ++c;
        std::cout << c << ". - " << s << std::endl;
    }
    std::cout << "c = " << c << std::endl << std::endl;

    std::string sugs[c - 1];
    std::stringstream ss2(recvbuf);
    int c2 = 0;
    while(!ss2.eof())
    {
        std::string s;
        ss2 >> s;
        if(c2 < c - 1)
        {
            sugs[c2] = s;
            std::cout << c2 << ". - " << sugs[c2] << std::endl;
        }
        ++c2;
    }

    for(int i = 1; i < c - 1; ++i)
    {
        if(sugs[i].compare("suggestions") == 0) {
            std::cout << "first is true" << std::endl;
            sugs[i - 1] += (" " + sugs[i]);
            for(int j = i + 1; j < c - 1; ++j)
            {
                sugs[j - 1] = sugs[j];
            }
            --c;
        } else if(sugs[i][0] == '/') {
            std::cout << "second is true" << std::endl;
            sugs[i - 1] += sugs[i];
            for(int j = i + 1; j < c - 1; ++j)
            {
                sugs[j - 1] = sugs[j];
            }
            --c;
        }
    }



	POINT p = adjustPopUp();
    int size_needed;
	std::wstring ws = s2ws(recvbuf, &size_needed);
    std::cout << "size_needed = " << size_needed << std::endl;
    std::cout << "recvbuflen = " << recvbuflen << std::endl;
    std::cout << "iResult = " << iResult << std::endl;
    std::cout << "(int)((std::string)recvbuf).size() = " << (int)((std::string)recvbuf).size() << std::endl;
    std::cout << "(int)ws.size() = " << (int)ws.size() << std::endl;

    view->clearPopup(c - 1);

    for(int i = 0; i < c - 1; ++i){
        int s_needed;
        std::wstring intent = (i == 0 ? L"" : L"   ");
        std::wstring wstr = s2ws(sugs[i], &s_needed);
        view->drawStringOnPopUp(intent + wstr, s_needed, p);
        //view->drawStringOnPopUp(ws, size_needed, p);
    }
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

