#include "../pch.h"
#include <sstream>
#include <fstream> 

#include "base64.h"
#include "xml_reader.h"
#include "../Managers/Skins/skin_info.h"
#include "../Managers/Skins/skin_item.h"
#include "../Managers/Skins/skin_frame.h"
#include "../Managers/Skins/skin_manager.h"

using namespace pugi;
std::vector<std::string> XmlReader::LoadCharecterSkin(const std::wstring& path)
{
	pugi::xml_document doc;
	//int id= 2000;
	//char buffer[150];
	//snprintf(buffer, 150, "Character\\000%05d.img.xml", id);
	//xml_path.append("000").append(std::to_string(id));
	auto err = doc.load_file(path.c_str());
	std::vector<std::string> list;


	if (err.status == status_ok)
	{

		auto data = doc.select_nodes("imgdir/imgdir");
		for (auto first = data.begin(); first != data.end(); ++first)
		{
			for (auto begin = first->node().begin(); begin != first->node().end(); ++begin)
			{
				SkinItem* item = new SkinItem();
				std::string str(doc.select_node("imgdir").node().attribute("name").value());
				str.append("/");
				str.append(first->node().attribute("name").value()).append("/");
				str.append(begin->attribute("name").value());

				std::string path("Character\\");
				path.append(doc.select_node("imgdir").node().attribute("name").value()).append("\\");
				path.append(first->node().attribute("name").value()).append(".");
				path.append(begin->attribute("name").value());
				SkinInfo* skininfo = new SkinInfo(str);

				//std::string path;
				//path.append("imdir/").append("imgdir[@name='").append(first->node().attribute("name").value()).append("']");

				item->SetName(begin->attribute("name").value());

				//if (!strcmp(begin->name(), "canvas") || !strcmp(begin->name(), "imgdir"))
				//{
				//	SkinFrame* frame = new SkinFrame();
				//	frame->SetPath(path);
				//	SetCanvasInfo(begin, "", frame);
				//	item->AddFrame(frame);
				//}
				//else 
				//{
				for (auto canvas = begin->children().begin(); canvas != begin->children().end(); ++canvas)
				{
					SkinFrame* frame = new SkinFrame();
					frame->SetPath(path);
					std::cout << canvas->name() << std::endl;

					if (!strcmp(first->node().attribute("name").value(), "front"))
					{
						std::cout << canvas->name() << std::endl;
					}


					if (!strcmp(canvas->name(), "canvas"))
					{
						SetCanvasInfo(canvas, begin->attribute("name").value(), frame);
					}
					else if (!strcmp(canvas->name(), "uol"))
					{
						std::cout << first->node().attribute("name").value() << std::endl;
						std::string name(begin->attribute("name").value());
						name.append("/").append(canvas->attribute("name").value());
						frame->SetName(name);

						std::string str(canvas->attribute("value").value());
						std::string mod("../");
						size_t start_pos = 0; //stringó������ �˻�

						while ((start_pos = str.find(mod, start_pos)) != std::string::npos)  //from�� ã�� �� ���� ������
						{
							str.replace(start_pos, mod.length(), "");
						}
						frame->SetUol(str);
						//item->SetImage(image);

					}
					else if (!strcmp(canvas->name(), "int"))
					{
						if (!strcmp(canvas->attribute("name").value(), "delay"))
						{
							skininfo->SetDelay(std::stoi(canvas->attribute("value").value()));
						}
					}
					item->AddFrame(frame);
				}
				//}
				skininfo->SetSkinItem(item);
				SkinManager::GetInstance()->AddSkin(skininfo);
			}
		}
	}
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
	for (auto begin = node->attributes().begin(); begin != node->attributes().end(); ++begin)
	{
		//std::cout << node->name() << " : " << begin->name() << " : " << begin->value() << std::endl;
		if (!strcmp(node->name(), "canvas"))
		{
			if (!strcmp(begin->name(), "name"))
			{
				std::string str(nummber);
				if (nummber.size() > 0)
					str.append("/");
				str.append(begin->value());
				item->SetName(str);
				std::string path(item->GetPath());


				if (nummber.size() > 0)
				{
					path.append(".");
					path.append(begin->value());
				}
				size_t pos = item->GetPath().find(".png");
				if (item->GetPath().find(".png") == std::string::npos)
				{
					path.append(".png");
				}
				item->SetPath(path);
			}
			else if (!strcmp(begin->name(), "width"))
			{
				item->SetWidth(std::stoi(begin->value()));
			}
			else if (!strcmp(begin->name(), "height"))
			{
				item->SetHeight(std::stoi(begin->value()));
			}
			else if (!strcmp(begin->name(), "basedata"))
			{
				std::wstring wstr;
				auto str = item->GetPath();
				wstr.assign(str.begin(), str.end());
				auto image = new Gdiplus::Image(wstr.c_str());
				item->SetImage(image);
			}
		}
		else if (!strcmp(node->name(), "vector"))
		{
			//std::cout << begin->value() << std::endl;
			if (!strcmp(begin->value(), "origin"))
			{
				++begin;
				item->SetOriginX(std::stof(begin->value()));
				++begin;
				item->SetOriginY(std::stof(begin->value()));
			}
		}
		else if (!strcmp(node->name(), "imgdir"))
		{
			if (item->GetPath().find("stand1") != std::string::npos)
			{
				int a = 123;
				std::cout << begin->value() << std::endl;
			}
			if (!strcmp(begin->value(), "map"))
			{
				SetMapInfo(node->children().begin(), node->children().end(), item);
			}
		}
		else if (!strcmp(node->name(), "uol"))
		{
			std::string name(nummber);
			if (nummber.size() > 0)
				name.append("/");
			name.append(begin->value());
			item->SetName(name);

			std::string str(node->attribute("value").value());
			std::string mod("../");
			size_t start_pos = 0; //stringó������ �˻�

			while ((start_pos = str.find(mod, start_pos)) != std::string::npos)  //from�� ã�� �� ���� ������
			{
				str.replace(start_pos, mod.length(), "");
			}
			item->SetUol(str);
			//item->SetImage(image);

		}
		else if (!strcmp(node->name(), "int"))
		{
			//std::cout << begin->value() << std::endl;
			if (!strcmp(begin->value(), "delay"))
			{
				++begin;
				auto zx = std::stoi(begin->value());
				auto xc = begin->name();
				//item->SetDelay(zx);
			}
		}

	}
	if (node->children().begin() != node->children().end())
	{
		HasChild(node->children().begin(), node->children().end(), nummber, item);
	}
}

void XmlReader::SetMapInfo(pugi::xml_node_iterator itr, pugi::xml_node_iterator end, SkinFrame* item)
{
	for (auto begin1 = itr; begin1 != end; ++begin1)
	{
		std::pair<std::string, ObjectPos> map;
		for (auto begin = begin1->attributes().begin(); begin != begin1->attributes().end(); ++begin)
		{
			if (!strcmp(begin->name(), "name"))
			{
				map.first = begin->value();
			}
			else if (!strcmp(begin->name(), "x"))
			{
				map.second.x = std::stof(begin->value());
			}
			else if (!strcmp(begin->name(), "y"))
			{
				map.second.y = std::stof(begin->value());
			}
		}
		item->AddMap(map);
	}
}

void XmlReader::HasChild(pugi::xml_node_iterator begin, pugi::xml_node_iterator end, std::string nummber, SkinFrame* item)
{
	for (; begin != end; ++begin)
	{
		SetCanvasInfo(begin, nummber, item);
	}
}