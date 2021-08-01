#pragma once
#include <string>
#include <list>
class FileManager
{
    FileManager() = default;
    ~FileManager() = default;
public:
    std::string ReadFile(std::wstring path) const;
    int GetDirFileCount(std::wstring);
    std::list<std::wstring> GetDirFileName(std::wstring);
    static FileManager* GetInstance() {
        static FileManager* instance = new FileManager;
        return instance;
    }
};

