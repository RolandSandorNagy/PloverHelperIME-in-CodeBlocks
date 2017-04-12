#include "Controller.h"

Controller::Controller()
{

}

Controller::~Controller()
{

}

void Controller::processMessage(char* recvbuf, int recvbuflen, int iResult)
{
    std::cout << "Processing..." << std::endl;
    // Todo...
    std::cout << "msg: " << recvbuf << std::endl;
}
