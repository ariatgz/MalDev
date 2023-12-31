#include <windows.h>
#include <iostream>
#include <tlhelp32.h>
 
int getPIDbyProcName(const char* procName) {
    int pid = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hSnap, &pe32) != FALSE) {
        while (pid == 0 && Process32Next(hSnap, &pe32) != FALSE) {
            if (strcmp(pe32.szExeFile, procName) == 0) {
                pid = pe32.th32ProcessID;
            }
        }
    }
    CloseHandle(hSnap);
    return pid;
}
 
int main() {
    const char* exePath = "C:\\Windows\\System32\\notepad.exe";
    DWORD parentPID = getPIDbyProcName("mspaint.exe");
 
    STARTUPINFOEX info = { sizeof(STARTUPINFOEX) }; // Changed sizeof(0) to sizeof(STARTUPINFOEX)
    PROCESS_INFORMATION processInfo; 
    SIZE_T cbAttributeListSize = 0;
    PPROC_THREAD_ATTRIBUTE_LIST pAttributeList = NULL; 
    
    HANDLE hParentProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, parentPID); // Open the parent process with all access
 
    InitializeProcThreadAttributeList(NULL, 1, 0, &cbAttributeListSize); // Get the size of the attribute list
    pAttributeList = (PPROC_THREAD_ATTRIBUTE_LIST)HeapAlloc(GetProcessHeap(), 0, cbAttributeListSize);  // Allocate memory for the attribute list
    InitializeProcThreadAttributeList(pAttributeList, 1, 0, &cbAttributeListSize); // Initialize the attribute list
  
    UpdateProcThreadAttribute(pAttributeList, 0, PROC_THREAD_ATTRIBUTE_PARENT_PROCESS, &hParentProcess, sizeof(HANDLE), NULL, NULL); // Update the attribute list with the parent process
 
    info.lpAttributeList = pAttributeList; // Set the attribute list in the startup info
 
    CreateProcess(NULL, (LPSTR)exePath, NULL, NULL, TRUE, EXTENDED_STARTUPINFO_PRESENT | CREATE_NO_WINDOW, NULL, NULL, reinterpret_cast<STARTUPINFO*>(&info), &processInfo); 
 
    printf("Process created with PID: %d\n", processInfo.dwProcessId);
    printf("Parent PID: %d\n", parentPID);
    return 0;
}