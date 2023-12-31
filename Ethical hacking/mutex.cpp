#include <iostream>
#include <windows.h>
 
#define MUTEX_NAME "Global\\IPC_MUTEX"
 
using namespace std;
 
bool run(){
    HANDLE hMutex = CreateMutexA(NULL, FALSE, MUTEX_NAME);
    MessageBoxA(NULL, "Payload", "Payload1", MB_OK);
    if (hMutex == NULL) {
        cerr << "Error creating Mutex" << endl;
        return false;
    }
    return true;
}
 
bool itsAlreadyRunning(){
    HANDLE hMutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, MUTEX_NAME);
    if (hMutex == NULL) {
        cerr << "Mutex dont exist" << endl;
        return false;
    }
    return true;
}
 
int main(){
    if (itsAlreadyRunning()){
        cerr << "Already running" << endl;
        getchar();
        return 1;
    }
    else{   
        run();
    }
    getchar();
    return 0;
}
