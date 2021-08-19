#include "../pch.h"
#include <fstream> 
#include <sstream>

#include "xml_reader.h"
#include "../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../Common/Utility/file_manager.h"
#include "../Components/Base/game_object.h"
#include "../Components/MapObject/ani_map_object.h"
#include "../Components/MapObject/Item/item.h"
#include "../Components/MapObject/Npc/npc.h"
#include "../Components/MapObject/Monster/monster.h"
#include "../Components/MapObject/Monster/AttackInfo/attack_info.h"
#include "../Managers/ItemManager/item_manager.h"
#include "../Managers/DropDataManager/drop_data_manager.h"
#include "../Managers/DropDataManager/drop_data_info.h"
#include "../Managers/MapManager/map_manager.h"
#include "../Managers/MonsterMnager/monster_manager.h"
#include "../Managers/MonsterMnager/monster_movement.h"
#include "../Managers/MonsterMnager/monster_parts.h"
#include "../Managers/SkillManager/skill_manager.h"
#include "../Managers/SkillManager/Skill/skill.h"
#include "../Managers/SkillManager/Skill/skill_effect_image.h"
#include "../Managers/SkillManager/Skill/skill_info.h"
#include "../Managers/Skins/skin_frame.h"
#include "../Managers/Skins/skin_info.h"
#include "../Managers/Skins/skin_item.h"
#include "../Managers/Skins/skin_manager.h"
#include "../Managers/Skins/skin_parts.h"
#include "../Managers/ShopManager/shop_item.h"
#include "../Managers/StringManager/string_Info.h"
#include "../Managers/StringManager/string_manager.h"
#include "../Managers/NpcManager/npc_manager.h"
#include "../Managers/QuestManager/quest_manager.h"
#include "../Managers/QuestManager/Quest/quest_info.h"
#include "../Managers/QuestManager/Quest/SubInfo/quest_say.h"
#include "../Managers/QuestManager/Quest/SubInfo/quest_check.h"
#include "../Managers/QuestManager/Quest/SubInfo/quest_reward.h"
#include "../Managers/ShopManager/shop_manager.h"

using namespace pugi;
SkinFrame* XmlReader::FindCanvas(std::string type, xml_node node, int32_t size)
{
	SkinFrame* frame = new SkinFrame();
	try
	{
		frame->SetFrame(node.attribute("name").value());
		if (!strcmp(node.name(), "canvas"))
		{
			SkinParts* parts = new SkinParts();
			std::wstring fileName;
			fileName.append(StringToWString(node.parent().attribute("name").value())).append(L".").
				append(StringToWString(node.attribute("name").value())).append(L".bmp");
			wchar_t fullPath[256];
			swprintf_s(fullPath, L"Client\\%s\\%08d.img\\%s", StringTools::StringToWString(type.c_str()).c_str(), size, fileName.c_str());
			MyBitmap* bitmap = new MyBitmap();
			bitmap->Insert_Bitmap(_hWnd, fullPath);
			parts->SetBitmap(bitmap);
			parts->SetName(node.attribute("name").value());
			frame->InsertParts(parts->GetName(), parts);
			for (auto canvasInfo : node)
			{
				if (!strcmp(canvasInfo.attribute("name").value(), "map"))
				{
					for (auto map : canvasInfo)
					{
						ObjectPos pos{ std::stof(map.attribute("x").value()), std::stof(map.attribute("y").value()) };
						parts->InsertMap(map.attribute("name").value(), pos);
					}
				}
				else if (!strcmp(canvasInfo.attribute("name").value(), "origin"))
				{
					parts->SetOriginX(std::stof(canvasInfo.attribute("x").value()));
					parts->SetOriginY(std::stof(canvasInfo.attribute("y").value()));
				}
				else if (!strcmp(canvasInfo.attribute("name").value(), "z"))
				{
					parts->SetZ(canvasInfo.attribute("value").value());
				}
			}
			frame->InsertParts(parts->GetName(), parts);
		}
	}
	catch (std::exception&)
	{

	}
	return frame;
}

