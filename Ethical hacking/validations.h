#include <iostream>
 
using namespace std;
 
// Function to check if a process with the specified name is running.
bool IsProcessRunning(const string& processName) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // Take a snapshot of all running processes
    if (hSnapshot == INVALID_HANDLE_VALUE)
        return false;
 
    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);
 
    if (Process32First(hSnapshot, &processEntry)) { // Start iterating through the processes in the snapshot
        do {
            if (processName.compare(processEntry.szExeFile) == 0) { // Compare the process name with the target name
                CloseHandle(hSnapshot); // Close the handle to the snapshot
                return true; // Return true if the process name matches the target name
            }
        } while (Process32Next(hSnapshot, &processEntry)); // Continue iterating through the processes
    }
 
    CloseHandle(hSnapshot); // Close the handle to the snapshot if the process is not found
    return false; // Return false if the process is not found
}
 
// Function to check if a DLL with the specified name is loaded in a given process.
bool IsDLLLoaded(DWORD processId, const std::wstring& dllName) {
    HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId); // Take a snapshot of the modules loaded in the target process
    if (hModuleSnap == INVALID_HANDLE_VALUE) {
        return false; // Return false if the snapshot creation fails
    }
 
    MODULEENTRY32W moduleEntry;
    moduleEntry.dwSize = sizeof(MODULEENTRY32W);
 
    bool dllFound = false;
    if (Module32FirstW(hModuleSnap, &moduleEntry)) { // Start iterating through the modules in the snapshot
        do {
            if (dllName.compare(moduleEntry.szModule) == 0) { // Compare the DLL name with the target name
                dllFound = true; // Set the flag to true if the DLL name matches the target name
                break; // Exit the loop as we have found the DLL
            }
        } while (Module32NextW(hModuleSnap, &moduleEntry)); // Continue iterating through the modules
    }
    CloseHandle(hModuleSnap); // Close the handle to the snapshot
    return dllFound; // Return true if the DLL is found, otherwise, return false
}