#include "../../pch.h"
#include "player.h"
#include "../../Managers/KeyManaer/key_manager.h"
#include "../../Managers/Skins/skin_frame.h"
#include "../../Managers/Skins/skin_info.h"
#include "../../Managers/Skins/skin_item.h"
#include "../../Managers/Skins/skin_manager.h"

Player::Player()
{
    ReadyGameObject();
}

Player::~Player()
{
}

int Player::ReadyGameObject()
{
	_info.x = 400.f;
	_info.y = 300.f;
	_info.cx = 42;
	_info.cy = 64;
	_speed = 200.f;
    return 0;
}

void Player::UpdateActor(const float deltaTime)
{
	auto keymanager = KeyManager::Get_Instance();
	if (keymanager->KeyPressing(KEY_LEFT))
	{
		_info.x -= _speed * deltaTime;
	}
	if (keymanager->KeyPressing(KEY_RIGHT))
	{
		_info.x += _speed * deltaTime;
	}
}

void Player::RenderGameObject(HDC hdc)
{
	UpdateRectGameObject();

	auto info = SkinManager::GetInstance()->GetSkinInfo("00002000.img/stand1/1");
	auto info2 = SkinManager::GetInstance()->GetSkinInfo("00012000.img/stand1/1");

	std::vector<SkinFrame*> partsFrames;
	std::vector<SkinFrame*> offsets;
	auto frames = info2->GetSkinItem()->GetFrames();
	auto frames2 = info->GetSkinItem()->GetFrames();
	

	std::for_each(frames2->begin(), frames2->end(),
		[&partsFrames, &offsets](auto& data) {
			partsFrames.push_back(data.second);
			offsets.push_back(data.second);
		});

	std::for_each(frames->begin(), frames->end(),
		[&partsFrames, &offsets](auto& data) {
			partsFrames.push_back(data.second);
			offsets.push_back(data.second);
		});

	std::sort(partsFrames.begin(), partsFrames.end(),
		[](auto& lhs, auto& rhs) {
			return lhs->GetPosition() < rhs->GetPosition();
		});
	std::sort(offsets.begin(), offsets.end(),
		[](auto& lhs, auto& rhs) {
			return lhs->GetPosition() < rhs->GetPosition();
		});
	auto bodyFrame = info->GetSkinItem()->GetFindFrame("body");

	std::map<std::string, ObjectPos> list;

	list.insert({ "navel", {} });
	bool alert = true;
	if (alert)
	{
		int frame = 0;
		switch (frame)
		{
		case 0:
			list.insert({ "handMove", {-8, -2} });
			break;
		case 1:
			list.insert({ "handMove", {-10, 0} });
			break;
		case 2:
			list.insert({ "handMove", {-12, 3} });
			break;
		}
	}
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


	auto buffer = CreateCompatibleDC(hdc);
	auto hBitmap = CreateCompatibleBitmap(hdc, 42, 64);
	SelectObject(buffer, hBitmap);


	Gdiplus::Graphics destination(hdc);

	int left = static_cast<int>(_info.x - (_info.cx >> 1));
	int top = static_cast<int>(_info.y - (_info.cy * 0.5));
	int right = static_cast<int>(_info.x + (_info.cx >> 1));
	int bottom = static_cast<int>(_info.y + (_info.cy * 0.5));



	ObjectPos bodyShouldBe{ 36, 55 };

	ObjectPos cropOrigin;
	RECT cropArea;
	if (bodyFrame->GetMap().find("neck") != bodyFrame->GetMap().end())
	{
		ObjectPos temp = bodyFrame->GetMap().find("neck")->second;
		ObjectPos tempPos{ temp.x - bodyShouldBe.x, temp.y - bodyShouldBe.y };
		ObjectPos tempPos2{ minX , minY };
		cropOrigin.x = tempPos.x - tempPos2.x;
		cropOrigin.y = tempPos.y - tempPos2.y;
		cropArea.left = max(static_cast<long>(cropOrigin.x), 0);
		cropArea.top = max(static_cast<long>(cropOrigin.y), 0);
		ObjectPos cropOffsetFromOrigin{
			cropArea.left - cropOrigin.x, cropArea.top - cropOrigin.y };

		//Rectangle(hdc, left, top, right, bottom);

		for (auto draw : positionedFramesList)
		{
			Gdiplus::Rect rc{
				static_cast<int>(left + draw.second.x - minX),
				static_cast<int>(top + draw.second.y - minY),
				static_cast<int>(draw.first->GetWidth()),
				static_cast<int>(draw.first->GetHeight()) };

			destination.DrawImage(draw.first->GetImage(), rc);
		}

		//BitBlt(hdc, static_cast<int>(_info.x), static_cast<int>(_info.y), 42, 64, hdc, 0, 0, SRCCOPY);

		DeleteObject(hBitmap);
		DeleteDC(buffer);

	}






}

void Player::LateUpdateGameObject()
{
}

void Player::SetFrameThis(SkinInfo* frame)
{
	_frameThis = frame;
}

SkinInfo* Player::GetFrameThis()
{
	return _frameThis;
}

std::vector<SkinInfo*> Player::FindSkinFrame() const
{

	return std::vector<SkinInfo*>();
}

void Player::AddFrame(SkinInfo* item)
{
}