std::vector<std::string> XmlReader::LoadCharecterSkin(int32_t size)
{
	pugi::xml_document doc;
	//L"Client\\Character\\00012000.img.xml"
	char_t xmlPath[100];
	snprintf(xmlPath, 100, "Client\\Character\\%08d.img.xml", size);
	auto err = doc.load_file(xmlPath);

	std::vector<std::string> list;
	if (err.status == status_ok)
	{
		auto data = doc.select_nodes("imgdir/imgdir");
		SkinInfo* info = new SkinInfo();
		info->SetName(std::to_string(size));
		info->SetItemId(size);
		for (auto begin : data) // alert ..
		{
			auto itemName = begin.node().attribute("name").value();
			SkinItem* item = new SkinItem();
			item->SetName(itemName);
			item->SetPartner(info);
			for (auto frameCount : begin.node()) // alert\0~n
			{
				if (!strcmp(frameCount.name(), "canvas"))
				{
					SkinFrame* frame = FindCanvas("Character\\", frameCount, size);
					frame->SetFrame(frameCount.attribute("name").value());
					item->InsertFrame(frame);
				}
				else
				{
					SkinFrame* frame = new SkinFrame();
					try
					{
						frame->SetPartner(item);
						frame->SetFrame(frameCount.attribute("name").value());
						for (auto canvas : frameCount)
						{
							if (!strcmp(canvas.name(), "canvas"))
							{
								SkinParts* parts = new SkinParts();
								std::wstring fileName;
								fileName.append(StringToWString(itemName)).append(L".")
									.append(StringToWString(frameCount.attribute("name").value())).append(L".").
									append(StringToWString(canvas.attribute("name").value())).append(L".bmp");
								wchar_t fullPath[256];
								swprintf_s(fullPath, L"Client\\Character\\000%05d.img\\%s", size, fileName.c_str());
								MyBitmap* bitmap = new MyBitmap();
								bitmap->Insert_Bitmap(_hWnd, fullPath);
								parts->SetBitmap(bitmap);
								parts->SetName(canvas.attribute("name").value());
								parts->SetPartner(frame);
								frame->InsertParts(parts->GetName(), parts);

								for (auto canvasInfo : canvas)
								{
									if (!strcmp(canvasInfo.attribute("name").value(), "map"))
									{
										for (auto map : canvasInfo)
										{
											ObjectPos pos{ std::stof(map.attribute("x").value()), std::stof(map.attribute("y").value()) };
											parts->InsertMap(map.attribute("name").value(), pos);
										}
									}
									else if (!strcmp(canvasInfo.attribute("name").value(), "origin"))
									{
										parts->SetOriginX(std::stof(canvasInfo.attribute("x").value()));
										parts->SetOriginY(std::stof(canvasInfo.attribute("y").value()));
									}
									else if (!strcmp(canvasInfo.attribute("name").value(), "z"))
									{
										parts->SetZ(canvasInfo.attribute("value").value());
									}
								}
								frame->InsertParts(parts->GetName(), parts);
							}
							else if (!strcmp(canvas.name(), "uol"))
							{
								SkinParts* parts = new SkinParts();
								parts->SetPartner(frame);
								parts->SetUol(canvas.attribute("value").value());
								parts->SetName(canvas.attribute("name").value());
								frame->InsertParts(parts->GetName(), parts);
							}
							else if (!strcmp(canvas.name(), "string"))
							{
								if (!strcmp(canvas.attribute("name").value(), "action"))
								{
									frame->SetAction(canvas.attribute("value").value());
								}
							}
							else if (!strcmp(canvas.name(), "int"))
							{
								if (!strcmp(canvas.attribute("name").value(), "delay"))
								{
									frame->SetDelay(static_cast<uint16_t>(std::stoi(canvas.attribute("value").value())));
								}
								else if (!strcmp(canvas.attribute("name").value(), "frame"))
								{
									frame->SetActionFrame(static_cast<uint16_t>(std::stoi(canvas.attribute("value").value())));
								}
							}
						}
					}
					catch (std::exception&)
					{

					}
					item->InsertFrame(frame);
				}
			}
			if (size > 10000)
			{
				info->InsertHeadSkinItem(item);
			}
			else 
			{
				info->InsertBodySkinItem(item);
			}
		}
		SkinManager::GetInstance()->InsertBodySkin(info);
	}
	return list;
}

std::list<std::pair<std::string, std::string>> XmlReader::LoadSmap()
{
	pugi::xml_document doc;
	auto err = doc.load_file(L"Client\\Character\\Base\\smap.img.xml");
	std::list<std::pair<std::string, std::string>> list;

	auto data = doc.select_nodes("imgdir/string");
	std::for_each(data.begin(), data.end(),
		[&list](xpath_node item)
		{
			list.emplace_back(std::make_pair(item.node().attribute("name").value(), item.node().attribute("value").value()));
		});

	return list;
}

std::list<std::string> XmlReader::LoadZmap()
{
	std::list<std::string> list;
	pugi::xml_document doc;
	auto err = doc.load_file(L"Client\\Character\\Base\\zmap.img.xml");

	auto data = doc.select_nodes("imgdir/null");
	std::for_each(data.begin(), data.end(),
		[&list](xpath_node item)
		{
			list.emplace_front(item.node().attribute("name").value());
		});
	return list;
}

const std::wstring XmlReader::StringToWString(const char* buffer) const
{
	std::wstring wstr;
	std::string str = buffer;
	wstr.assign(str.begin(), str.end());
	return wstr.c_str();
}

