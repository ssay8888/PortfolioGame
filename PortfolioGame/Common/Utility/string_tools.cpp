#include "../pch.h"
#include "string_tools.h"

std::vector<std::string> StringTools::SplitString(const std::string text, const char delimiter)
{
	std::vector<std::string> answer;
	std::stringstream ss(text);
	std::string temp;

	while (getline(ss, temp, delimiter)) {
		answer.push_back(temp);
	}

	return answer;
}

const std::wstring StringTools::StringToWString(const char* buffer)
{
	const size_t cSize = strlen(buffer) + 1;
	size_t cn;
	wchar_t* wc = new wchar_t[cSize];
	setlocale(LC_ALL, "Korean");
	mbstowcs_s(&cn, wc, cSize, buffer, cSize);
	return wc;
}

const std::string StringTools::WStringToString(const wchar_t* buffer)
{
	std::wstring wstr = buffer;
	std::string str;
	str.assign(wstr.begin(), wstr.end());
	return str.c_str();
}

uint16_t StringTools::FindOverlapCount(std::string text, std::string mod)
{
	size_t start_pos = 0;
	uint16_t count = 0;

	while ((start_pos = text.find(mod, start_pos)) != std::string::npos)
	{
		text.replace(start_pos, mod.length(), "");
		count++;
	}
	return count;
}

void StringTools::CreateTextOut(HDC hdc, int x, int y, std::wstring str, int fontSize, COLORREF color, std::wstring font)
{
	SetBkMode(hdc, TRANSPARENT);
	auto hFont = CreateFont(fontSize, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, font.c_str());
	auto OldFont = (HFONT)SelectObject(hdc, hFont);
	SetTextColor(hdc, color);
	TextOut(hdc, x, y, str.c_str(), lstrlen(str.c_str()));
	SelectObject(hdc, OldFont);
	DeleteObject(hFont);
	DeleteObject(OldFont);
}

uint16_t StringTools::ReplaceAll(std::string& text, std::string mod)
{
	size_t start_pos = 0;
	uint16_t count = 0;

	while ((start_pos = text.find(mod, start_pos)) != std::string::npos)
	{
		text.replace(start_pos, mod.length(), "");
		count++;
	}
	return count;
}
