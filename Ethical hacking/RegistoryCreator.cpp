#include <windows.h>
#include <string>
#include <iostream>
#include "winRegistery.h"
 
using namespace std;
 
int main(){
    string registryPath = "SOFTWARE\\MyCompany\\MyApp\\C2DomainName"; // Replace this with your desired path
    string valueName = "Malicious Domain Server"; // Replace this with your desired value name
    string valueData = "https://maliciousDomain.com"; // Replace this with your desired string data
 
    // Create the WinRegistry object
    WinRegistry winReg("","",registryPath);
 
    // Create the registry
    winReg.createRegistry();
 
    // Set the string value in the registry
    if (winReg.setRegisteryValue(valueName, valueData)) {
        cout << "String value added to the registry successfully." << endl;
    } else {
        cerr << "Failed to set the string value in the registry." << endl;
    }
 
    getchar();
 
    return 0;
}