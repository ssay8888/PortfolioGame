#include "../pch.h"
#include "main_game.h"
#include "../Managers/Skins/skin_info.h"
#include "../Managers/Skins/skin_frame.h"
#include "../Managers/Skins/skin_item.h"
#include "../Managers/Skins/skin_manager.h"
#include "../Utility/xml_reader.h"
#include <fstream>
#include <algorithm>
MainGame::MainGame(HDC hdc) :
	_hdc(hdc),
	_hdc_buffer(nullptr),
	_hBitmap(nullptr),
	_oldBitmap(nullptr)
{
}

MainGame::~MainGame()
{
	ReleaseGame();
}

void MainGame::ReadeyGame()
{
	_hdc_buffer = CreateCompatibleDC(_hdc);
	_hBitmap = CreateCompatibleBitmap(_hdc, 1024, 768);
	_oldBitmap = (HBITMAP)SelectObject(_hdc_buffer, _hBitmap);

	XmlReader::GetInstance().LoadCharecterSkin(L"Character\\00002000.img.xml");
	XmlReader::GetInstance().LoadCharecterSkin(L"Character\\00012000.img.xml");
	auto size = SkinManager::GetInstance()->GetSize();
	auto size2 = SkinManager::GetInstance()->GetSkinInfo("");
}

void MainGame::UpdateGame()
{
}

