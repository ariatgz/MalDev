#include <windows.h>
#include <stdio.h>
#include "skCrypter.h"
 
using namespace std;
 
int main(int argc, char *argv[]){
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
 
    CreateProcess(skCrypt("C:\\Windows\\System32\\calc.exe"),NULL, NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);
    return 0;
}