std::vector<std::string> XmlReader::LoadCharacterItem(std::string type, const int32_t code)
{
	pugi::xml_document doc;
	//L"Client\\Character\\00012000.img.xml"
	char_t xmlPath[100];
	snprintf(xmlPath, 100, "Client\\%s\\%08d.img.xml", type.c_str(), code);
	auto err = doc.load_file(xmlPath);

	std::vector<std::string> list;
	if (err.status == status_ok)
	{
		auto data = doc.select_nodes("imgdir/imgdir");
		SkinInfo* info = new SkinInfo();
		info->SetName(std::to_string(code));
		info->SetItemId(code);
		for (auto begin : data) // alert ..
		{
			auto itemName = begin.node().attribute("name").value();
			SkinItem* item = new SkinItem();
			item->SetName(itemName);
			item->SetPartner(info);
			if (!strcmp(begin.node().attribute("name").value(), "info"))
			{
				for (auto info_node : begin.node())
				{
					if (!strcmp(info_node.attribute("name").value(), "reqLevel"))
					{
						info->GetItemInfo().SetReqLevel(std::stoi(info_node.attribute("value").value()));
					}
					else if (!strcmp(info_node.attribute("name").value(), "reqSTR"))
					{
						info->GetItemInfo().SetReqStr(std::stoi(info_node.attribute("value").value()));
					}
					else if (!strcmp(info_node.attribute("name").value(), "reqDEX"))
					{
						info->GetItemInfo().SetReqDex(std::stoi(info_node.attribute("value").value()));
					}
					else if (!strcmp(info_node.attribute("name").value(), "reqINT"))
					{
						info->GetItemInfo().SetReqInt(std::stoi(info_node.attribute("value").value()));
					}
					else if (!strcmp(info_node.attribute("name").value(), "reqLUK"))
					{
						info->GetItemInfo().SetReqLuk(std::stoi(info_node.attribute("value").value()));
					}
					else if (!strcmp(info_node.attribute("name").value(), "incSTR"))
					{
						info->GetItemInfo().SetIncStr(std::stoi(info_node.attribute("value").value()));
					}
					else if (!strcmp(info_node.attribute("name").value(), "incDEX"))
					{
						info->GetItemInfo().SetIncDex(std::stoi(info_node.attribute("value").value()));
					}
					else if (!strcmp(info_node.attribute("name").value(), "incINT"))
					{
						info->GetItemInfo().SetIncInt(std::stoi(info_node.attribute("value").value()));
					}
					else if (!strcmp(info_node.attribute("name").value(), "incLUK"))
					{
						info->GetItemInfo().SetIncLuk(std::stoi(info_node.attribute("value").value()));
					}
					else if (!strcmp(info_node.attribute("name").value(), "incPAD"))
					{
						info->GetItemInfo().SetIncPad(std::stoi(info_node.attribute("value").value()));
					}
					else if (!strcmp(info_node.attribute("name").value(), "incMAD"))
					{
						info->GetItemInfo().SetIncMad(std::stoi(info_node.attribute("value").value()));
					}
					else if (!strcmp(info_node.attribute("name").value(), "incPDD"))
					{
						info->GetItemInfo().SetIncPdd(std::stoi(info_node.attribute("value").value()));
					}
					else if (!strcmp(info_node.attribute("name").value(), "incMDD"))
					{
						info->GetItemInfo().SetIncMdd(std::stoi(info_node.attribute("value").value()));
					}
					else if (!strcmp(info_node.attribute("name").value(), "incMHP"))
					{
						info->GetItemInfo().SetIncMhp(std::stoi(info_node.attribute("value").value()));
					}
					else if (!strcmp(info_node.attribute("name").value(), "incMMP"))
					{
						info->GetItemInfo().SetIncMmp(std::stoi(info_node.attribute("value").value()));
					}
					else if (!strcmp(info_node.attribute("name").value(), "price"))
					{
						info->GetItemInfo().SetPrice(std::stoi(info_node.attribute("value").value()));
					}
					else if (!strcmp(info_node.attribute("name").value(), "tuc"))
					{
						info->GetItemInfo().SetTuc(std::stoi(info_node.attribute("value").value()));
					}
				}
				auto data = begin.node().select_node("canvas[@name='icon']");
				if (data)
				{
					auto icon = std::make_shared<MyBitmap>(MyBitmap());
					std::wstring bmp_path;
					wchar_t bmpPath[100];
					swprintf(bmpPath, 100, L"Client\\%s\\%08d.img\\info.icon.bmp",
						StringTools::StringToWString(type.c_str()).c_str(), code);
					icon->Insert_Bitmap(_hWnd, bmpPath);
					info->SetIcon(icon);

					data = begin.node().select_node("canvas[@name='iconRaw']");
					auto icon2 = std::make_shared<MyBitmap>(MyBitmap());
					swprintf(bmpPath, 100, L"Client\\%s\\%08d.img\\info.iconRaw.bmp",
						StringTools::StringToWString(type.c_str()).c_str(), code);
					icon2->Insert_Bitmap(_hWnd, bmpPath);
					info->SetIconRaw(icon2);
				}
				continue;
			}
			for (auto frameCount : begin.node()) // alert\0~n
			{
				if (!strcmp(frameCount.name(), "canvas"))
				{
					SkinFrame* frame = FindCanvas(type, frameCount, code);
					frame->SetFrame(frameCount.attribute("name").value());
					item->InsertFrame(frame);
				}
				else
				{
					SkinFrame* frame = new SkinFrame();
					try
					{
						frame->SetPartner(item);
						frame->SetFrame(frameCount.attribute("name").value());
						for (auto canvas : frameCount)
						{
							if (!strcmp(canvas.name(), "canvas"))
							{
								SkinParts* parts = new SkinParts();
								std::wstring fileName;
								fileName.append(StringToWString(itemName)).append(L".")
									.append(StringToWString(frameCount.attribute("name").value())).append(L".").
									append(StringToWString(canvas.attribute("name").value())).append(L".bmp");
								wchar_t fullPath[256];
								swprintf_s(fullPath, L"Client\\%s\\%08d.img\\%s", StringTools::StringToWString(type.c_str()).c_str(), code, fileName.c_str());
								MyBitmap* bitmap = new MyBitmap();
								bitmap->Insert_Bitmap(_hWnd, fullPath);
								parts->SetBitmap(bitmap);
								parts->SetName(canvas.attribute("name").value());
								parts->SetPartner(frame);
								frame->InsertParts(parts->GetName(), parts);

								for (auto canvasInfo : canvas)
								{
									if (!strcmp(canvasInfo.attribute("name").value(), "map"))
									{
										for (auto map : canvasInfo)
										{
											ObjectPos pos{ std::stof(map.attribute("x").value()), std::stof(map.attribute("y").value()) };
											parts->InsertMap(map.attribute("name").value(), pos);
										}
									}
									else if (!strcmp(canvasInfo.attribute("name").value(), "origin"))
									{
										parts->SetOriginX(std::stof(canvasInfo.attribute("x").value()));
										parts->SetOriginY(std::stof(canvasInfo.attribute("y").value()));
									}
									else if (!strcmp(canvasInfo.attribute("name").value(), "z"))
									{
										parts->SetZ(canvasInfo.attribute("value").value());
									}
								}
								frame->InsertParts(parts->GetName(), parts);
							}
							else if (!strcmp(canvas.name(), "uol"))
							{
								SkinParts* parts = new SkinParts();
								parts->SetPartner(frame);
								parts->SetUol(canvas.attribute("value").value());
								parts->SetName(canvas.attribute("name").value());
								frame->InsertParts(parts->GetName(), parts);
							}
							else if (!strcmp(canvas.name(), "string"))
							{
								if (!strcmp(canvas.attribute("name").value(), "action"))
								{
									frame->SetAction(canvas.attribute("value").value());
								}
							}
							else if (!strcmp(canvas.name(), "int"))
							{
								if (!strcmp(canvas.attribute("name").value(), "delay"))
								{
									frame->SetDelay(static_cast<uint16_t>(std::stoi(canvas.attribute("value").value())));
								}
								else if (!strcmp(canvas.attribute("name").value(), "frame"))
								{
									frame->SetActionFrame(static_cast<uint16_t>(std::stoi(canvas.attribute("value").value())));
								}
							}
						}
					}
					catch (std::exception&)
					{

					}
					item->InsertFrame(frame);
				}
			}
			info->InsertBodySkinItem(item);
		}
		SkinManager::GetInstance()->InsertBodySkin(info);
	}

	return list;
}

