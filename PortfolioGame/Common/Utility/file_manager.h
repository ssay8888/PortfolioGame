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

    std::wstring GetFileName(std::wstring path);
    std::string GetFileName(std::string path);

    static FileManager* GetInstance() {
        static FileManager* instance = new FileManager;
        return instance;
    }
};
