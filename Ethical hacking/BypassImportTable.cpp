#include <windows.h>
#include "lazy_importer.hpp"
 
int main(){
    PROCESS_INFORMATION pi;
    _STARTUPINFOW si = { sizeof(si) };
    LI_FN(CreateProcessW)(L"C:\\Windows\\System32\\notepad.exe", nullptr, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi);
    WaitForSingleObject(pi.hProcess, INFINITE);
     
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}