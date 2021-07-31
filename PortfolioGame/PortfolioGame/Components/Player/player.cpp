#include "../../pch.h"
#include "player.h"
#include "../../Managers/KeyManaer/key_manager.h"
#include "../../Managers/Skins/skin_frame.h"
#include "../../Managers/Skins/skin_info.h"
#include "../../Managers/Skins/skin_item.h"
#include "../../Managers/Skins/skin_manager.h"
#include "../../Managers/ScrollManager/scroll_manager.h"

Player::Player() :
	_frameState("stand1"),
	_frameTick(0)
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
	LoadCharacterFrame("stand1");
    return 0;
}

void Player::UpdateGameObject(const float deltaTime)
{
	auto keymanager = KeyManager::Get_Instance();
	if (keymanager->KeyPressing(KEY_LEFT))
	{
		float value = _speed * deltaTime;
		_info.x -= value;
		ScrollManager::GainScrollX(value);

	}
	if (keymanager->KeyPressing(KEY_RIGHT))
	{
		float value = _speed * deltaTime;
		_info.x += _speed * deltaTime;
		ScrollManager::GainScrollX(-value);
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
			_frameNummber++;
			_frameTick = tick;
		}
	}
}

void Player::LoadCharacterFrame(std::string frameName, uint16_t frameCount)
{
	char findStr[20];
	snprintf(findStr, 20, "%s/%d", frameName.c_str(), GetFrameNummber(frameCount));
	if (_skinFrames.find(findStr) != _skinFrames.end())
		return;

	char bodyStr[100];
	char headStr[100];
	snprintf(bodyStr, 100, "000%05d.img/%s/%d", _skinId + 2000, frameName.c_str(), GetFrameNummber(frameCount));
	snprintf(headStr, 100, "000%05d.img/%s/%d", _skinId + 12000, frameName.c_str(), GetFrameNummber(frameCount));
	auto bodySkinInfo = SkinManager::GetInstance()->GetSkinInfo(bodyStr);
	auto headSkinInfo = SkinManager::GetInstance()->GetSkinInfo(headStr);
	_skinFrames.insert({ findStr, { bodySkinInfo, headSkinInfo} });
}

void Player::RenderCharacter(HDC hdc)
{
	this->LoadCharacterFrame(_frameState);
	char findStr[20];
	snprintf(findStr, 20, "%s/%d", _frameState.c_str(), GetFrameNummber(3));
	auto frameItr = _skinFrames.find(findStr);
	if (frameItr != _skinFrames.end())
	{
		std::vector<SkinFrame*> partsFrames;
		std::vector<SkinFrame*> offsets;
		SkinFrame* bodyFrame = nullptr;
		std::map<std::string, ObjectPos> list;

		for (auto skinInfo : frameItr->second)
		{
			auto frames = skinInfo->GetSkinItem()->GetFrames();
			for (auto frame = frames->begin(); frame != frames->end(); ++frame) 
			{
				if (!strcmp(frame->second->GetName().c_str(), "body"))
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

		auto maxXPair = std::max_element(positionedFramesList.begin(),
			positionedFramesList.end(),
			[](const std::pair<SkinFrame*, ObjectPos>& lhs, const std::pair<SkinFrame*, ObjectPos>& rhs) {
				return lhs.second.x + lhs.first->GetWidth() < rhs.second.x + rhs.first->GetWidth();
			});
		auto maxX = maxXPair->second.x + maxXPair->first->GetWidth();

		auto maxYPair = std::max_element(positionedFramesList.begin(),
			positionedFramesList.end(),
			[](const std::pair<SkinFrame*, ObjectPos>& lhs, const std::pair<SkinFrame*, ObjectPos>& rhs) {
				return lhs.second.y + lhs.first->GetHeight() < rhs.second.y + rhs.first->GetHeight();
			});

		auto maxY = maxYPair->second.y + maxYPair->first->GetHeight();

		Gdiplus::Graphics destination(hdc);
		if (bodyFrame->GetMap().find("neck") != bodyFrame->GetMap().end())
		{
			Rectangle(hdc, 
				_rect.left + static_cast<int>(ScrollManager::GetScrollX()),
				_rect.top + static_cast<int>(ScrollManager::GetScrollY()),
				_rect.right + static_cast<int>(ScrollManager::GetScrollX()), 
				_rect.bottom + static_cast<int>(ScrollManager::GetScrollY()));

			for (auto draw : positionedFramesList)
			{
				Gdiplus::Rect rc{
					static_cast<int>(_rect.left + _info.cx + static_cast<int>(ScrollManager::GetScrollX()) + draw.second.x - maxX),
					static_cast<int>(_rect.top + _info.cy + static_cast<int>(ScrollManager::GetScrollY()) + draw.second.y - maxY),
					static_cast<int>(draw.first->GetWidth()),
					static_cast<int>(draw.first->GetHeight())
				};

				destination.DrawImage(draw.first->GetImage(), rc);
			}

			//BitBlt(hdc, static_cast<int>(_info.x), static_cast<int>(_info.y), 42, 64, hdc, 0, 0, SRCCOPY);

			//DeleteObject(hBitmap);
			//DeleteDC(buffer);

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
}

char* Player::GetFrameState()
{
	//char findStr[30];
	//snprintf(findStr, 30, "%s/%d", _frameState.c_str(), _frameNummber);

	//return findStr;
	return nullptr;
}

uint16_t Player::GetFrameNummber(uint16_t remain)
{
	return _frameNummber % remain;
}
