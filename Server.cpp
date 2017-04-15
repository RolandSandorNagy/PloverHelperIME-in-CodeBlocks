#include "Server.h"


namespace ServerNS
{
    void* sThreadMethod(void* hInst);
}

namespace global
{
    extern View* hgView;

    extern bool isRunning;
}


Server::Server(HINSTANCE* data)
{
    initServer(data);
    controller = (Controller*)new Controller(global::hgView);
}

Server::~Server()
{

}

void Server::initServer(HINSTANCE* hInst)
{
    hInstance = hInst;

    recvbuflen = DEFAULT_BUFLEN;

	ListenSocket = INVALID_SOCKET;
	ClientSocket = INVALID_SOCKET;

	result = NULL;
}

void Server::run()
{
    while(global::isRunning)
    {
        initWinSock();
        resolveServerAddressAndPort();
        createSocket();
        setupListenSocket();
        acceptClientSocket();
        closeServerSocket();
        receiveUntilPeerShutsDown();
        shutDownConnection();
        CleanUp();
    }
}

bool Server::initWinSock()
{
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cout << "getaddrinfo failed with error: " << iResult << std::endl;
        WSACleanup();
        return false;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    return true;
}

bool Server::resolveServerAddressAndPort()
{
    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        std::cout << "getaddrinfo failed with error: " << iResult << std::endl;
        WSACleanup();
        return false;
    }
    return true;
}

bool Server::createSocket()
{
    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        std::cout << "socket failed with error: " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        WSACleanup();
        return false;
    }
    return true;
}

bool Server::setupListenSocket()
{
    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        std::cout << "bind failed with error: " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        CleanUp();
        return false;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        std::cout << "listen failed with error: " << WSAGetLastError() << std::endl;
        CleanUp();
        return false;
    }

    return true;
}

bool Server::acceptClientSocket()
{
    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        std::cout << "accept failed with error: " << WSAGetLastError() << std::endl;
        CleanUp();
        return false;
    }
    std::cout << "Plover has connected." << std::endl;
    return true;
}

void Server::closeServerSocket()
{
    // No longer need server socket
    closesocket(ListenSocket);
}


bool Server::receiveUntilPeerShutsDown()
{
    // Receive until the peer shuts down the connection
    do
    {
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            recvbuf[iResult] = '\0';
            controller->processMessage(recvbuf, recvbuflen, iResult);
        } else if (iResult < 0) {
            std::cout << "recv failed with error: " << WSAGetLastError() << std::endl;
            CleanUp();
            return false;
        }
    } while (iResult > 0);

    return true;
}

void Server::shutDownConnection()
{
    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        std::cout << "shutdown failed with error: " << WSAGetLastError() << std::endl;
        CleanUp();
        return;
    }
}

void Server::CleanUp()
{
    // cleanup
    closeServerSocket();
    WSACleanup();
}


// ----------------------------------------------------- //
/* ***************************************************** */
// ----------------------------------------------------- //


void* ServerNS::sThreadMethod(void* hInst)
{
    Server server((HINSTANCE*)hInst);
	while (true) server.run();

    return (void*)0;
}