void MainGame::RenderGame()
{

	Rectangle(_hdc_buffer, -10, -10, WindowCX + 10, WindowCY + 10);
	//auto data = list[0].c_str();
	/*auto a = new Gdiplus::Image(L"");
	Gdiplus::Graphics g(_hdc_buffer);
	Gdiplus::Rect rc{0,0,37,26};
	g.DrawImage(a, rc);
	delete a;*/
    auto info = SkinManager::GetInstance()->GetSkinInfo("00002000.img/stand1/0");
	auto info2 = SkinManager::GetInstance()->GetSkinInfo("00012000.img/stand1/0");
	
	std::vector<SkinFrame*> partsFrames;
	std::vector<SkinFrame*> offsets;
	auto frame = info->GetSkinItem()->GetFrame("0/arm");
	auto bodyFrame = info->GetSkinItem()->GetFrame("0/body");
	auto frame3 = info2->GetSkinItem()->GetFrame("0/head");
	partsFrames.push_back(bodyFrame);
	partsFrames.push_back(frame);
	partsFrames.push_back(frame3);
	offsets.push_back(bodyFrame);
	offsets.push_back(frame);
	offsets.push_back(frame3);

	std::map<std::string, ObjectPos> list;
	list.insert({ "navel", {} });
	for (auto offsetPairing = offsets.begin(); offsetPairing != offsets.end();)
	{
		std::pair<std::string, ObjectPos> anchorPointEntry{};
		//offsetPairing
		for (auto begin = (*offsetPairing)->GetMap().begin(); begin != (*offsetPairing)->GetMap().end(); ++begin)
		{
			auto item = list.find(begin->first);
			if (item != list.end()) {
				anchorPointEntry = (*begin);
				break;
			}
		}
		auto temp = list.find(anchorPointEntry.first);
		ObjectPos anchorPoint{};
		if (temp != list.end())
		{
			anchorPoint = temp->second;
		}
		ObjectPos vectorPoint = anchorPointEntry.second;
		ObjectPos fromAnchorPoint{ anchorPoint.x - vectorPoint.x, anchorPoint.y - vectorPoint.y };

		for (auto childAnchorPoint : (*offsetPairing)->GetMap())
		{
			if (childAnchorPoint.first != anchorPointEntry.first)
			{
				if (list.find(childAnchorPoint.first) == list.end())
				{
					list.insert({ childAnchorPoint.first,
						{ fromAnchorPoint.x + childAnchorPoint.second.x,
						  fromAnchorPoint.y + childAnchorPoint.second.y } });
				}
			}
		}
		offsetPairing = offsets.erase(offsetPairing);
	}
	
	auto neckOffsetBody = bodyFrame->GetMap().find("neck")->second;
	auto navelOffsetBody = bodyFrame->GetMap().find("navel")->second;
	std::list<std::pair<SkinFrame*, ObjectPos>> positionedFramesList;
	for (auto positionedFrame : partsFrames) 
	{
		auto fromAnchorPoint = neckOffsetBody;
		if (positionedFrame->GetMapSize() > 0)
		{
			auto anchorPointEntry = positionedFrame->GetMap().begin();

			ObjectPos anchorPoint{};
			if (list.find(anchorPointEntry->first) != list.end())
			anchorPoint = list.find(anchorPointEntry->first)->second;

			auto vectorFromPoint = anchorPointEntry->second;
			fromAnchorPoint = { anchorPoint.x - vectorFromPoint.x, anchorPoint.y - vectorFromPoint.y };
		}
		auto partOrigin = positionedFrame->GetOrigin();
		auto tempPos = ObjectPos{ fromAnchorPoint.x - partOrigin.x, fromAnchorPoint.y - partOrigin.y };
		positionedFramesList.push_back(std::make_pair(positionedFrame, tempPos));
	}

	auto minX = std::min_element(positionedFramesList.begin(),
		positionedFramesList.end(),
		[](const std::pair<SkinFrame*, ObjectPos>& lhs, const std::pair<SkinFrame*, ObjectPos>& rhs) {
			return lhs.second.x < rhs.second.x;
		})->second.x;
	auto maxXPair = std::max_element(positionedFramesList.begin(),
		positionedFramesList.end(),
		[](const std::pair<SkinFrame*, ObjectPos>& lhs, const std::pair<SkinFrame*, ObjectPos>& rhs) {
			return lhs.second.x + lhs.first->GetWidth() < rhs.second.x + rhs.first->GetWidth();
		});
	auto maxX = maxXPair->second.x + maxXPair->first->GetWidth();

	auto minY = std::min_element(positionedFramesList.begin(),
		positionedFramesList.end(),
		[](const std::pair<SkinFrame*, ObjectPos>& lhs, const std::pair<SkinFrame*, ObjectPos>& rhs) {
			return lhs.second.y < rhs.second.y;
		})->second.y;

	auto maxYPair = std::max_element(positionedFramesList.begin(),
		positionedFramesList.end(),
		[](const std::pair<SkinFrame*, ObjectPos>& lhs, const std::pair<SkinFrame*, ObjectPos>& rhs) {
			return lhs.second.y + lhs.first->GetHeight() < rhs.second.y + rhs.first->GetHeight();
		});

	auto maxY = maxYPair->second.y + maxYPair->first->GetHeight();
	Gdiplus::Graphics destination(_hdc_buffer);

	for (auto draw : positionedFramesList)
	{ 
		Gdiplus::Rect rc{ 
			static_cast<int>(draw.second.x - minX), 
			static_cast<int>(draw.second.y - minY), 
			static_cast<int>(draw.first->GetWidth()),
			static_cast<int>(draw.first->GetHeight()) };

		destination.DrawImage(draw.first->GetImage(), rc);
	}

	frame->GetMapItem("navel");
	//Gdiplus::Rect rc  { 0, 0, static_cast<int>(frame->GetWidth()),static_cast<int>(frame->GetHeight())};
	//Gdiplus::Rect rc2 { 0, 0, static_cast<int>(bodyFrame->GetWidth()),static_cast<int>(bodyFrame->GetHeight()) };
	//destination.DrawImage(frame->GetImage(), rc);

	//destination.DrawImage(bodyFrame->GetImage(), rc2);
	BitBlt(_hdc, 0, 0, WindowCX, WindowCY, _hdc_buffer, 0, 0, SRCCOPY);
}

void MainGame::ReleaseGame()
{
}
