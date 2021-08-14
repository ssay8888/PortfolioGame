#include "../pch.h"
#include <sstream>
#include <fstream> 

#include "xml_reader.h"
#include "../Components/Base/game_object.h"
#include "../Components/MapObject/Item/item.h"
#include "../Managers/MapManager/map_manager.h"
#include "../Managers/Skins/skin_info.h"
#include "../Managers/Skins/skin_item.h"
#include "../Managers/Skins/skin_frame.h"
#include "../Managers/Skins/skin_parts.h"
#include "../Managers/Skins/skin_manager.h"
#include "../Managers/MonsterMnager/monster_manager.h"
#include "../Managers/MonsterMnager/monster_movement.h"
#include "../Managers/MonsterMnager/monster_parts.h"
#include "../Managers/SkillManager/skill_manager.h"
#include "../Managers/SkillManager/Skill/skill.h"
#include "../Managers/SkillManager/Skill/skill_effect_image.h"
#include "../Managers/SkillManager/Skill/skill_info.h"
#include "../Managers/ItemManager/item_manager.h"
#include "../Components/MapObject/Monster/monster.h"
#include "../Components/MapObject/Monster/AttackInfo/attack_info.h"
#include "../Components/MapObject/ani_map_object.h"
#include "../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../Common/Utility/file_manager.h"

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
		for (auto begin : data) // alert ..
		{
			auto itemName = begin.node().attribute("name").value();
			SkinItem* item = new SkinItem();
			item->SetName(itemName);
			item->SetPartner(info);
			if (!strcmp(begin.node().attribute("name").value(), "info"))
			{
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
					auto data = doc.select_nodes("imgdir/imgdir[@name='info']/*");

					std::shared_ptr<Monster> monster = std::make_shared<Monster>(Monster());
					monster->SetMonsterCode(path);
					SetInfoMonster(data, monster);

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
												std::cout << range_node.attribute("name").value() << std::endl;
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
			monster->SetSpeed(std::abs(std::stof(info.node().attribute("value").value())) / 100);
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

			auto info_node = item_code.node().select_node("imgdir[@name='info']");

			if (info_node)
			{
				for (auto info : info_node.node())
				{
					if (!strcmp(info.attribute("name").value(), "icon") || !strcmp(info.attribute("name").value(), "iconRaw"))
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
							item->SetIconRaw(icon);
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
