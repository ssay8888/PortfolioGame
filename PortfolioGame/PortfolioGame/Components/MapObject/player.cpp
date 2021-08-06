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
#include "foot_hold.h"
#include "../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../../Common/Managers/CollisionManager/Collision_Manager.h"

Player::Player(uint8_t layer) :
	GameObject(layer),
	_frameRevers(false),
	_thisFrameMaxCount(0),
	_frameState("stand1"),
	_frameTick(0),
	_nowFootHold(nullptr),
	_nextFootHold(nullptr),
	_isFirstFootHold(false)
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
	_info.x = 250.f;
	_info.y = 400.f;
	_info.cx = 42;
	_info.cy = 64;
	_speed = 2.f;
	LoadCharacterFrame("stand1");

	HDC hDC = GetDC(_hWnd);
	ScrollManager::SetScrollX(1024/2);
	ScrollManager::SetScrollY(768/2);

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
	float outY = 0;
	FootHold* tempHold;
	bool isFoothold = MapManager::GetInstance()->FootholdYCollision(this, &outY, &tempHold);
	if (!_isJump && !_isProne && keymanager->KeyPressing(KEY_DOWN))
	{
		float outX;
		FootHold* outHold = nullptr;
		bool isRope = MapManager::GetInstance()->LadderRopeCollsition(this, &outX, &outHold);
		if (isRope)
		{
			if (!_isRope)
			{
				switch (outHold->GetState())
				{
				case FootHold::HoldState::kLadder:
					this->ChangeFrameState("ladder");
					break;
				case FootHold::HoldState::kRope:
					this->ChangeFrameState("rope");
					break;
				default:
					break;
				}
				_isRope = true;
				_info.x = outX;
			}
			_info.y += GetSpeed();
			if (CollisionManager::LineAndRectCollsition(MapManager::GetInstance()->GetMapFootHold(), this))
			{
				if (!_isFirstFootHold)
				{
					_info.y -= GetSpeed();
				}
				else if (_isFirstFootHold && _nextFootHold == nullptr)
				{
					_info.y -= GetSpeed();
					_isRope = false;
				}
			}
			else
			{
				_info.y -= GetSpeed();
				totalMoveY += GetSpeed();
			}

		}
	}

	if (_info.y >= outY)
	{
		if (!_isJump && !_isProne && !_isRope && keymanager->KeyPressing(KEY_DOWN))
		{
			this->ChangeFrameState("prone");
			_isProne = true;
		}
	}

	if (keymanager->KeyUp(KEY_DOWN))
	{
		if (!_isRope)
		{
			this->ChangeFrameState("stand1");
			_isProne = false;
		}
	}
	if (!_isProne)
	{
		if (keymanager->KeyPressing(KEY_LEFT))
		{
			if (_isRope)
			{
				if (keymanager->KeyPressing(KEY_C))
				{
					_isRope = false;
					this->ChangeFrameState("jump");
					_isJump = true;
					totalMoveX -= GetSpeed();
				}
			}
			else
			{
				totalMoveX -= GetSpeed();
			}
		}
		if (keymanager->KeyPressing(KEY_RIGHT))
		{
			if (_isRope)
			{
				if (keymanager->KeyPressing(KEY_C))
				{
					_isRope = false;
					this->ChangeFrameState("jump");
					_isJump = true;
					totalMoveX += GetSpeed();
				}
			}
			else
			{
				totalMoveX += GetSpeed();
			}
		}
		if (_info.y >= outY)
		{
			if (!_isJump && keymanager->KeyPressing(KEY_C))
			{
				this->ChangeFrameState("jump");
				_isJump = true;
			}
		}
	}


	if (!_isJump && !_isProne && keymanager->KeyPressing(KEY_UP))
	{
		auto rope = MapManager::GetInstance()->GetListRopeLadder();
		float outX = 0;
		FootHold* outHold = nullptr;
		bool isRope = MapManager::GetInstance()->LadderRopeCollsition(this, &outX, &outHold);


		if (isRope)
		{
			_info.x = outX;
			totalMoveY -= GetSpeed();
			if (!_isRope)
			{
				switch (outHold->GetState())
				{
				case FootHold::HoldState::kLadder:
					this->ChangeFrameState("ladder");
					break;
				case FootHold::HoldState::kRope:
					this->ChangeFrameState("rope");
					break;
				default:
					break;
				}
				_isRope = true;
			}
		}
		else 
		{
			if (strcmp(GetFrameState(), "stand1"))
			{
				this->ChangeFrameState("stand1");
				_isRope = false;
			}
		}
	}

	if (totalMoveX != 0 || totalMoveY != 0)
	{
		if (totalMoveX < 0)
		{
			this->SetFacingDirection(0);
		}
		else if (totalMoveX > 0)
		{
			this->SetFacingDirection(1);
		}
		if (!_isRope && !_isJump && strcmp(GetFrameState(), "walk1"))
		{
			this->ChangeFrameState("walk1");
		}

		float outY = 0;
		_info.x += totalMoveX;
		_info.y += totalMoveY;
		UpdateRectGameObject();
		if (MapManager::GetInstance()->FootholdAndRectCollision(this))
		{
			_info.x -= totalMoveX;
			_info.y -= totalMoveY;
		}
	}
	else
	{
		if (!_isRope && !_isJump && !_isProne && strcmp(GetFrameState(), "stand1"))
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
		if (_isRope)
		{
			uint64_t tick = GetTickCount64();
			if (tick > _frameTick + _frameThis->GetDelay() && totalMoveY != 0)
			{
				_frameNummber++;
				_frameTick = tick;
				std::cout << _frameNummber << std::endl;
			}
		}
		else
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
	ScrollMove();


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
		std::map<std::string, ObjectPos> list;
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
					if ((!strcmp(part->second->GetZ().c_str(), "body") || !strcmp(part->second->GetZ().c_str(), "backBody")))
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
				partsFrames.emplace_back(part->second);
				offsets.emplace_back(part->second);
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

			for (auto begin = (*offsetPairing)->GetMaps()->begin(); begin != (*offsetPairing)->GetMaps()->end(); ++begin)
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
		std::list<std::pair<SkinParts*, ObjectPos>> positionedFramesList;
		for (auto positionedFrame : partsFrames)
		{
			if (positionedFrame->GetMaps()->size() > 0)
			{
				auto anchorPointEntry = positionedFrame->GetMaps()->begin();

				ObjectPos anchorPoint{};
				if (list.find(anchorPointEntry->first) != list.end())
					anchorPoint = list.find(anchorPointEntry->first)->second;

				auto vectorFromPoint = anchorPointEntry->second;
				neckOffsetBody = { anchorPoint.x - vectorFromPoint.x, anchorPoint.y - vectorFromPoint.y };
			}
			auto partOrigin = positionedFrame->GetOrigin();
			auto tempPos = ObjectPos{ neckOffsetBody.x - partOrigin.x, neckOffsetBody.y - partOrigin.y };
			positionedFramesList.push_back({ positionedFrame, tempPos });
		}
		auto minXPair = std::min_element(positionedFramesList.begin(),
			positionedFramesList.end(),
			[](const std::pair<SkinParts*, ObjectPos>& lhs, const std::pair<SkinParts*, ObjectPos>& rhs) {
				return lhs.second.x + lhs.first->GetBitmap()->GetWidth() < rhs.second.x + rhs.first->GetBitmap()->GetWidth();
			});
		auto minX = minXPair->second.x + minXPair->first->GetBitmap()->GetWidth();

		auto maxXPair = std::max_element(positionedFramesList.begin(),
			positionedFramesList.end(),
			[](const std::pair<SkinParts*, ObjectPos>& lhs, const std::pair<SkinParts*, ObjectPos>& rhs) {
				return lhs.second.x + lhs.first->GetBitmap()->GetWidth() < rhs.second.x + rhs.first->GetBitmap()->GetWidth();
			});
		auto maxX = maxXPair->second.x + maxXPair->first->GetBitmap()->GetWidth();

		auto minYPair = std::max_element(positionedFramesList.begin(),
			positionedFramesList.end(),
			[](const std::pair<SkinParts*, ObjectPos>& lhs, const std::pair<SkinParts*, ObjectPos>& rhs) {
				return lhs.second.y + lhs.first->GetBitmap()->GetHeight() < rhs.second.y + rhs.first->GetBitmap()->GetHeight();
			});
		auto minY = minYPair->second.y + minYPair->first->GetBitmap()->GetHeight();

		auto maxYPair = std::max_element(positionedFramesList.begin(),
			positionedFramesList.end(),
			[](const std::pair<SkinParts*, ObjectPos>& lhs, const std::pair<SkinParts*, ObjectPos>& rhs) {
				return lhs.second.y + lhs.first->GetBitmap()->GetHeight()  < rhs.second.y + rhs.first->GetBitmap()->GetHeight();
			});

		auto maxY = maxYPair->second.y + maxYPair->first->GetBitmap()->GetHeight();


		if (bodyFrame->GetMaps()->find("neck") != bodyFrame->GetMaps()->end())
		{
			int plus = (_isProne ? 25 : 0);
			HBRUSH brush = CreateSolidBrush(RGB(255, 0, 255));
			HBRUSH brushPrev = (HBRUSH)SelectObject(_memDC, brush);
			Rectangle(_memDC, -30, -30, _info.cx + 30, _info.cy + 30);
			SelectObject(_memDC, brushPrev);
			DeleteObject(brush);
			DeleteObject(brushPrev);

			for (auto draw : positionedFramesList)
			{
				draw.first->GetBitmap()->RenderBitmapImage(_memDC,
					static_cast<int>(std::floor(_info.cx + draw.second.x - maxX)) + plus,
					static_cast<int>(std::floor(_info.cy + draw.second.y - maxY)),
					static_cast<int>(draw.first->GetBitmap()->GetWidth()),
					static_cast<int>(draw.first->GetBitmap()->GetHeight()));
			}

			if (GetFacingDirection())
			{
				StretchBlt(_memDC, 0, 0, 42 + plus, 64, _memDC, 41 + plus, 0, -42 - plus, 64, SRCCOPY);
			}

			Rectangle(_memDC,
				_rect.left + static_cast<int>(ScrollManager::GetScrollX()) + plus,
				_rect.top + static_cast<int>(ScrollManager::GetScrollY()),
				_rect.right + static_cast<int>(ScrollManager::GetScrollX()),
				_rect.bottom + static_cast<int>(ScrollManager::GetScrollY()));

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
		}
	}
}

