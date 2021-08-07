#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <Windows.h>
namespace StringTools
{
    std::vector<std::string> SplitString(const std::string text, const char delimiter);
    const std::wstring StringToWString(const char* buffer);
    const std::string WStringToString(const wchar_t* buffer);
    uint16_t ReplaceAll(std::string& text, std::string mod);
    uint16_t FindOverlapCount(std::string text, std::string mod);

    void CreateTextOut(HDC hdc, int x, int y, std::wstring str, int fontSize = 11, COLORREF color = RGB(255, 255, 255), std::wstring font = L"±¼¸²Ã¼");
}