void XmlReader::LoadMonsters()
{
	pugi::xml_document doc;

	auto files = FileManager::GetInstance()->GetDirFileName(L"Client\\Mob\\");

	for (auto xmlPath : files)
	{
		auto path = StringTools::WStringToString(xmlPath.c_str());
		if (xmlPath.find(L".xml") != std::wstring::npos)
		{
			if (!_access(path.c_str(), 0))
			{
				auto err = doc.load_file(StringTools::WStringToString(xmlPath.c_str()).c_str());

				if (err.status == status_ok)
				{
					std::string monster_id(doc.select_node("imgdir").node().attribute("name").value());
					auto data = doc.select_nodes("imgdir/imgdir[@name='info']/*");

					std::shared_ptr<Monster> monster = std::make_shared<Monster>(Monster());
					monster->SetMonsterCode(path);
					SetInfoMonster(data, monster);
					monster->SetMonsterId(std::stoi(monster_id.substr(0, 7)));

					data = doc.select_nodes("imgdir/imgdir");
					std::shared_ptr<MonsterMovement> movement = std::make_shared<MonsterMovement>(MonsterMovement());
					for (auto nodes : data)
					{
						if (strcmp(nodes.node().attribute("name").value(), "info"))
						{
							monster->SetMovement(movement);
							std::vector<std::shared_ptr<MonsterParts>>  list;
							for (auto node : nodes.node())
							{
								auto parts = std::make_shared<MonsterParts>(MonsterParts());

								if (!strcmp(node.name(), "canvas"))
								{
									//std::cout << nodes.node().attribute("name").value() << std::endl;
									//std::cout << node.attribute("name").value() << std::endl;
									// <imgdir name="move">
									std::string path;
									char_t imagePath[100];
									path.append(nodes.node().attribute("name").value()).append(".").append(node.attribute("name").value())
										.append(".bmp");
									//Client\Mob\0100100.img.xml
									snprintf(imagePath, 100, "%s\\%s", StringTools::WStringToString(xmlPath.substr(0, xmlPath.size() - 4).c_str()).c_str(), path.c_str());
									if (parts->GetImage() == nullptr) {
										std::shared_ptr<MyBitmap> bitmap = std::make_shared<MyBitmap>(MyBitmap());
										bitmap->Insert_Bitmap(_hWnd, StringTools::StringToWString(imagePath).c_str());
										parts->SetImage(bitmap);
									}
									CanvasMonster(node, parts, list);
									list.push_back(parts);
								}
								else if (!strcmp(node.attribute("name").value(), "info"))
								{
									auto attack_info = new AttackInfo();
									for (auto info_node : node)
									{
										if (!strcmp(info_node.attribute("name").value(), "attackAfter"))
										{
											attack_info->SetAttackAfter(std::stoi(info_node.attribute("value").value()));
										}
										else if (!strcmp(info_node.attribute("name").value(), "conMP"))
										{
											attack_info->SetMpCon(std::stoi(info_node.attribute("value").value()));
										}
										else if (!strcmp(info_node.attribute("name").value(), "deadlyAttack"))
										{
											attack_info->SetDeadlyAttack(std::stoi(info_node.attribute("value").value()));
										}
										else if (!strcmp(info_node.attribute("name").value(), "effectAfter"))
										{
											attack_info->SetEffectAfter(std::stoi(info_node.attribute("value").value()));
										}
										else if (!strcmp(info_node.attribute("name").value(), "effect") || 
											!strcmp(info_node.attribute("name").value(), "hit") ||
											!strcmp(info_node.attribute("name").value(), "areaWarning"))
										{
											std::vector<std::shared_ptr<MonsterParts>>  list2;
											for (auto effect_node : info_node)
											{
												auto parts2 = std::make_shared<MonsterParts>(MonsterParts());
												if(!strcmp(effect_node.name(), "canvas"))
												{
													CanvasMonster(effect_node, parts2, list2);

													std::string path;
													char_t imagePath[100];
													path.append(nodes.node().attribute("name").value()).append(".")
														.append(node.attribute("name").value()).append(".").append(info_node.attribute("name").value())
														.append(".").append(effect_node.attribute("name").value()).append(".bmp");
													snprintf(imagePath, 100, "%s\\%s", StringTools::WStringToString(xmlPath.substr(0, xmlPath.size() - 4).c_str()).c_str(), path.c_str());

													if (parts2->GetImage() == nullptr)
													{
														std::shared_ptr<MyBitmap> bitmap2 = std::make_shared<MyBitmap>(MyBitmap());
														bitmap2->Insert_Bitmap(_hWnd, StringTools::StringToWString(imagePath).c_str());
														parts2->SetImage(bitmap2);
													}
													list2.emplace_back(parts2);
												}
												else if (!strcmp(effect_node.name(), "uol"))
												{
													auto parts2 = std::make_shared<MonsterParts>(MonsterParts());
													parts2->SetUol(effect_node.attribute("value").value());
													list2.emplace_back(parts2);
												}
											}
											if (!strcmp(info_node.attribute("name").value(), "effect"))
											{
												attack_info->SetEffect(list2);
											}
											else if (!strcmp(info_node.attribute("name").value(), "hit"))
											{
												attack_info->SetHitEffect(list2);
											}
											else if (!strcmp(info_node.attribute("name").value(), "areaWarning"))
											{
												attack_info->SetAreaWarning(list2);
											}
										}
										else if (!strcmp(info_node.attribute("name").value(), "range"))
										{
											RECT ragne{};
											for (auto range_node : info_node)
											{
												if (!strcmp(range_node.attribute("name").value(), "lt"))
												{
													ragne.left = std::stol(range_node.attribute("x").value());
													ragne.top = std::stol(range_node.attribute("y").value());
												}
												else if (!strcmp(range_node.attribute("name").value(), "rb"))
												{
													ragne.right = std::stol(range_node.attribute("x").value());
													ragne.bottom = std::stol(range_node.attribute("y").value());
												}
											}
											attack_info->SetRange(ragne);
										}
										else if (!strcmp(info_node.attribute("name").value(), "type"))
										{
											attack_info->SetType(std::stoi(info_node.attribute("value").value()));
										}
									}

									monster->InsertAttackInfo(nodes.node().attribute("name").value(), attack_info);
								}
								if (!strcmp(node.name(), "uol"))
								{
									parts->SetUol(node.attribute("value").value());
									list.push_back(parts);
								}
								parts->SetPartner(movement);
							}
							movement->InsertMovement(nodes.node().attribute("name").value(), list);
						}
					}


					MonsterManager::GetInstance()->InsertMonster(monster->GetMonsteCode(), monster);
				}
			}
		}
	}
}

