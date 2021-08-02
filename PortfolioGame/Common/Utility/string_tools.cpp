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
	std::wstring wstr;
	std::string str = buffer;
	wstr.assign(str.begin(), str.end());
	return wstr.c_str();
}

const std::string StringTools::WStringToString(const wchar_t* buffer)
{
	std::wstring wstr = buffer;
	std::string str;
	str.assign(wstr.begin(), wstr.end());
	return str.c_str();
}
