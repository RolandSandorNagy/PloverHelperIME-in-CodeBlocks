#include "globals.h"
#include "Server.h"


int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{

    // TODO: Here should be checked if there is an
    // instance of this program already running.
    // If so, this instance should be terminated.
    //
    // ...
    //

    pthread_t windowThread;
    pthread_t serverThread;

    pthread_create(&windowThread, NULL, ViewNS::wThreadMethod, (void*)&hThisInstance);
    while(!global::hgView){}
    pthread_create(&serverThread, NULL, ServerNS::sThreadMethod, (void*)&hThisInstance);

    void *windowThreadStatus;
    void *serverThreadStatus;

    pthread_join(windowThread, &windowThreadStatus);
    pthread_join(serverThread, &serverThreadStatus);




    return 0;
}
