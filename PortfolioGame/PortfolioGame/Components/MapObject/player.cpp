#include "../../pch.h"
#include "player.h"
#include "../../Managers/KeyManaer/key_manager.h"
#include "../../Managers/Skins/skin_frame.h"
#include "../../Managers/Skins/skin_info.h"
#include "../../Managers/Skins/skin_item.h"
#include "../../Managers/Skins/skin_manager.h"
#include "../../Managers/ScrollManager/scroll_manager.h"
#include "../../Managers/MapManager/map_manager.h"
#include "../../../Common/Managers/BitmapManager/my_bitmap.h"

Player::Player(uint8_t layer) :
	GameObject(layer),
	_frameRevers(false),
	_thisFrameMaxCount(0),
	_frameState("stand1"),
	_frameTick(0)
{
    ReadyGameObject();
}

Player::~Player()
{
	DeleteObject(_hBitmap);
	DeleteObject(_oldBitmap);
	DeleteDC(_memDC);
}

int Player::ReadyGameObject()
{
	_info.x = 400.f;
	_info.y = 300.f;
	_info.cx = 42;
	_info.cy = 64;
	_speed = 1;
	LoadCharacterFrame("stand1");

	HDC hDC = GetDC(_hWnd);

	_memDC = CreateCompatibleDC(hDC);
	_hBitmap = CreateCompatibleBitmap(hDC, 1024, 768);
	_oldBitmap = (HBITMAP)SelectObject(_memDC, _hBitmap);
	ReleaseDC(_hWnd, hDC);

    return 0;
}

void Player::UpdateGameObject(const float deltaTime)
{
	auto keymanager = KeyManager::GetInstance();
	float totalMoveX = 0;
	float totalMoveY = 0;
	if (keymanager->KeyPressing(KEY_LEFT))
	{
		totalMoveX -= GetSpeed();
	}
	if (keymanager->KeyPressing(KEY_RIGHT))
	{
		totalMoveX += GetSpeed();
	}
	if (keymanager->KeyPressing(KEY_UP))
	{
		//totalMoveY -= GetSpeed();
	}
	if (keymanager->KeyPressing(KEY_DOWN))
	{
		this->ChangeFrameState("prone");
		_prone = true;
		//totalMoveY += GetSpeed();
	}
	
	if (keymanager->KeyUp(KEY_DOWN))
	{
		this->ChangeFrameState("stand1");
		_prone = false;
	}
	if (totalMoveX != 0 || totalMoveY != 0)
	{
		if (totalMoveX < 0)
		{
			this->SetFacingDirection(0);
		}
		else 
		{
			this->SetFacingDirection(1);
		}
		if (strcmp(GetFrameState(), "walk1"))
		{
			this->ChangeFrameState("walk1");
		}

		float outY = 0;
		_info.x += totalMoveX;
		_info.y += totalMoveY;
		if (MapManager::GetInstance()->FootholdCollision(_info, &outY))
		{
		}
		else
		{
			_info.x -= totalMoveX;
			_info.y -= totalMoveY;
		}
	}
	else 
	{
		if (!_prone && strcmp(GetFrameState(), "stand1"))
		{
			this->ChangeFrameState("stand1");
		}
	}

	if (keymanager->KeyPressing(KEY_A))
	{
		this->ChangeFrameState("alert");
	}
	if (keymanager->KeyPressing(KEY_B))
	{
		this->ChangeFrameState("swingT1");
	}
	if (_frameThis != nullptr)
	{
		uint64_t tick = GetTickCount64();
		if (tick > _frameTick + _frameThis->GetDelay())
		{
			if (!strcmp(GetFrameState(), "stand1"))
			{
				if (_frameRevers)
				{
					if (--_frameNummber == 0)
					{
						_frameRevers = false;
					}
				}
				else
				{
					if (++_frameNummber >= _thisFrameMaxCount - 1)
					{
						_frameRevers = true;
					}
				}
			}
			else
			{
				++_frameNummber;
			}
			_frameTick = tick;
			std::cout << std::to_string(_frameNummber) << std::endl;
		}
	}


}

