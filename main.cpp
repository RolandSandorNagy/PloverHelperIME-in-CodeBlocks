#include "globals.h"
#include "Config.h"
#include "Server.h"
#include <fstream>

#define LOCK_FILE_LOCATION "C:\\Users\\Rol\\AppData\\Local\\plover\\plover\\lock.txt"
#define DONE 0
#define LOCK_FILE_EXIST -1
#define CONFIG_FILE_NOT_FOUND -2


bool lockFileExist();
void makeLockFile();
void deleteLockFile();


int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    if(lockFileExist())
        return LOCK_FILE_EXIST;
    else
        makeLockFile();

    Config config;
    if(config.file_not_found)
        return CONFIG_FILE_NOT_FOUND;

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

    return DONE;
}

bool lockFileExist()
{
    std::ifstream lockFile(LOCK_FILE_LOCATION);
    if(lockFile)
        return true;
    return false;
}

void makeLockFile()
{
    std::ofstream lockFile(LOCK_FILE_LOCATION);
    if(lockFile.is_open())
        lockFile.close();
}

void deleteLockFile()
{
    if(remove(LOCK_FILE_LOCATION) != 0)
        perror("Error deleting file");
    else
        puts("File successfully deleted");
}
