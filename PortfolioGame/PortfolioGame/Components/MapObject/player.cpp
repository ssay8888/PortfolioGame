#include "../../pch.h"
#include "player.h"
#include "../../Managers/KeyManaer/key_manager.h"
#include "../../Managers/Skins/skin_frame.h"
#include "../../Managers/Skins/skin_info.h"
#include "../../Managers/Skins/skin_item.h"
#include "../../Managers/Skins/skin_parts.h"
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
	if (!_isJump && keymanager->KeyPressing(KEY_DOWN))
	{
		this->ChangeFrameState("prone");
		_isProne = true;
	}
	if (!_isProne)
	{
		if (keymanager->KeyPressing(KEY_LEFT))
		{
			totalMoveX -= GetSpeed();
		}
		if (keymanager->KeyPressing(KEY_RIGHT))
		{
			totalMoveX += GetSpeed();
		}
		if (_isJump || keymanager->KeyDown(KEY_C))
		{
			if (!_isJump)
			{
				this->ChangeFrameState("jump");
				_isJump = true;
			}
			else
			{
				++_jumpCount;
				const int count = 45;
				const float speed = 2.f;
				if (_jumpCount < count)
				{
					totalMoveY -= speed;
				}
				else if (_jumpCount < count * 2 - 1)
				{
					totalMoveY += speed;
				}
				else
				{
					_isJump = false;
					_jumpCount = 0;
				}
			}
		}
	}
	if (keymanager->KeyPressing(KEY_UP))
	{
	}
	if (keymanager->KeyUp(KEY_DOWN))
	{
		this->ChangeFrameState("stand1");
		_isProne = false;
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
		if (!_isJump && strcmp(GetFrameState(), "walk1"))
		{
			this->ChangeFrameState("walk1");
		}

		float outY = 0;
		_info.x += totalMoveX;
		_info.y += totalMoveY;
		//if (_isJump || MapManager::GetInstance()->FootholdCollision(_info, &outY))
		//{
		//	if (_isJump)
		//	{
		//		m_fAccel += 0.20f;
		//		totalMoveY -= m_fJumpPower * m_fAccel - m_fGravity * m_fAccel * m_fAccel * 0.5f;
		//		if (bIsColl && m_tInfo.fY >= fY)
		//		{
		//			m_bIsJump = false;
		//			m_fAccel = 0.f;
		//			m_tInfo.fY = fY;
		//		}
		//	}
		//	else if (bIsColl)
		//		m_tInfo.fY = fY;
		//}
		//else
		//{
		//	_info.x -= totalMoveX;
		//	_info.y -= totalMoveY;
		//}
	}
	else 
	{
		if (!_isJump && !_isProne && strcmp(GetFrameState(), "stand1"))
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
					if (++_frameNummber >= _frameThis->GetParts()->size())
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
	//std::vector<SkinItem*> bodyList;
	//std::vector<SkinItem*> headList;
	//for (; maxSize < 100; maxSize++)
	//{
		std::vector<SkinItem*> tempList;
		snprintf(bodyStr, 100, "000%05d.img/%s/%d", _skinId + 2000, frameName.c_str(), maxSize);
		snprintf(headStr, 100, "000%05d.img/%s/%d", _skinId + 12000, frameName.c_str(), maxSize);
		auto bodySkinInfo = SkinManager::GetInstance()->GetBodySkinInfo(std::to_string(_skinId + 2000));
		auto headSkinInfo = SkinManager::GetInstance()->GetHeadSkinInfo(std::to_string(_skinId + 12000));
		if (bodySkinInfo != nullptr && headSkinInfo != nullptr)
		{
			_skinFrames.insert({ frameName, bodySkinInfo->FindBodySkinItem(frameName) });
			_headSkinFrames.insert({ frameName, headSkinInfo->FindHeadSkinItem(frameName) });
			//bodyList.push_back(bodySkinInfo->FindBodySkinItem(frameName));
			//headList.push_back(headSkinInfo->FindHeadSkinItem(frameName));
		}
	//}
}

