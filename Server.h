#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED


#include "defines.h"
#include "includes.h"
#include "View.h"
#include "Controller.h"


struct thread_data{
    HINSTANCE *hIns;
    View *v;
};


class Server
{
/* Public members*/
public:

/* Private members*/
private:
    HINSTANCE* hInstance;
	WSADATA wsaData;
	int iResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen;
	SOCKET ListenSocket;
	SOCKET ClientSocket;
	struct addrinfo *result = NULL;
	struct addrinfo hints;
    View *view;
    Controller *controller;

/* Public methods*/
public:
    Server(HINSTANCE*);
    ~Server();
    void run();

/* Private methods*/
private:
    void initServer(HINSTANCE*);
    bool initWinSock();
    bool resolveServerAddressAndPort();
    bool createSocket();
    bool setupListenSocket();
    bool acceptClientSocket();
    void closeServerSocket();
    bool receiveUntilPeerShutsDown();
    void shutDownConnection();
    void CleanUp();


};


namespace ServerNS
{
    extern void* sThreadMethod(void* hInst);
}



#endif // SERVER_H_INCLUDED
