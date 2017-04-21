#include "globals.h"
#include "Config.h"
#include "Server.h"


int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{

    Config config;
    if(config.file_not_found)
        return -1;

    pthread_t windowThread;
    pthread_t serverThread;

    pthread_create(&windowThread, NULL, ViewNS::wThreadMethod, (void*)&hThisInstance);

    while(!global::hgView){}
    global::hgView->setPopupTimeout(config.getPopupTimeout());

    pthread_create(&serverThread, NULL, ServerNS::sThreadMethod, (void*)&config);

    void *windowThreadStatus;
    void *serverThreadStatus;

    pthread_join(windowThread, &windowThreadStatus);
    pthread_join(serverThread, &serverThreadStatus);


    return 0;
}
