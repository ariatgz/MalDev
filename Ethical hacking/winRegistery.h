#include <windows.h>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class WinRegistry{
    private:
        string name;
        string type;
        string registryPath;
    public:
        WinRegistry(string name, string type, string registryPath){
            this->name = name;
            this->type = type;
            this->registryPath = registryPath;
        }

        // Getters
        string getName(){
            return this->name;
        }
        string getType(){
            return this->type;
        }
        string getRegistryPath(){
            return this->registryPath;
        }

        // Setters
        void setName(string name){
            this->name = name;
        }
        void setType(string type){
            this->type = type;
        }
        void setRegistryPath(string registryPath){
            this->registryPath = registryPath;
        }

        //Create/Delete Registry
        void createRegistry() {
            HKEY hKey;
            LONG createStatus = RegCreateKeyExA(HKEY_CURRENT_USER, this->registryPath.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
            if (createStatus == ERROR_SUCCESS) {
                cout << "Registry created successfully" << endl;
            } else {
                cout << "Error creating registry" << endl;
            }
        }

        void deleteRegistry() { 
            LONG deleteStatus = RegDeleteKeyA(HKEY_CURRENT_USER, this->registryPath.c_str());
            if (deleteStatus == ERROR_SUCCESS) {
                cout << "Registry deleted successfully" << endl;
            } else {
                cout << "Error deleting registry" << endl;
            }
        }

        // Set Registry Value
        bool setRegistryValue(const std::string& valueName, const std::string& valueData) {
            HKEY hKey;
            LONG openStatus = RegOpenKeyEx(HKEY_CURRENT_USER, registryPath.c_str(), 0, KEY_SET_VALUE, &hKey);
            if (openStatus == ERROR_SUCCESS) {
                LONG setValueStatus = RegSetValueEx(hKey, valueName.c_str(), 0, REG_SZ, reinterpret_cast<const BYTE*>(valueData.c_str()), static_cast<DWORD>((valueData.length() + 1) * sizeof(char)));
                RegCloseKey(hKey);
                if (setValueStatus == ERROR_SUCCESS) {
                    std::cout << "Registry value set successfully" << std::endl;
                    return true;
                } else {
                    std::cerr << "Error setting registry value" << std::endl;
                    return false;
                }
            } else {
                std::cerr << "Error opening registry key" << std::endl;
                return false;
            }
        }

        // Get Registry Value
        bool getRegistryValue(const std::string& valueName, std::string& buffer, DWORD bufferSize) {
            HKEY hKey;
            LONG openStatus = RegOpenKeyEx(HKEY_CURRENT_USER, registryPath.c_str(), 0, KEY_QUERY_VALUE, &hKey);
            if (openStatus == ERROR_SUCCESS) {
                DWORD valueType;
                buffer.resize(bufferSize);
                LONG queryValueStatus = RegQueryValueEx(hKey, valueName.c_str(), NULL, &valueType, reinterpret_cast<BYTE*>(&buffer[0]), &bufferSize);
                RegCloseKey(hKey);
                if (queryValueStatus == ERROR_SUCCESS && valueType == REG_SZ) {
                    std::cout << "Registry value retrieved successfully: " << buffer << std::endl;
                    return true;
                } else {
                    std::cerr << "Error retrieving registry value" << std::endl;
                    return false;
                }
            } else {
                std::cerr << "Error opening registry key" << std::endl;
                return false;
            }
        }

        // Set Registry Value as binary
        bool setRegistryValueAsBinary(string& valueName,string& valueData){
            HKEY hKey;
            LONG openStatus = RegOpenKeyEx(HKEY_CURRENT_USER, registryPath.c_str(), 0, KEY_SET_VALUE, &hKey);
            if (openStatus == ERROR_SUCCESS) {
                LONG setValueStatus = RegSetValueEx(hKey, valueName.c_str(), 0, REG_BINARY, reinterpret_cast<const BYTE*>(valueData.c_str()), static_cast<DWORD>((valueData.length() + 1) * sizeof(char)));
                RegCloseKey(hKey);
                if (setValueStatus == ERROR_SUCCESS) {
                    std::cout << "Registry value set successfully" << std::endl;
                    return true;
                } else {
                    std::cerr << "Error setting registry value" << std::endl;
                    return false;
                }
            } else {
                std::cerr << "Error opening registry key" << std::endl;
                return false;
            }
        }

        // Get Registry Value as binary
        bool getRegistryValueAsBinary(string& valueName, string& buffer, DWORD bufferSize){
            HKEY hKey;
            LONG openStatus = RegOpenKeyEx(HKEY_CURRENT_USER, registryPath.c_str(), 0, KEY_QUERY_VALUE, &hKey);
            if (openStatus == ERROR_SUCCESS) {
                DWORD valueType;
                buffer.resize(bufferSize);
                LONG queryValueStatus = RegQueryValueEx(hKey, valueName.c_str(), NULL, &valueType, reinterpret_cast<BYTE*>(&buffer[0]), &bufferSize);
                RegCloseKey(hKey);
                if (queryValueStatus == ERROR_SUCCESS && valueType == REG_BINARY) {
                    std::cout << "Registry value retrieved successfully: " << buffer << std::endl;
                    return true;
                } else {
                    std::cerr << "Error retrieving registry value" << std::endl;
                    return false;
                }
            } else {
                std::cerr << "Error opening registry key" << std::endl;
                return false;
            }
        }

        // Set file as binary in registry
        bool setFileAsBinary(string srcFile) {
            HANDLE hFile = CreateFile(srcFile.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
            if (hFile == INVALID_HANDLE_VALUE) {
                std::cerr << "Error opening file: " << GetLastError() << std::endl;
                return false;
            }

            HKEY hKey;
            LONG openStatus = RegOpenKeyEx(HKEY_CURRENT_USER, registryPath.c_str(), 0, KEY_SET_VALUE, &hKey);
            if (openStatus != ERROR_SUCCESS) {
                std::cerr << "Error opening registry key: " << openStatus << std::endl;
                CloseHandle(hFile);
                return false;
            }

            // Read and write the file in chunks
            const DWORD CHUNK_SIZE = 1024; // Adjust as needed
            BYTE buffer[CHUNK_SIZE];
            DWORD bytesRead;

            while (ReadFile(hFile, buffer, CHUNK_SIZE, &bytesRead, NULL) && bytesRead > 0) {
                LSTATUS status = RegSetValueEx(hKey, "FileValue", 0, REG_BINARY, buffer, bytesRead);
                if (status != ERROR_SUCCESS) {
                    std::cerr << "Error setting registry value: " << status << std::endl;
                    CloseHandle(hFile);
                    RegCloseKey(hKey);
                    return false;
                }
            }
            std::cout << "File stored in the registry as binary data." << std::endl;
            CloseHandle(hFile);
            RegCloseKey(hKey);

            return true;
        }

        // Get file as binary from registry
        bool getFileAsBinary(string& valueName, string& buffer, DWORD bufferSize, string dtsFile){
            HKEY hKey;
            LONG openStatus = RegOpenKeyEx(HKEY_CURRENT_USER, registryPath.c_str(), 0, KEY_QUERY_VALUE, &hKey);
            if (openStatus == ERROR_SUCCESS) {
                DWORD valueType;
                buffer.resize(bufferSize);
                LONG queryValueStatus = RegQueryValueEx(hKey, valueName.c_str(), NULL, &valueType, reinterpret_cast<BYTE*>(&buffer[0]), &bufferSize);
                RegCloseKey(hKey);
                if (queryValueStatus == ERROR_SUCCESS && valueType == REG_BINARY) {
                    std::cout << "Registry value retrieved successfully: " << buffer << std::endl;
                    // Write buffer to file
                    ofstream outfile;
                    outfile.open(dtsFile.c_str(), ios::binary);
                    outfile.write(buffer.c_str(), buffer.size());
                    outfile.close();
                    return true;
                } else {
                    std::cerr << "Error retrieving registry value" << std::endl;
                    return false;
                }
            } else {
                std::cerr << "Error opening registry key" << std::endl;
                return false;
            }
        }

};