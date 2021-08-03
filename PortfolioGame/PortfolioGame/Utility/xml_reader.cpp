#include "../pch.h"
#include <sstream>
#include <fstream> 

#include "xml_reader.h"
#include "../Managers/Skins/skin_info.h"
#include "../Managers/Skins/skin_item.h"
#include "../Managers/Skins/skin_frame.h"
#include "../Managers/Skins/skin_parts.h"
#include "../Managers/Skins/skin_manager.h"
#include "../../Common/Managers/BitmapManager/my_bitmap.h"

using namespace pugi;
SkinFrame* XmlReader::FindCanvas(xml_node node, int32_t size)
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
			swprintf_s(fullPath, L"Client\\Character\\000%05d.img\\%s", size, fileName.c_str());
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
						POINT pos{ std::stol(map.attribute("x").value()), std::stol(map.attribute("y").value()) };
						parts->InsertMap(map.attribute("name").value(), pos);
					}
				}
				else if (!strcmp(canvasInfo.attribute("name").value(), "origin"))
				{
					parts->SetOriginX(std::stol(canvasInfo.attribute("x").value()));
					parts->SetOriginY(std::stol(canvasInfo.attribute("y").value()));
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
	snprintf(xmlPath, 100, "Client\\Character\\000%05d.img.xml", size);
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
					SkinFrame* frame = FindCanvas(frameCount, size);
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
											POINT pos{ std::stol(map.attribute("x").value()), std::stol(map.attribute("y").value()) };
											parts->InsertMap(map.attribute("name").value(), pos);
										}
									}
									else if (!strcmp(canvasInfo.attribute("name").value(), "origin"))
									{
										parts->SetOriginX(std::stol(canvasInfo.attribute("x").value()));
										parts->SetOriginY(std::stol(canvasInfo.attribute("y").value()));
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
									frame->SetDelay(static_cast<uint16_t>(std::stoi(canvas.attribute("delay").value())));
								}
								else if (!strcmp(canvas.attribute("name").value(), "frame"))
								{
									frame->SetActionFrame(static_cast<uint16_t>(std::stoi(canvas.attribute("delay").value())));
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

void XmlReader::SetCanvasInfo(pugi::xml_node_iterator node, std::string nummber, SkinFrame* item)
{
}

void XmlReader::SetMapInfo(pugi::xml_node_iterator itr, pugi::xml_node_iterator end, SkinFrame* item)
{
}

void XmlReader::HasChild(pugi::xml_node_iterator begin, pugi::xml_node_iterator end, std::string nummber, SkinFrame* item)
{
	for (; begin != end; ++begin)
	{
		SetCanvasInfo(begin, nummber, item);
	}
}
