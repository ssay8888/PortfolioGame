#pragma once
#include <iostream>
#include <pugixml.hpp>
#include <string>
class SkinFrame;
class SkinItem;
class XmlReader
{
public:
	static XmlReader& GetInstance()
	{
		static XmlReader instance;
		return instance;
	}
	std::vector<std::string> LoadCharecterSkin(const std::wstring& path);
	const std::wstring StringToWString(const char* buffer) const;
	void SetCanvasInfo(pugi::xml_node_iterator attributes, std::string number, SkinFrame* item);
	void SetMapInfo(pugi::xml_node_iterator begin, pugi::xml_node_iterator end, SkinFrame* item);
	void HasChild(pugi::xml_node_iterator node, pugi::xml_node_iterator end, std::string number, SkinFrame* item);
};

