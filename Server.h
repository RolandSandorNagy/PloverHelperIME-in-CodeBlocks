#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED


#include "defines.h"
#include "includes.h"

class View;
class Controller;


struct thread_data{
    HINSTANCE *hIns;
    View *v;
};



class Server
{

public: /* members */


private: /* members */
    HINSTANCE* hInstance;
	WSADATA wsaData;

	SOCKET ListenSocket;
	SOCKET ClientSocket;

	struct addrinfo *result;
	struct addrinfo hints;

	int iResult;
	int recvbuflen;
	char recvbuf[DEFAULT_BUFLEN];

    View *view;
    Controller *controller;


public: /* methods */
    Server(HINSTANCE*);
    Server();
    ~Server();

    void run();


private: /* methods */
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