void Player::RenderCharacter(HDC hdc)
{
	this->LoadCharacterFrame(_frameState);

	auto bodyFrameIter = _skinFrames.find(_frameState.c_str());
	auto headFrameIter = _headSkinFrames.find(_frameState.c_str());
	if (bodyFrameIter != _skinFrames.end() 
		&& headFrameIter != _headSkinFrames.end())
	{

		std::vector<SkinParts*> partsFrames;
		std::vector<SkinParts*> offsets;
		SkinParts* bodyFrame = nullptr;
		std::map<std::string, POINT> list;
		_thisFrameMaxCount = bodyFrameIter->second->GetFrameSize();
		auto bodyskinItem = bodyFrameIter->second;
		auto headskinItem = headFrameIter->second;
		if (bodyskinItem && headskinItem)
		{
			auto bodyFrames = bodyskinItem->FindFrame(std::to_string(_frameNummber % _thisFrameMaxCount));
			auto bodyParts = bodyFrames->GetParts();
			for (auto part = bodyParts->begin(); part != bodyParts->end(); ++part)
			{
				for (auto frame = part->second->GetMaps()->begin(); frame != part->second->GetMaps()->end(); ++frame)
				{
					if (!strcmp(part->first.c_str(), "body"))
					{
						_frameThis = bodyFrames;
						bodyFrame = part->second;
					}
				}
				partsFrames.emplace_back(part->second);
				offsets.emplace_back(part->second);
			}
			auto headFrames = headskinItem->FindFrame(std::to_string(_frameNummber % _thisFrameMaxCount));
			auto headParts = headFrames->GetParts();
			for (auto part = headParts->begin(); part != headParts->end(); ++part)
			{
				for (auto frame = part->second->GetMaps()->begin(); frame != part->second->GetMaps()->end(); ++frame)
				{
					partsFrames.emplace_back(part->second);
					offsets.emplace_back(part->second);
				}
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
			std::pair<std::string, POINT> anchorPointEntry{};

			for (auto begin = (*offsetPairing)->GetMaps()->begin(); begin != (*offsetPairing)->GetMaps()->end(); ++begin)
			{
				auto item = list.find(begin->first);
				if (item != list.end()) {
					anchorPointEntry = (*begin);
					break;
				}
			}
			auto temp = list.find(anchorPointEntry.first);
			POINT anchorPoint{};
			if (temp != list.end())
			{
				anchorPoint = temp->second;
			}
			POINT vectorPoint = anchorPointEntry.second;
			POINT fromAnchorPoint{ anchorPoint.x - vectorPoint.x, anchorPoint.y - vectorPoint.y };

			for (auto childAnchorPoint = (*offsetPairing)->GetMaps()->begin(); childAnchorPoint != (*offsetPairing)->GetMaps()->end(); ++childAnchorPoint)
			{
				if (childAnchorPoint->first != anchorPointEntry.first)
				{
					if (list.find(childAnchorPoint->first) == list.end())
					{
						list.insert({ childAnchorPoint->first,
							{ fromAnchorPoint.x + childAnchorPoint->second.x,
							  fromAnchorPoint.y + childAnchorPoint->second.y } });
					}
				}
			}
			offsetPairing = offsets.erase(offsetPairing);
		}

		auto neckOffsetBody = bodyFrame->GetMaps()->find("neck")->second;
		auto navelOffsetBody = bodyFrame->GetMaps()->find("navel")->second;
		std::list<std::pair<SkinParts*, POINT>> positionedFramesList;
		for (auto positionedFrame : partsFrames)
		{
			if (positionedFrame->GetMaps()->size() > 0)
			{
				auto anchorPointEntry = positionedFrame->GetMaps()->begin();

				POINT anchorPoint{};
				if (list.find(anchorPointEntry->first) != list.end())
					anchorPoint = list.find(anchorPointEntry->first)->second;

				auto vectorFromPoint = anchorPointEntry->second;
				neckOffsetBody = { anchorPoint.x - vectorFromPoint.x, anchorPoint.y - vectorFromPoint.y };
			}
			auto partOrigin = positionedFrame->GetOrigin();
			auto tempPos = POINT{ neckOffsetBody.x - partOrigin.x, neckOffsetBody.y - partOrigin.y };
			positionedFramesList.push_back({ positionedFrame, tempPos });
		}
		auto minXPair = std::min_element(positionedFramesList.begin(),
			positionedFramesList.end(),
			[](const std::pair<SkinParts*, POINT>& lhs, const std::pair<SkinParts*, POINT>& rhs) {
				return lhs.second.x + lhs.first->GetBitmap()->GetWidth() < rhs.second.x + rhs.first->GetBitmap()->GetWidth();
			});
		auto minX = minXPair->second.x + minXPair->first->GetBitmap()->GetWidth();

		auto maxXPair = std::max_element(positionedFramesList.begin(),
			positionedFramesList.end(),
			[](const std::pair<SkinParts*, POINT>& lhs, const std::pair<SkinParts*, POINT>& rhs) {
				return lhs.second.x + lhs.first->GetBitmap()->GetWidth() < rhs.second.x + rhs.first->GetBitmap()->GetWidth();
			});
		auto maxX = maxXPair->second.x + maxXPair->first->GetBitmap()->GetWidth();

		auto minYPair = std::max_element(positionedFramesList.begin(),
			positionedFramesList.end(),
			[](const std::pair<SkinParts*, POINT>& lhs, const std::pair<SkinParts*, POINT>& rhs) {
				return lhs.second.y + lhs.first->GetBitmap()->GetHeight() < rhs.second.y + rhs.first->GetBitmap()->GetHeight();
			});
		auto minY = minYPair->second.y + minYPair->first->GetBitmap()->GetHeight();

		auto maxYPair = std::max_element(positionedFramesList.begin(),
			positionedFramesList.end(),
			[](const std::pair<SkinParts*, POINT>& lhs, const std::pair<SkinParts*, POINT>& rhs) {
				return lhs.second.y + lhs.first->GetBitmap()->GetHeight()  < rhs.second.y + rhs.first->GetBitmap()->GetHeight();
			});

		auto maxY = maxYPair->second.y + maxYPair->first->GetBitmap()->GetHeight();


		if (bodyFrame->GetMaps()->find("neck") != bodyFrame->GetMaps()->end())
		{
			int plus = (_isProne ? 25 : 0);
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
				draw.first->GetBitmap()->RenderBitmapImage(_memDC,
					static_cast<int>(std::floor(_info.cx + draw.second.x - maxX)) + plus,
					static_cast<int>(std::floor(_info.cy + draw.second.y - maxY)) ,
					static_cast<int>(draw.first->GetBitmap()->GetWidth()),
					static_cast<int>(draw.first->GetBitmap()->GetHeight()));
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
				static_cast<int>(std::floor(_rect.left + ScrollManager::GetScrollX()))  - (GetFacingDirection() ? plus * -1 : plus),
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
	//float fY = 0.f;
	//bool bIsColl = MapManager::GetInstance()->FootholdCollision(_info, &fY);

	//if (_isJump)
	//{
	//	_accel += 0.10f;
	//	float value = _jumpPower * _accel - _gravity * _accel * _accel * 0.5f;
	//	std::cout << std::to_string(value) << std::endl;
	//	_info.y -= value;
	//	if (bIsColl && _info.y >= fY)
	//	{
	//		_isJump = false;
	//		_accel = 0.f;
	//		_info.y = fY;
	//	}
	//}
	//else if (bIsColl)
	//	_info.y = fY;
}

void Player::SetFrameThis(SkinFrame* frame)
{
	_frameThis = frame;
}

SkinFrame* Player::GetFrameThis()
{
	return _frameThis;
}

std::vector<SkinInfo*> Player::FindSkinFrame() const
{

	return std::vector<SkinInfo*>();
}

void Player::InsertFrame(SkinInfo* item)
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
