#include <windows.h>
#include <iostream>
#include <tlhelp32.h>
#include "PrivilegeEsclation.h"
 
using namespace std;
 
int main(){
    PrivEscalationClass privEsc;
    bool success = privEsc.runProcAsAdminFromUser("C:\\Windows\\System32\\cmd.exe");
    if(success){
        cout << "Successfully ran process as admin\n";
    } else {
        cout << "Failed to run process as admin\n";
    }
    return 0;
}