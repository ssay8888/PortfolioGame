#include "../pch.h"
#include <sstream>
#include <fstream> 

#include "xml_reader.h"
#include "../Managers/Skins/skin_info.h"
#include "../Managers/Skins/skin_item.h"
#include "../Managers/Skins/skin_frame.h"
#include "../Managers/Skins/skin_parts.h"
#include "../Managers/Skins/skin_manager.h"
#include "../Managers/MonsterMnager/monster_manager.h"
#include "../Managers/MonsterMnager/monster_movement.h"
#include "../Managers/MonsterMnager/monster_parts.h"
#include "../Components/MapObject/monster.h"
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

					std::shared_ptr<Monster*> monster = std::make_shared<Monster*>(new Monster());
					(*monster)->SetMonsterCode(path);
					SetInfoMonster(data, monster);

					data = doc.select_nodes("imgdir/imgdir");
					std::shared_ptr<MonsterMovement*> movement = std::make_shared<MonsterMovement*>(new MonsterMovement());
					for (auto nodes : data)
					{
						if (strcmp(nodes.node().attribute("name").value(), "info"))
						{
							(*monster)->SetMovement(movement);
							std::vector<std::shared_ptr<MonsterParts*>>  list;
							for (auto node : nodes.node())
							{
								auto parts = std::make_shared<MonsterParts*>(new MonsterParts());

								if (!strcmp(node.name(), "canvas"))
								{
									std::cout << nodes.node().attribute("name").value() << std::endl;
									std::cout << node.attribute("name").value() << std::endl;
									// <imgdir name="move">
									std::string path;
									char_t imagePath[100];
									path.append(nodes.node().attribute("name").value()).append(".").append(node.attribute("name").value())
										.append(".bmp");
									//Client\Mob\0100100.img.xml
									snprintf(imagePath, 100, "%s\\%s", StringTools::WStringToString(xmlPath.substr(0, xmlPath.size() - 4).c_str()).c_str(), path.c_str());
									if ((*parts)->GetImage() == nullptr) {
										std::shared_ptr<MyBitmap*> bitmap = std::make_shared<MyBitmap*>(new MyBitmap());
										(*bitmap)->Insert_Bitmap(_hWnd, StringTools::StringToWString(imagePath).c_str());
										(*parts)->SetImage(bitmap);
									}

									for (auto canvans : node) // canvas info
									{
										std::cout << nodes.node().attribute("name").value() << std::endl;
										if (!strcmp(canvans.attribute("name").value(), "origin"))
										{
											(*parts)->SetOriginPosX(std::stoi(canvans.attribute("x").value()));
											(*parts)->SetOriginPosY(std::stoi(canvans.attribute("y").value()));
										}
										else if (!strcmp(canvans.attribute("name").value(), "It"))
										{
											(*parts)->SetRectLeft(std::stoi(canvans.attribute("x").value()));
											(*parts)->SetRectRight(std::stoi(canvans.attribute("y").value()));
										}
										else if (!strcmp(canvans.attribute("name").value(), "rb"))
										{
											(*parts)->SetRectRight(std::stoi(canvans.attribute("x").value()));
											(*parts)->SetRectBottom(std::stoi(canvans.attribute("y").value()));
										}
										else if (!strcmp(canvans.attribute("name").value(), "head"))
										{
											(*parts)->SetHeadPosX(std::stoi(canvans.attribute("x").value()));
											(*parts)->SetHeadPosY(std::stoi(canvans.attribute("y").value()));
										}
										else if (!strcmp(canvans.attribute("name").value(), "delay"))
										{
											(*parts)->SetDelay(std::stoi(canvans.attribute("value").value()));
										}
									}
									list.push_back(parts);
								}
							}
							(*movement)->InsertMovement(nodes.node().attribute("name").value(), list);
						}
					}


					MonsterManager::GetInstance()->InsertMonster((*monster)->GetMonsteCode(), monster);
				}
			}
		}
	}
}

void XmlReader::SetInfoMonster(pugi::xpath_node_set data, std::shared_ptr<Monster*> monster)
{

	for (auto info : data)
	{

		if (!strcmp(info.node().attribute("name").value(), "level"))
		{
			(*monster)->SetLevel(std::stoi(info.node().attribute("value").value()));
		}
		else if (!strcmp(info.node().attribute("name").value(), "bodyAttack"))
		{
			(*monster)->SetBodyAttack(std::stoi(info.node().attribute("value").value()));
		}
		else if (!strcmp(info.node().attribute("name").value(), "maxHP"))
		{
			(*monster)->SetMaxHp(std::stoi(info.node().attribute("value").value()));
			(*monster)->SetHp(std::stoi(info.node().attribute("value").value()));
		}
		else if (!strcmp(info.node().attribute("name").value(), "maxMP"))
		{
			(*monster)->SetMaxMp(std::stoi(info.node().attribute("value").value()));
			(*monster)->SetMp(std::stoi(info.node().attribute("value").value()));
		}
		else if (!strcmp(info.node().attribute("name").value(), "speed"))
		{
			(*monster)->SetSpeed(std::abs(std::stof(info.node().attribute("value").value())) / 100);
		}
		else if (!strcmp(info.node().attribute("name").value(), "PADamage"))
		{
			(*monster)->SetPad(std::stoi(info.node().attribute("value").value()));
		}
		else if (!strcmp(info.node().attribute("name").value(), "PDDamage"))
		{
			(*monster)->SetPdd(std::stoi(info.node().attribute("value").value()));
		}
		else if (!strcmp(info.node().attribute("name").value(), "MADamage"))
		{
			(*monster)->SetMad(std::stoi(info.node().attribute("value").value()));
		}
		else if (!strcmp(info.node().attribute("name").value(), "MDDamage"))
		{
			(*monster)->SetMdd(std::stoi(info.node().attribute("value").value()));
		}
		else if (!strcmp(info.node().attribute("name").value(), "undead"))
		{
			(*monster)->SetUndead(std::stoi(info.node().attribute("value").value()));
		}
		else if (!strcmp(info.node().attribute("name").value(), "exp"))
		{
			(*monster)->SetExp(std::stoi(info.node().attribute("value").value()));
		}
	}
}
