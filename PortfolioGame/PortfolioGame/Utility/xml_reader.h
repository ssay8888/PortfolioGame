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

	// ��Ų���ø޼ҵ�
	SkinFrame* FindCanvas(std::string type, pugi::xml_node node, int32_t size);
	std::vector<std::string> LoadCharecterSkin(const int32_t count);

	std::list<std::pair<std::string, std::string>> LoadSmap();
	std::list<std::string> LoadZmap();

	const std::wstring StringToWString(const char* buffer) const;

	//�����۸޼ҵ�
	std::vector<std::string> LoadCharacterItem(std::string type,  const int32_t code);


	///���Ͱ��� �޼ҵ�


	void LoadMonsters();
	void SetInfoMonster(pugi::xpath_node_set node, std::shared_ptr<Monster> monster);
	void CanvasMonster(pugi::xml_node node, std::shared_ptr<MonsterParts> parts, std::vector<std::shared_ptr<MonsterParts>>& list);

	//��ų���� �޼ҵ�
	void SkillLoad();

	//��Ż���� �޼ҵ�
	void LoadPortal();

	//������ �޼ҵ�
	void LoadItem(std::string path);
	void LoadSpecialItem(std::string path);
	void LoadDropData();

	//���ǽ� �޼ҵ�
	void LoadNpc();
	void LoadQuest();
	void LoadShop();


	//��Ʈ�� �޼ҵ�
	void LoadItemString(std::string path, const std::string node_path);
};

