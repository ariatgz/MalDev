#include <windows.h>;
#include <iostream>;
#include <string.h>;
#include <wininet.h>;
 
#pragma comment (lib, "wininet.lib")    
 
using namespace std;
 
bool downloadFile(const string& url, const string& filepath) {
    HINTERNET hSession = InternetOpen((LPCSTR)"Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hSession) {
        return false;
    }
     
    HINTERNET hHttpFile = InternetOpenUrl(hSession, (LPCSTR)url.c_str(), 0, 0, 0, 0);
    if (!hHttpFile) {
        InternetCloseHandle(hSession);
        return false;
    }
     
    HANDLE hFile = CreateFile(filepath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        InternetCloseHandle(hHttpFile);
        InternetCloseHandle(hSession);
        return false;
    }
     
    DWORD dwBytesRead;
    DWORD dwBytesWritten;
    const DWORD BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];
    bool success = true;
     
    while (InternetReadFile(hHttpFile, buffer, BUFFER_SIZE, &dwBytesRead) && dwBytesRead != 0) {
        if (!WriteFile(hFile, buffer, dwBytesRead, &dwBytesWritten, NULL) || dwBytesRead != dwBytesWritten) {
            success = false;
            break;
        }
    }
     
    CloseHandle(hFile);
    InternetCloseHandle(hHttpFile);
    InternetCloseHandle(hSession);
     
    return success;
}