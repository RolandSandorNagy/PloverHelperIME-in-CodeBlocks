#include "globals.h"
#include "Config.h"
#include "Server.h"
#include <fstream>

bool lockFileExist();
void makeLockFile();
void deleteLockFile();


int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    if(lockFileExist())
        return -1;
    else
        makeLockFile();

    Config config;
    if(config.file_not_found)
        return -2;

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

    deleteLockFile();

    return 0;
}

bool lockFileExist()
{
    std::ifstream my_file("C:\\Users\\Rol\\AppData\\Local\\plover\\plover\\lock.txt");
    if (my_file)
    {
        deleteLockFile();
        return true;
    }
    return false;
}

void makeLockFile()
{
    std::ofstream lockFile("C:\\Users\\Rol\\AppData\\Local\\plover\\plover\\lock.txt");
    if(lockFile.is_open())
        lockFile.close();
}

void deleteLockFile()
{
    if( remove( "C:\\Users\\Rol\\AppData\\Local\\plover\\plover\\lock.txt" ) != 0 )
        perror( "Error deleting file" );
    else
        puts( "File successfully deleted" );
}
