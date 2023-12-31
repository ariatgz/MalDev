#include <windows.h>
 
SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hStatus;

VOID ServiceMain(DWORD argc, LPTSTR *argv) {
    ServiceStatus.dwServiceType = SERVICE_WIN32;
    ServiceStatus.dwCurrentState = SERVICE_RUNNING;
 
 
    if (hStatus == (SERVICE_STATUS_HANDLE)0) {
        return;
    }
 
    CreateProcess(NULL,
        "C:\\Windows\\System32\\calc.exe",
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        NULL,
        NULL
    );
 
    WaitForSingleObject(GetCurrentProcess(), INFINITE);
}
 
VOID ServiceCtrlHandler(DWORD control) {
    switch (control) {
        case SERVICE_CONTROL_STOP:
            ServiceStatus.dwCurrentState = SERVICE_STOPPED;
            SetServiceStatus(hStatus, &ServiceStatus);
            return;
        default:
            break;
    }
}
 
int main() {
    SERVICE_TABLE_ENTRY ServiceTable[] = {
        {NULL, (LPSERVICE_MAIN_FUNCTION)ServiceMain},
        {NULL, NULL}
    };
 
    StartServiceCtrlDispatcher(ServiceTable);
 
    return 0;
}
 
