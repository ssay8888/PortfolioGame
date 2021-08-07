#pragma once
#include <iostream>
#include <pugixml.hpp>
#include <string>
#include <vector>
#include <list>
class SkinFrame;
class SkinItem;
class Monster;
class XmlReader
{
public:
	static XmlReader& GetInstance()
	{
		static XmlReader instance;
		return instance;
	}

	// ��Ų���ø޼ҵ�
	SkinFrame* FindCanvas(pugi::xml_node node, int32_t size);
	std::vector<std::string> LoadCharecterSkin(const int32_t count);

	std::list<std::pair<std::string, std::string>> LoadSmap();
	std::list<std::string> LoadZmap();

	const std::wstring StringToWString(const char* buffer) const;


	///���Ͱ��� �޼ҵ�


	void LoadMonsters();
	void SetInfoMonster(pugi::xpath_node_set node, std::shared_ptr<Monster*> monster);

};