void XmlReader::SetInfoMonster(pugi::xpath_node_set data, std::shared_ptr<Monster> monster)
{

	for (auto info : data)
	{

		if (!strcmp(info.node().attribute("name").value(), "level"))
		{
			monster->SetLevel(std::stoi(info.node().attribute("value").value()));
		}
		else if (!strcmp(info.node().attribute("name").value(), "bodyAttack"))
		{
			monster->SetBodyAttack(std::stoi(info.node().attribute("value").value()));
		}
		else if (!strcmp(info.node().attribute("name").value(), "maxHP"))
		{
			monster->SetMaxHp(std::stoi(info.node().attribute("value").value()));
			monster->SetHp(std::stoi(info.node().attribute("value").value()));
		}
		else if (!strcmp(info.node().attribute("name").value(), "maxMP"))
		{
			monster->SetMaxMp(std::stoi(info.node().attribute("value").value()));
			monster->SetMp(std::stoi(info.node().attribute("value").value()));
		}
		else if (!strcmp(info.node().attribute("name").value(), "speed"))
		{
			monster->SetSpeed(std::abs(std::stof(info.node().attribute("value").value())) / 65);
		}
		else if (!strcmp(info.node().attribute("name").value(), "PADamage"))
		{
			monster->SetPad(std::stoi(info.node().attribute("value").value()));
		}
		else if (!strcmp(info.node().attribute("name").value(), "PDDamage"))
		{
			monster->SetPdd(std::stoi(info.node().attribute("value").value()));
		}
		else if (!strcmp(info.node().attribute("name").value(), "MADamage"))
		{
			monster->SetMad(std::stoi(info.node().attribute("value").value()));
		}
		else if (!strcmp(info.node().attribute("name").value(), "MDDamage"))
		{
			monster->SetMdd(std::stoi(info.node().attribute("value").value()));
		}
		else if (!strcmp(info.node().attribute("name").value(), "undead"))
		{
			monster->SetUndead(std::stoi(info.node().attribute("value").value()));
		}
		else if (!strcmp(info.node().attribute("name").value(), "exp"))
		{
			monster->SetExp(std::stoi(info.node().attribute("value").value()));
		}
		else if (!strcmp(info.node().attribute("name").value(), "boss"))
		{
			monster->SetBoss(std::stoi(info.node().attribute("value").value()));
		}
	}
}

void XmlReader::CanvasMonster(pugi::xml_node node, std::shared_ptr<MonsterParts> parts, std::vector<std::shared_ptr<MonsterParts>>& list)
{

	for (auto canvans : node) // canvas info
	{
		//std::cout << nodes.node().attribute("name").value() << std::endl;
		if (!strcmp(canvans.attribute("name").value(), "origin"))
		{
			parts->SetOriginPosX(std::stoi(canvans.attribute("x").value()));
			parts->SetOriginPosY(std::stoi(canvans.attribute("y").value()));
		}
		else if (!strcmp(canvans.attribute("name").value(), "lt"))
		{
			parts->SetRectLeft(std::stoi(canvans.attribute("x").value()));
			parts->SetRectTop(std::stoi(canvans.attribute("y").value()));
		}
		else if (!strcmp(canvans.attribute("name").value(), "rb"))
		{
			parts->SetRectRight(std::stoi(canvans.attribute("x").value()));
			parts->SetRectBottom(std::stoi(canvans.attribute("y").value()));
		}
		else if (!strcmp(canvans.attribute("name").value(), "head"))
		{
			parts->SetHeadPosX(std::stoi(canvans.attribute("x").value()));
			parts->SetHeadPosY(std::stoi(canvans.attribute("y").value()));
		}
		else if (!strcmp(canvans.attribute("name").value(), "delay"))
		{
			parts->SetDelay(std::stoi(canvans.attribute("value").value()));
		}
	}
}