void Player::IsJumping()
{
	if(_isRope) 
	{
		float outY = 0;
		_isFirstFootHold = true;
		bool isFoothold = MapManager::GetInstance()->FootholdYCollision(this, &outY, &_nextFootHold);
		return;
	}
	float outY = 0;
	bool isFoothold = MapManager::GetInstance()->FootholdYCollision(this, &outY, &_nowFootHold);

	const int count = 14;
	const float speed = 9.f;
	if (_isJump)
	{
		++_jumpCount;
		if (_jumpCount < count)
		{
			_info.y -= speed;
			//ScrollManager::GainScrollY(speed, true);
		}
		else
		{
			_isJump = false;
			_jumpCount = 0;
		}
	}
	else if (!_isRope && isFoothold)
	{
		if (_info.y <= outY)
		{
			if (_info.y - outY <= speed && _info.y - outY >= -speed)
			{
				_info.y = outY;
				//ScrollManager::SetScrollY(-_info.y + (768 / 2));
			}
			else
			{
				_info.y += speed;
				//ScrollManager::GainScrollY(-speed);
			}
		}
		else
		{
			_info.y = outY;
			//ScrollManager::SetScrollY(-_info.y + (768 / 2));
		}
	}
	//float fY = 0.f;
	//bool bIsColl = MapManager::GetInstance()->FootholdCollision(this, &fY);

	//if (_isJump)
	//{
	//	_accel += 0.20f;
	//	_info.y -= _jumpPower * _accel - _gravity * _accel * _accel * 0.5f;
	//	int i = 0;
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


void Player::RenderGameObject(HDC hdc)
{
	UpdateRectGameObject();
	RenderCharacter(hdc);
}

void Player::LateUpdateGameObject()
{
	Player::IsJumping();
}

void Player::ScrollMove()
{
	const int range = 75;

	const int divideCX = WindowCX >> 1;
	const int divideCY = WindowCY >> 1;


	const float scrollX = ScrollManager::GetScrollX();
	const float scrollY = ScrollManager::GetScrollY();
	const float  sumX = _info.x + scrollX;
	const float  sumY = _info.y + scrollY;

	if (divideCX < sumX + range)
	{
		ScrollManager::GainScrollX(-GetSpeed());
	}
	if (divideCX > sumX - range)
	{
		ScrollManager::GainScrollX(GetSpeed());
	}

	if (divideCY < sumY + range)
	{
		ScrollManager::GainScrollY(-GetSpeed());
	}
	if (divideCY > sumY - range)
	{
		ScrollManager::GainScrollY(GetSpeed());
	}

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

FootHold* Player::GetNowFootHold()
{
	return _nowFootHold;
}

void Player::SetNowFootHold(FootHold* hold)
{
	_nowFootHold = hold;
}