void Player::LoadCharacterFrame(std::string frameName, uint16_t frameCount)
{
	if (_skinFrames.find(frameName) != _skinFrames.end())
		return;

	char bodyStr[100];
	char headStr[100];
	uint16_t maxSize = 0;
	std::vector<std::vector<SkinInfo*>> allList;
	for (; maxSize < 100; maxSize++)
	{
		std::vector<SkinInfo*> tempList;
		snprintf(bodyStr, 100, "000%05d.img/%s/%d", _skinId + 2000, frameName.c_str(), maxSize);
		snprintf(headStr, 100, "000%05d.img/%s/%d", _skinId + 12000, frameName.c_str(), maxSize);
		auto bodySkinInfo = SkinManager::GetInstance()->GetSkinInfo(bodyStr);
		auto headSkinInfo = SkinManager::GetInstance()->GetSkinInfo(headStr);
		if (bodySkinInfo != nullptr && headSkinInfo != nullptr)
		{
			tempList.push_back(SkinManager::GetInstance()->GetSkinInfo(bodyStr));
			tempList.push_back(SkinManager::GetInstance()->GetSkinInfo(headStr));
			allList.push_back(tempList);
		}
		else
		{
			if (allList.empty())
			{
				return;
			}
			break;
		}
	}
	_skinFrames.insert({ frameName, {maxSize, allList} });
}