void XmlReader::SkillLoad()
{
	pugi::xml_document doc;
	const auto foldersPath = FileManager::GetInstance()->GetDirFileName(L"Client\\Skill\\");
	for (auto folderPath : foldersPath)
	{
		const auto err = doc.load_file(StringTools::WStringToString(folderPath.c_str()).c_str());

		if (err.status == status_ok)
		{
			const auto datas = doc.select_nodes("imgdir/imgdir/imgdir");

			for (auto skills_node : datas) // skillid
			{
				const auto skill = new Skill();
				skill->SetSkillId(std::stoi(skills_node.node().attribute("name").value()));


				for (auto skill_info : skills_node.node())
				{
					if (!strcmp(skill_info.attribute("name").value(), "action"))
					{
						skill->SetAction(skill_info.children().begin()->attribute("value").value());
					}
					else if (!strcmp(skill_info.attribute("name").value(), "level"))
					{
						for (auto& skill_level_node : skill_info)
						{
							auto skill_info = new SkillInfo();
							for (auto& skill_level_info_node : skill_level_node)
							{
								if (!strcmp(skill_level_info_node.attribute("name").value(), "x"))
								{
									skill_info->SetX(std::stoi(skill_level_info_node.attribute("value").value()));
								}
								else if (!strcmp(skill_level_info_node.attribute("name").value(), "y"))
								{
									skill_info->SetY(std::stoi(skill_level_info_node.attribute("value").value()));
								}
								else if (!strcmp(skill_level_info_node.attribute("name").value(), "time"))
								{
									skill_info->SetTime(std::stoi(skill_level_info_node.attribute("value").value()));
								}
								else if (!strcmp(skill_level_info_node.attribute("name").value(), "time"))
								{
									skill_info->SetTime(std::stoi(skill_level_info_node.attribute("value").value()));
								}
								else if (!strcmp(skill_level_info_node.attribute("name").value(), "pdd"))
								{
									skill_info->SetPdd(std::stoi(skill_level_info_node.attribute("value").value()));
								}
								else if (!strcmp(skill_level_info_node.attribute("name").value(), "attackCount"))
								{
									skill_info->SetAttackCount(std::stoi(skill_level_info_node.attribute("value").value()));
								}
								else if (!strcmp(skill_level_info_node.attribute("name").value(), "mobCount"))
								{
									skill_info->SetMobCount(std::stoi(skill_level_info_node.attribute("value").value()));
								}
								else if (!strcmp(skill_level_info_node.attribute("name").value(), "mpCon"))
								{
									skill_info->SetMpCon(std::stoi(skill_level_info_node.attribute("value").value()));
								}
								else if (!strcmp(skill_level_info_node.attribute("name").value(), "lt"))
								{
									skill_info->SetRectLt(std::stol(skill_level_info_node.attribute("x").value()), std::stol(skill_level_info_node.attribute("y").value()));
								}
								else if (!strcmp(skill_level_info_node.attribute("name").value(), "rb"))
								{
									skill_info->SetRectRb(std::stol(skill_level_info_node.attribute("x").value()), std::stol(skill_level_info_node.attribute("y").value()));
								}
							}
							skill->InsertSkillInfo(skill_info);

						}
					}
					else if (!strcmp(skill_info.attribute("name").value(), "effect") || !strcmp(skill_info.attribute("name").value(), "ball"))
					{
						for (auto& skill_effect_node : skill_info)
						{
							wchar_t path[150];
							swprintf_s(path, 150, L"Client\\Skill\\%d.img\\skill.%d.%s.%s.bmp",
								(skill->GetSkillId() / 10000),
								skill->GetSkillId(),
								StringTools::StringToWString(skill_info.attribute("name").value()).c_str(),
								StringTools::StringToWString(skill_effect_node.attribute("name").value()).c_str());
							const auto skill_effect = new SkillEffectImage();
							for (auto& skill_effect_info : skill_effect_node)
							{
								if (!strcmp(skill_effect_info.attribute("name").value(), "delay"))
								{
									skill_effect->SetDelay(std::stoi(skill_effect_info.attribute("value").value()));
								}
								else if (!strcmp(skill_effect_info.attribute("name").value(), "origin"))
								{
									skill_effect->SetOrigin(
										{ std::stof(skill_effect_info.attribute("x").value()) ,
											std::stof(skill_effect_info.attribute("y").value()) });
								}
							}
							const auto image = new MyBitmap();
							image->Insert_Bitmap(_hWnd, path);
							skill_effect->SetImage(image);
							if (!strcmp(skill_info.attribute("name").value(), "effect"))
							{
								skill->InsertSkillEffect(skill_effect);
							}
							else
							{
								skill->InsertBallEffect(skill_effect);
							}
						}

					}
					else if (!strcmp(skill_info.attribute("name").value(), "hit"))
					{
						for (auto skill_hits_node : skill_info)
						{
							for (auto& skill_effect_node : skill_hits_node)
							{
								wchar_t path[150];
								swprintf_s(path, 150, L"Client\\Skill\\%d.img\\skill.%d.hit.%s.%s.bmp",
									(skill->GetSkillId() / 10000),
									skill->GetSkillId(),
									StringTools::StringToWString(skill_hits_node.attribute("name").value()).c_str(),
									StringTools::StringToWString(skill_effect_node.attribute("name").value()).c_str());
								const auto skill_effect = new SkillEffectImage();
								for (auto& skill_effect_info : skill_effect_node)
								{
									if (!strcmp(skill_effect_info.attribute("name").value(), "delay"))
									{
										skill_effect->SetDelay(std::stoi(skill_effect_info.attribute("value").value()));
									}
									else if (!strcmp(skill_effect_info.attribute("name").value(), "origin"))
									{
										skill_effect->SetOrigin(
											{ std::stof(skill_effect_info.attribute("x").value()) ,
												std::stof(skill_effect_info.attribute("y").value()) });
									}
								}
								const auto image = new MyBitmap();
								image->Insert_Bitmap(_hWnd, path);
								skill_effect->SetImage(image);
								skill->InsertHitEffect(skill_effect);
							}
						}
					}
				}
				skill->ReadySkill();
				SkillManager::GetInstance()->InsertSkill(skill->GetSkillId(), skill);
			}
		}
	}
}

void XmlReader::LoadPortal()
{

	pugi::xml_document doc;
	auto err = doc.load_file("Client\\Map\\Portal.xml");

	std::vector<std::string> list;
	if (err.status == status_ok)
	{
		auto datas = doc.select_nodes("imgdir/imgdir/canvas");

		AniMapObject* aniObject = new AniMapObject();
		for (auto canvas : datas)
		{
			char path[150];
			sprintf_s(path, 150, "Client\\Map\\pv.%s.bmp", canvas.node().attribute("name").value());
			MyBitmap* image = new MyBitmap();
			image->Insert_Bitmap(_hWnd, StringTools::StringToWString(path).c_str());
			for(auto vector : canvas.node())
			{
				ObjectPos pos{ std::stof(vector.attribute("x").value()) , std::stof(vector.attribute("y").value()) };
				aniObject->InsertAni(pos, image);
			}
		}
		MapManager::GetInstance()->InsertObjectImage("portal", aniObject);
	}
}

