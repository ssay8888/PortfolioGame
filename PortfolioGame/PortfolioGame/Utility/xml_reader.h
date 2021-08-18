#pragma once
#include <iostream>
#include <pugixml.hpp>
#include <string>
#include <vector>
#include <list>
class MonsterParts;
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

	// 스킨관련메소드
	SkinFrame* FindCanvas(std::string type, pugi::xml_node node, int32_t size);
	std::vector<std::string> LoadCharecterSkin(const int32_t count);

	std::list<std::pair<std::string, std::string>> LoadSmap();
	std::list<std::string> LoadZmap();

	const std::wstring StringToWString(const char* buffer) const;

	//아이템메소드
	std::vector<std::string> LoadCharacterItem(std::string type,  const int32_t code);


	///몬스터관련 메소드


	void LoadMonsters();
	void SetInfoMonster(pugi::xpath_node_set node, std::shared_ptr<Monster> monster);
	void CanvasMonster(pugi::xml_node node, std::shared_ptr<MonsterParts> parts, std::vector<std::shared_ptr<MonsterParts>>& list);

	//스킬관련 메소드
	void SkillLoad();

	//포탈관련 메소드
	void LoadPortal();

	//아이템 메소드
	void LoadItem(std::string path);
	void LoadSpecialItem(std::string path);
	void LoadDropData();

	//엔피시 메소드
	void LoadNpc();
	void LoadQuest();
	void LoadShop();


	//스트링 메소드
	void LoadItemString(std::string path, const std::string node_path);
};