void Player::RenderCharacter(HDC hdc)
{
	this->LoadCharacterFrame(_frameState);

	auto frameItr = _skinFrames.find(_frameState.c_str());
	if (frameItr != _skinFrames.end())
	{

		std::vector<SkinFrame*> partsFrames;
		std::vector<SkinFrame*> offsets;
		SkinFrame* bodyFrame = nullptr;
		std::map<std::string, ObjectPos> list;

		_thisFrameMaxCount = frameItr->second.first;
		
		for (auto skinInfo : frameItr->second.second[_frameNummber % _thisFrameMaxCount])
		{
			auto frames = skinInfo->GetSkinItem()->GetFrames();
			for (auto frame = frames->begin(); frame != frames->end(); ++frame) 
			{
				if (frame->second->GetName().find("body", 0) != std::string::npos)

				//if (!strcmp(frame->second->GetName().c_str(), "body"))
				{
 					_frameThis = skinInfo;
					bodyFrame = frame->second;
				}
				partsFrames.emplace_back(frame->second);
				offsets.emplace_back(frame->second);
			}
		}

		std::sort(partsFrames.begin(), partsFrames.end(),
			[](auto& lhs, auto& rhs) {
				return lhs->GetPosition() < rhs->GetPosition();
			});
		std::sort(offsets.begin(), offsets.end(),
			[](auto& lhs, auto& rhs) {
				return lhs->GetPosition() < rhs->GetPosition();
			});

		list.insert({ "navel", {} });
		if (!strcmp(_frameState.c_str(), "alert"))
		{
			switch (_frameNummber % 3)
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
			if (positionedFrame->GetMapSize() > 0)
			{
				auto anchorPointEntry = positionedFrame->GetMap().begin();

				ObjectPos anchorPoint{};
				if (list.find(anchorPointEntry->first) != list.end())
					anchorPoint = list.find(anchorPointEntry->first)->second;

				auto vectorFromPoint = anchorPointEntry->second;
				neckOffsetBody = { anchorPoint.x - vectorFromPoint.x, anchorPoint.y - vectorFromPoint.y };
			}
			auto partOrigin = positionedFrame->GetOrigin();
			auto tempPos = ObjectPos{ neckOffsetBody.x - partOrigin.x, neckOffsetBody.y - partOrigin.y };
			positionedFramesList.push_back(std::make_pair(positionedFrame, tempPos));
		}
		auto minXPair = std::min_element(positionedFramesList.begin(),
			positionedFramesList.end(),
			[](const std::pair<SkinFrame*, ObjectPos>& lhs, const std::pair<SkinFrame*, ObjectPos>& rhs) {
				return lhs.second.x + lhs.first->GetWidth() < rhs.second.x + rhs.first->GetWidth();
			});
		auto minX = minXPair->second.x + minXPair->first->GetWidth();

		auto maxXPair = std::max_element(positionedFramesList.begin(),
			positionedFramesList.end(),
			[](const std::pair<SkinFrame*, ObjectPos>& lhs, const std::pair<SkinFrame*, ObjectPos>& rhs) {
				return lhs.second.x + lhs.first->GetWidth() < rhs.second.x + rhs.first->GetWidth();
			});
		auto maxX = maxXPair->second.x + maxXPair->first->GetWidth();

		auto minYPair = std::max_element(positionedFramesList.begin(),
			positionedFramesList.end(),
			[](const std::pair<SkinFrame*, ObjectPos>& lhs, const std::pair<SkinFrame*, ObjectPos>& rhs) {
				return lhs.second.y + lhs.first->GetHeight() < rhs.second.y + rhs.first->GetHeight();
			});
		auto minY = minYPair->second.y + minYPair->first->GetHeight();

		auto maxYPair = std::max_element(positionedFramesList.begin(),
			positionedFramesList.end(),
			[](const std::pair<SkinFrame*, ObjectPos>& lhs, const std::pair<SkinFrame*, ObjectPos>& rhs) {
				return lhs.second.y + lhs.first->GetHeight() < rhs.second.y + rhs.first->GetHeight();
			});

		auto maxY = maxYPair->second.y + maxYPair->first->GetHeight();

		if (bodyFrame->GetMap().find("neck") != bodyFrame->GetMap().end())
		{
			const int plus = 25;
			Rectangle(_memDC,
				_rect.left + static_cast<int>(ScrollManager::GetScrollX()) + plus,
				_rect.top + static_cast<int>(ScrollManager::GetScrollY()),
				_rect.right + static_cast<int>(ScrollManager::GetScrollX()), 
				_rect.bottom + static_cast<int>(ScrollManager::GetScrollY()));

			HBRUSH brush = CreateSolidBrush(RGB(255, 0, 255));
			HBRUSH brushPrev = (HBRUSH)SelectObject(_memDC, brush);
			Rectangle(_memDC, -30, -30, _info.cx + 30, _info.cy + 30);
			/*
				static_cast<int>(_rect.left + _info.cx + ScrollManager::GetScrollX() + minX),
				static_cast<int>(_rect.top + _info.cy + ScrollManager::GetScrollY() + minY),
				static_cast<int>(maxX),
				static_cast<int>(maxY)
				*/
			SelectObject(_memDC, brushPrev);
			DeleteObject(brush);
			DeleteObject(brushPrev);

			for (auto draw : positionedFramesList)
			{
				draw.first->GetImage()->RenderBitmapImage(_memDC,
					static_cast<int>(std::floor(_info.cx + draw.second.x - maxX)) + plus,
					static_cast<int>(std::floor(_info.cy + draw.second.y - maxY)) ,
					static_cast<int>(draw.first->GetWidth()),
					static_cast<int>(draw.first->GetHeight()));
				/*draw.first->GetImage()->RenderBitmapImage(_memDC,
					static_cast<int>(std::floor(_rect.left + _info.cx + ScrollManager::GetScrollX() + draw.second.x - maxX)),
					static_cast<int>(std::floor(_rect.top + _info.cy + ScrollManager::GetScrollY() + draw.second.y - maxY)),
					static_cast<int>(draw.first->GetWidth()),
					static_cast<int>(draw.first->GetHeight()));*/
			}

			if (GetFacingDirection())
			{
				StretchBlt(_memDC, 0, 0, 42 + plus, 64, _memDC, 41 + plus, 0, -42 - plus, 64, SRCCOPY);
			}

			GdiTransparentBlt(hdc, 
				static_cast<int>(std::floor(_rect.left + ScrollManager::GetScrollX())),
				static_cast<int>(std::floor(_rect.top  + ScrollManager::GetScrollY())),
				42 + plus,
				64,
				_memDC,
				0, 0,
				42 + plus,
				64,
				RGB(255, 0, 255));

			//ÁÂ¿ì¹ÝÀü
			//StretchBlt(hdc, 0, 0, 1024, 768, _memDC, 1023, 0, -1024, 768, SRCCOPY);

			//BitBlt(hdc, 0, 0, 1024, 768, _memDC, 0, 0, SRCCOPY);

		}

	}
}


void Player::RenderGameObject(HDC hdc)
{
	UpdateRectGameObject();
	RenderCharacter(hdc);
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

void Player::ChangeFrameState(std::string frame)
{
	LoadCharacterFrame(frame);
	_frameState = frame;
	_frameNummber = 0;
	_frameRevers = false;
}

const char* Player::GetFrameState() const
{
	//char findStr[30];
	//snprintf(findStr, 30, "%s/%d", _frameState.c_str(), _frameNummber);

	//return findStr;
	return _frameState.c_str();
}

uint16_t Player::GetFrameNummber(uint16_t remain)
{
	return _frameNummber % remain;
}

void Player::SetFacingDirection(uint8_t direction)
{
	_facingDirection = direction;
}

uint8_t Player::GetFacingDirection() const
{
	return _facingDirection;
}