void XmlReader::LoadItem(std::string path)
{
	pugi::xml_document doc;
	char buff[255];
	snprintf(buff, 255, "Client\\Item\\%s.xml", path.c_str()); // Consume\\0200.img.xml
	const auto err = doc.load_file(buff);

	if (err.status == status_ok)
	{
		auto datas = doc.select_nodes("imgdir/imgdir");

		for (auto item_code : datas)
		{
			int32_t item_id = std::stoi(item_code.node().attribute("name").value());
			std::shared_ptr<Item> item_shared = std::make_shared<Item>(Item());
			auto item = (item_shared);
			item->SetItemId(item_id);
			auto info_node = item_code.node().select_node("imgdir[@name='info']");

			if (info_node)
			{
				for (auto info : info_node.node())
				{
					if (!strcmp(info.attribute("name").value(), "icon") || !strcmp(info.attribute("name").value(), "iconRaw"))
					{
						if (!strcmp(info.name(), "canvas"))
						{
							char bmp_path[150];
							snprintf(bmp_path, 150, "Client\\Item\\%s\\%08d.info.%s.bmp",
								path.c_str(),
								item_id,
								info.attribute("name").value());
							auto icon = std::make_shared<MyBitmap>(MyBitmap());
							icon->Insert_Bitmap(_hWnd, StringTools::StringToWString(bmp_path).c_str());
							if (!strcmp(info.attribute("name").value(), "icon"))
							{
								item->SetIcon(icon);
							}
							else
							{
								item->InsertIconRaw(icon);
							}
						}

					}
					else if (!strcmp(info.attribute("name").value(), "price"))
					{
						item->SetPrice(std::stoi(info.attribute("value").value()));
					}
					else if (!strcmp(info.attribute("name").value(), "slotMax"))
					{
						item->SetSlotMax(std::stoi(info.attribute("value").value()));
					}
					else if (!strcmp(info.attribute("name").value(), "incMAD"))
					{
						item->SetIncMad(std::stoi(info.attribute("value").value()));
					}
					else if (!strcmp(info.attribute("name").value(), "incINT"))
					{
						item->SetIncInt(std::stoi(info.attribute("value").value()));
					}
					else if (!strcmp(info.attribute("name").value(), "success"))
					{
						item->SetSuccess(std::stoi(info.attribute("value").value()));
					}
				}
			}

			auto spec_node = item_code.node().select_node("imgdir[@name='spec']");
			if (spec_node)
			{
				for (auto spec : spec_node.node())
				{
					item->InsertSpec(spec.attribute("name").value(), std::stoi(spec.attribute("value").value()));
				}
			}
			ItemManager::GetInstance()->InsertItem(item_id, item_shared);
		}
	}
}

void XmlReader::LoadSpecialItem(std::string path)
{
	pugi::xml_document doc;
	char buff[255];
	snprintf(buff, 255, "Client\\Item\\%s.xml", path.c_str()); // Consume\\0200.img.xml
	const auto err = doc.load_file(buff);

	if (err.status == status_ok)
	{
		auto datas = doc.select_nodes("imgdir/imgdir");

		for (auto item_code : datas)
		{
			int32_t item_id = std::stoi(item_code.node().attribute("name").value());
			std::shared_ptr<Item> item = std::make_shared<Item>(Item());
			for (auto iconRaw : item_code.node())
			{
				for (auto canvas : iconRaw)
				{
					char bmp_path[150];
					snprintf(bmp_path, 150, "Client\\Item\\%s\\%08d.iconRaw.%s.bmp",
						path.c_str(),
						item_id,
						canvas.attribute("name").value());
					auto icon = std::make_shared<MyBitmap>(MyBitmap());
					icon->Insert_Bitmap(_hWnd, StringTools::StringToWString(bmp_path).c_str());

					item->SetItemId(item_id);
					item->InsertIconRaw(icon);
				}
			}
			ItemManager::GetInstance()->InsertItem(item_id, item);
		}
	}
}

void XmlReader::LoadDropData()
{
	pugi::xml_document doc;
	const auto err = doc.load_file("DropData\\dropdata.xml");


	if (err.status == status_ok)
	{
		auto datas = doc.select_nodes("imgdir/imgdir");
		for (auto monster_id_node : datas)
		{
			int32_t monster_id = std::stoi(monster_id_node.node().attribute("name").value());
			std::list<std::shared_ptr<DropDataInfo>> list;
			for (auto info_node : monster_id_node.node())
			{
				std::shared_ptr<DropDataInfo> info = std::make_shared<DropDataInfo>(DropDataInfo());

				info->SetItemId(std::stoi(info_node.attribute("item").value()));
				info->SetRate(std::stoi(info_node.attribute("rate").value()));
				info->SetMinPrice(std::stoi(info_node.attribute("minprice").value()));
				info->SetMaxPrice(std::stoi(info_node.attribute("maxprice").value()));
				list.emplace_back(info);
			}
			DropDataManager::GetInstance()->InsertDropData(monster_id, list);
		}
	}
}

void XmlReader::LoadNpc()
{
	pugi::xml_document doc;

	auto files = FileManager::GetInstance()->GetDirFileName(L"Client\\Npc\\");

	for (auto xmlPath : files)
	{
		auto path = StringTools::WStringToString(xmlPath.c_str());
		if (xmlPath.find(L".xml") != std::wstring::npos)
		{
			if (!_access(path.c_str(), 0))
			{
				auto err = doc.load_file(StringTools::WStringToString(xmlPath.c_str()).c_str());

				if (err.status == status_ok)
				{
					auto stand_node = doc.select_node("imgdir/imgdir[@name='stand']");

					std::shared_ptr<Npc> npc = std::make_shared<Npc>(Npc());
					npc->SetNpcPath(StringTools::WStringToString(xmlPath.c_str()));
					for (auto canvas_name : stand_node.node())
					{
						std::shared_ptr<MyBitmap> image = std::make_shared<MyBitmap>(MyBitmap());
						wchar_t image_path[128];
						swprintf_s(image_path, 128, L"Client\\Npc\\%s\\%s.%s.bmp", 
							StringTools::StringToWString(canvas_name.parent().parent().attribute("name").value()).c_str(),
							StringTools::StringToWString(canvas_name.parent().attribute("name").value()).c_str(),
							StringTools::StringToWString(canvas_name.attribute("name").value()).c_str());
						image->Insert_Bitmap(_hWnd, image_path);
						npc->InsertImage(image);
						std::string str(canvas_name.parent().parent().attribute("name").value());

						str = str.substr(0, 7);
						npc->SetNpcId(std::stoi(str));
						for (auto canvas_info : canvas_name)
						{
							if (!strcmp(canvas_info.attribute("name").value(), "origin"))
							{
								npc->SetOriginX(std::stoi(canvas_info.attribute("x").value()));
								npc->SetOriginX(std::stoi(canvas_info.attribute("y").value()));
							}
						}
					}
					NpcManager::GetInstance()->InsertNpc(npc->GetNpcPath(), npc);
				}
			}
		}
	}
}

