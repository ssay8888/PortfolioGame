#pragma once
#include <vector>
#include <string>
#include <sstream>

namespace StringTools
{
    std::vector<std::string> SplitString(const std::string text, const char delimiter);
    const std::wstring StringToWString(const char* buffer);
    const std::string WStringToString(const wchar_t* buffer);
}
