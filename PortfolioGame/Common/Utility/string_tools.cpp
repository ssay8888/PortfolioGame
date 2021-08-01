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