void XmlReader::LoadQuest()
{
	pugi::xml_document doc;
	const auto err = doc.load_file("Client\\Quest\\QuestInfo.xml");


	if (err.status == status_ok)
	{
		auto datas = doc.select_nodes("quest/quest");
		std::shared_ptr<QuestInfo> quest_info = std::make_shared<QuestInfo>(QuestInfo());
		for (auto npcid_node: datas)
		{
			quest_info->SetNpcId(std::stoi(npcid_node.node().attribute("name").value()));
			for (auto info_node : npcid_node.node())
			{
				if (!strcmp(info_node.attribute("name").value(), "say"))
				{
					for (auto say_node : info_node)
					{
						std::shared_ptr<QuestSay> say = std::make_shared<QuestSay>(QuestSay());
						say->SetId(std::stoi(say_node.attribute("name").value()));
						say->SetSay(say_node.attribute("value").value());
						say->SetType(static_cast<ObjectType::NpcTalkType>(std::stoi(say_node.attribute("type").value())));
						quest_info->InsertSay(say);
					}
				}
				else if (!strcmp(info_node.attribute("name").value(), "success"))
				{
					for (auto say_node : info_node)
					{
						std::shared_ptr<QuestSay> say = std::make_shared<QuestSay>(QuestSay());
						say->SetId(std::stoi(say_node.attribute("name").value()));
						say->SetSay(say_node.attribute("value").value());
						say->SetType(static_cast<ObjectType::NpcTalkType>(std::stoi(say_node.attribute("type").value())));
						quest_info->InsertSuccess(say);
					}
				}
				else if (!strcmp(info_node.attribute("name").value(), "ing"))
				{
					for (auto say_node : info_node)
					{
						std::shared_ptr<QuestSay> say = std::make_shared<QuestSay>(QuestSay());
						say->SetId(std::stoi(say_node.attribute("name").value()));
						say->SetSay(say_node.attribute("value").value());
						say->SetType(static_cast<ObjectType::NpcTalkType>(std::stoi(say_node.attribute("type").value())));
						quest_info->InsertIngSay(say);
					}
				}
				else if (!strcmp(info_node.attribute("name").value(), "check"))
				{
					for (auto check_node : info_node)
					{
						std::shared_ptr<QuestCheck> check = std::make_shared<QuestCheck>(QuestCheck());
						check->SetItemId(std::stoi(check_node.attribute("item").value()));
						check->SetPrice(std::stoi(check_node.attribute("price").value()));
						quest_info->InsertCheck(check);
					}
				}
				else if (!strcmp(info_node.attribute("name").value(), "reward"))
				{
					for (auto check_node : info_node)
					{
						std::shared_ptr<QuestReward> reward = std::make_shared<QuestReward>(QuestReward());
						reward->SetRewardType(check_node.attribute("name").value());
						reward->SetItemId(std::stoi(check_node.attribute("item").value()));
						reward->SetValue(std::stoi(check_node.attribute("value").value()));
						quest_info->InsertReward(reward);
					}
				}
			}
			QuestManager::GetInstance()->InsertQuestInfo(quest_info);
		}
	}
}

void XmlReader::LoadShop()
{
	pugi::xml_document doc;
	const auto err = doc.load_file("ShopData\\ShopData.xml");


	if (err.status == status_ok)
	{
		auto datas = doc.select_nodes("shop/shop");
		for (auto shop_id_node : datas)
		{
			int32_t shop_id = std::stoi(shop_id_node.node().attribute("name").value());
			std::vector<std::shared_ptr<ShopItem>> list;
			for (auto info_node : shop_id_node.node())
			{
				std::shared_ptr<ShopItem> info = std::make_shared<ShopItem>(ShopItem());

				info->SetItemId(std::stoi(info_node.attribute("item").value()));
				info->SetPrice(std::stoi(info_node.attribute("price").value()));
				info->SetMeso(std::stoi(info_node.attribute("meso").value()));
				list.emplace_back(info);
			}
			ShopManager::GetInstance()->InsertShop(shop_id, list);
		}
	}
}

void XmlReader::LoadItemString(const std::string path, const std::string node_path)
{
	pugi::xml_document doc;
	char buff[255];
	snprintf(buff, 255, "Client\\String\\%s", path.c_str()); // Etc.img.xml
	const auto err = doc.load_file(buff);

	if (err.status == status_ok)
	{
		auto datas = doc.select_nodes(node_path.c_str());
		for (auto item_id_node : datas)
		{
			StringInfo str_info;
			str_info.SetItemId(std::stoi(item_id_node.node().attribute("name").value()));
			for (auto info_node : item_id_node.node())
			{
				if (!strcmp(info_node.attribute("name").value(), "name"))
				{
					str_info.SetName(StringTools::StringToWString(info_node.attribute("value").value()));
				}
				else if(!strcmp(info_node.attribute("name").value(), "desc"))
				{
					str_info.SetDesc(StringTools::StringToWString(info_node.attribute("value").value()));
				}
			}
			StringManager::GetInstance()->InsertStringInfo(str_info);
		}
	}
}
