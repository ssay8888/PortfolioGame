#include "../../../pch.h"
#include "player.h"
#include "Damage/damage_handler.h"
#include "../../../Managers/KeyManaer/key_manager.h"
#include "../../../Managers/Skins/skin_frame.h"
#include "../../../Managers/Skins/skin_info.h"
#include "../../../Managers/Skins/skin_item.h"
#include "../../../Managers/Skins/skin_parts.h"
#include "../../../Managers/Skins/skin_manager.h"
#include "../../../Managers/ScrollManager/scroll_manager.h"
#include "../../../Managers/MapManager/map_manager.h"
#include "../../../Managers/EffectManager/effect_manager.h"
#include "../../../Components/MapObject/Monster/monster.h"
#include "../foot_hold.h"
#include "../../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../../../Common/Managers/CollisionManager/Collision_Manager.h"

#include <time.h>
Player::Player(uint8_t layer) :
	GameObject(layer),
	_frame_revers(false),
	_this_frame_max_count(0),
	_frame_state("stand1"),
	_frame_tick(0),
	_now_foothold(nullptr),
	_next_foothold(nullptr),
	_is_first_foothold(false),
	_player_info({51, 1000, 432, 1000, 534, 1000, "����ȣ"})
{
    ReadyGameObject();
}

Player::~Player()
{
	SelectObject(_memDC, _old_bitmap);
	DeleteObject(_bitmap);
	DeleteDC(_memDC);
}

int Player::ReadyGameObject()
{
	_info.x = 250.f;
	_info.y = 800.f;
	_info.cx = 42;
	_info.cy = 64;
	_speed = 5.f;
	LoadCharacterFrame("stand1");

	HDC hDC = GetDC(_hWnd);
	ScrollManager::SetScrollX(1024/2);
	ScrollManager::SetScrollY(768/2);
	_damage_handler = new DamageHandler();
	_memDC = CreateCompatibleDC(hDC);
	_bitmap = CreateCompatibleBitmap(hDC, 1024, 768);
	_old_bitmap = (HBITMAP)SelectObject(_memDC, _bitmap);
	ReleaseDC(_hWnd, hDC);

	srand(int(time(NULL)));
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
	if (!_is_fly && !_is_jump && !_is_prone && keymanager->KeyPressing(KEY_DOWN))
	{
		float outX;
		FootHold* outHold = nullptr;
		bool isRope = MapManager::GetInstance()->LadderRopeCollsition(this, &outX, &outHold);
		if (isRope)
		{
			if (!_is_rope)
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
				_is_rope = true;
				_info.x = outX;
			}
			_info.y += GetSpeed();
			if (CollisionManager::LineAndRectCollsition(MapManager::GetInstance()->GetMapFootHold(), this))
			{
				if (!_is_first_foothold)
				{
					_info.y -= GetSpeed();
				}
				else if (_is_first_foothold && _next_foothold == nullptr)
				{
					_info.y -= GetSpeed();
					_is_rope = false;
				}
			}
			else if (_info.y >= outY)
			{
				_info.y = outY;
				_is_rope = false;
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
		if (!_is_attacking && !_is_jump && !_is_prone && !_is_rope && keymanager->KeyPressing(KEY_DOWN))
		{
			this->ChangeFrameState("prone");
			_is_prone = true;
		}
	}

	if (keymanager->KeyUp(KEY_DOWN))
	{
		if (!_is_attacking && !_is_rope)
		{
			this->ChangeFrameState("stand1");
			_is_prone = false;
		}
	}
	if (!_is_prone)
	{
		if (keymanager->KeyPressing(KEY_LEFT))
		{
			if (_is_rope)
			{
				if (keymanager->KeyPressing(KEY_C))
				{
					_is_rope = false;
					this->ChangeFrameState("jump");
					_is_jump = true;
					totalMoveX -= GetSpeed();
				}
			}
			else
			{
				if (((_is_jump || _is_fly) && _is_attacking) || !_is_attacking)
				{
					totalMoveX -= GetSpeed();
				}
			}
		}
		if (keymanager->KeyPressing(KEY_RIGHT))
		{
			if (_is_rope)
			{
				if (keymanager->KeyPressing(KEY_C))
				{
					_is_rope = false;
					this->ChangeFrameState("jump");
					_is_jump = true;
					totalMoveX += GetSpeed();
				}
			}
			else
			{
				if (((_is_jump || _is_fly) && _is_attacking) || !_is_attacking)
				{
					totalMoveX += GetSpeed();
				}
			}
		}
		if (_info.y >= outY)
		{
			if (!_is_attacking && !_is_jump && keymanager->KeyPressing(KEY_C))
			{
				this->ChangeFrameState("jump");
				_is_jump = true;
			}
		}
	}


	if (!_is_prone && keymanager->KeyPressing(KEY_UP))
	{
		auto rope = MapManager::GetInstance()->GetListRopeLadder();
		float outX = 0;
		FootHold* outHold = nullptr;
		bool isRope = MapManager::GetInstance()->LadderRopeCollsition(this, &outX, &outHold);


		if (isRope)
		{
			_info.x = outX;
			totalMoveY -= GetSpeed();
			if (!_is_rope)
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
				_is_rope = true;
				_is_fly = false;
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
		if (!_is_fly && !_is_rope && !_is_jump && strcmp(GetFrameState(), "walk1"))
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
		if (!_is_attacking && !_is_fly && !_is_rope && !_is_jump && !_is_prone && 
			strcmp(GetFrameState(), "stand1"))
		{
			if (IsAlertStateTick())
			{
				this->ChangeFrameState("alert");
			}
			else
			{
				this->ChangeFrameState("stand1");
			}
		}
	}


	if (!_is_attacking && keymanager->KeyPressing(KEY_A))
	{
		this->ChangeFrameState("swingO2");
		Player::TryMeleeAttack();
		UpdateAlertTick();
	}
	if (keymanager->KeyPressing(KEY_B))
	{
		this->ChangeFrameState("swingT1");
	}
	if (_frame_this != nullptr)
	{
		if (_is_rope)
		{
			uint64_t tick = GetTickCount64();
			if (tick > _frame_tick + _frame_this->GetDelay() && totalMoveY != 0)
			{
				_frame_nummber++;
				_frame_tick = tick;
			}
		}
	}
	ScrollMove();
	_damage_handler->UpdateDamages();
}

void Player::LoadCharacterFrame(std::string frameName, uint16_t frameCount)
{
	char bodyStr[100];
	char headStr[100];
	uint16_t maxSize = 0;
	//std::vector<SkinItem*> bodyList;
	//std::vector<SkinItem*> headList;
	//for (; maxSize < 100; maxSize++)
	//{
		std::vector<SkinItem*> tempList;
		snprintf(bodyStr, 100, "000%05d.img/%s/%d", _skin_id + 2000, frameName.c_str(), maxSize);
		snprintf(headStr, 100, "000%05d.img/%s/%d", _skin_id + 12000, frameName.c_str(), maxSize);
		auto bodySkinInfo = SkinManager::GetInstance()->GetBodySkinInfo(std::to_string(_skin_id + 2000));
		auto headSkinInfo = SkinManager::GetInstance()->GetBodySkinInfo(std::to_string(_skin_id + 12000));
		auto itemParts = SkinManager::GetInstance()->GetBodySkinInfo(std::to_string(1302000));
		auto hiarParts = SkinManager::GetInstance()->GetBodySkinInfo(std::to_string(30000));
		auto faceParts = SkinManager::GetInstance()->GetBodySkinInfo(std::to_string(20000));
		auto coatParts = SkinManager::GetInstance()->GetBodySkinInfo(std::to_string(1040002));
		auto pantsParts = SkinManager::GetInstance()->GetBodySkinInfo(std::to_string(1060002));
		auto shoesParts = SkinManager::GetInstance()->GetBodySkinInfo(std::to_string(1072001));
		if (bodySkinInfo != nullptr && headSkinInfo != nullptr)
		{
			_skin_frames.insert({ frameName, bodySkinInfo->FindBodySkinItem(frameName) });
			_head_skin_frames.insert({ frameName, headSkinInfo->FindHeadSkinItem(frameName) });
			//bodyList.push_back(bodySkinInfo->FindBodySkinItem(frameName));
			//headList.push_back(headSkinInfo->FindHeadSkinItem(frameName));
		}
		_skin_frames.erase(std::to_string(1302000));
		_skin_frames.insert({ std::to_string(1302000), itemParts->FindBodySkinItem(frameName) });
		_skin_frames.erase(std::to_string(1040002));
		_skin_frames.insert({ std::to_string(1040002), coatParts->FindBodySkinItem(frameName) });
		_skin_frames.erase(std::to_string(1060002));
		_skin_frames.insert({ std::to_string(1060002), pantsParts->FindBodySkinItem(frameName) });
		_skin_frames.erase(std::to_string(1072001));
		_skin_frames.insert({ std::to_string(1072001), shoesParts->FindBodySkinItem(frameName) });
		_skin_frames.erase(std::to_string(30000));
		_skin_frames.insert({ std::to_string(30000), hiarParts->FindBodySkinItem(frameName) });
		_skin_frames.erase(std::to_string(20000));
		_skin_frames.insert({ std::to_string(20000), faceParts->FindBodySkinItem("default") });
	//}
}

void Player::RenderCharacter(HDC hdc)
{
	this->LoadCharacterFrame(_frame_state);

	auto bodyFrameIter = _skin_frames.find(_frame_state.c_str());
	auto headFrameIter = _head_skin_frames.find(_frame_state.c_str());
	auto ItemFrameIter = _skin_frames.find(std::to_string(1302000));
	auto Item2FrameIter = _skin_frames.find(std::to_string(1040002));
	auto Item3FrameIter = _skin_frames.find(std::to_string(1060002));
	auto Item4FrameIter = _skin_frames.find(std::to_string(1072001));
	auto hairFrameIter = _skin_frames.find(std::to_string(30000));
	auto faceFrameItre = _skin_frames.find(std::to_string(20000));
	if (bodyFrameIter != _skin_frames.end() 
		&& headFrameIter != _head_skin_frames.end())
	{

		std::vector<SkinParts*> partsFrames;
		std::vector<SkinParts*> offsets;
		SkinParts* bodyFrame = nullptr;
		std::vector<std::pair<std::string, ObjectPos>> list;
		_this_frame_max_count = bodyFrameIter->second->GetFrameSize();
		auto bodyskinItem = bodyFrameIter->second;
		auto headskinItem = headFrameIter->second;
		auto bodyItemParts = ItemFrameIter->second;
		auto bodyItem2Parts = Item2FrameIter->second;
		auto bodyItem3Parts = Item3FrameIter->second;
		auto bodyItem4Parts = Item4FrameIter->second;
		auto bodyHairParts = hairFrameIter->second;
		auto faceParts = faceFrameItre->second;

		if (bodyskinItem && headskinItem)
		{
			auto bodyFrames = bodyskinItem->FindFrame(std::to_string(_frame_nummber % _this_frame_max_count));
			auto bodyParts = bodyFrames->GetParts();
			for (auto part = bodyParts->begin(); part != bodyParts->end(); ++part)
			{
				for (auto frame = part->second->GetMaps()->begin(); frame != part->second->GetMaps()->end(); ++frame)
				{
					if ((!strcmp(part->second->GetZ().c_str(), "body") || !strcmp(part->second->GetZ().c_str(), "backBody")))
					{
						_frame_this = bodyFrames;
						bodyFrame = part->second;
					}
				}
				partsFrames.emplace_back(part->second);
				offsets.emplace_back(part->second);
			}
			if (bodyItemParts)
			{
				auto itemFrames = bodyItemParts->FindFrame(std::to_string(_frame_nummber % _this_frame_max_count));
				auto itemParts = itemFrames->GetParts();
				for (auto part = itemParts->begin(); part != itemParts->end(); ++part)
				{
					partsFrames.emplace_back(part->second);
					offsets.emplace_back(part->second);
				}
			}
			if (bodyItem2Parts)
			{
				auto itemFrames = bodyItem2Parts->FindFrame(std::to_string(_frame_nummber % _this_frame_max_count));
				auto itemParts = itemFrames->GetParts();
				for (auto part = itemParts->begin(); part != itemParts->end(); ++part)
				{
					partsFrames.emplace_back(part->second);
					offsets.emplace_back(part->second);
				}
			}
			if (bodyItem3Parts)
			{
				auto itemFrames = bodyItem3Parts->FindFrame(std::to_string(_frame_nummber % _this_frame_max_count));
				auto itemParts = itemFrames->GetParts();
				for (auto part = itemParts->begin(); part != itemParts->end(); ++part)
				{
					partsFrames.emplace_back(part->second);
					offsets.emplace_back(part->second);
				}
			}
			if (bodyItem4Parts)
			{
				auto itemFrames = bodyItem4Parts->FindFrame(std::to_string(_frame_nummber % _this_frame_max_count));
				auto itemParts = itemFrames->GetParts();
				for (auto part = itemParts->begin(); part != itemParts->end(); ++part)
				{
					partsFrames.emplace_back(part->second);
					offsets.emplace_back(part->second);
				}
			}
			if (bodyHairParts)
			{
				auto itemFrames = bodyHairParts->FindFrame(std::to_string(_frame_nummber % _this_frame_max_count));
				auto itemParts = itemFrames->GetParts();
				for (auto part = itemParts->begin(); part != itemParts->end(); ++part)
				{
					partsFrames.emplace_back(part->second);
					offsets.emplace_back(part->second);
				}
			}
			if (faceParts && !_is_rope)
			{
				auto itemFrames = faceParts->FindFrame("face");
				auto itemParts = itemFrames->GetParts();
				for (auto part = itemParts->begin(); part != itemParts->end(); ++part)
				{
					partsFrames.emplace_back(part->second);
					offsets.emplace_back(part->second);
				}
			}
			auto headFrames = headskinItem->FindFrame(std::to_string(_frame_nummber % _this_frame_max_count));
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

		list.push_back({ "navel", {} });
		if (!strcmp(_frame_state.c_str(), "alert"))
		{
			switch (_frame_nummber % 3)
			{
			case 0:
				list.push_back({ "handMove", {-8, -2} });
				break;
			case 1:
				list.push_back({ "handMove", {-10, 0} });
				break;
			case 2:
				list.push_back({ "handMove", {-12, 3} });
				break;
			}
		}

		for (auto offsetPairing = offsets.begin(); offsetPairing != offsets.end();)
		{
			std::pair<std::string, ObjectPos> anchorPointEntry{};

			bool firstItem = false;
			for (auto begin = (*offsetPairing)->GetMaps()->begin(); begin != (*offsetPairing)->GetMaps()->end(); ++begin)
			{
				for (auto item : list)
				{
					if (begin->first == item.first)
					{
						firstItem = true;
						anchorPointEntry = (*begin);
						goto forexit;
					}
				}

			}
		forexit:
			if (!firstItem) {
				anchorPointEntry = (*(*offsetPairing)->GetMaps()->begin());
			}
			ObjectPos anchorPoint{};
			for (auto find : list)
			{
				if (find.first == anchorPointEntry.first)
				{
					anchorPoint = find.second;
				}
			}
			ObjectPos vectorPoint = anchorPointEntry.second;
			ObjectPos fromAnchorPoint{ anchorPoint.x - vectorPoint.x, anchorPoint.y - vectorPoint.y };

			for (auto childAnchorPoint = (*offsetPairing)->GetMaps()->begin(); childAnchorPoint != (*offsetPairing)->GetMaps()->end(); ++childAnchorPoint)
			{
				if (childAnchorPoint->first != anchorPointEntry.first)
				{
					bool check = false;
					for (auto item : list) 
					{
						if (item.first == childAnchorPoint->first) {
							check = true;
						}
					}
					if (!check) 
					{
						list.push_back({ childAnchorPoint->first,
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
			auto fromAnchorPoint = neckOffsetBody;
			if (positionedFrame->GetMaps()->size() > 0)
			{
				auto anchorPointEntry = positionedFrame->GetMaps()->begin();
				if (anchorPointEntry == positionedFrame->GetMaps()->end())
				{
					break;
				}
				ObjectPos anchorPoint{};
				bool check = false;
				for (auto item : list) {
					if (anchorPointEntry->first == item.first)
					{
						anchorPoint = item.second;
					}
				}

				auto vectorFromPoint = anchorPointEntry->second;
				fromAnchorPoint = { anchorPoint.x - vectorFromPoint.x, anchorPoint.y - vectorFromPoint.y };
			}
			std::unordered_map<std::string, ObjectPos> maplist;
			for (auto begin = positionedFrame->GetMaps()->begin();  begin != positionedFrame->GetMaps()->end(); ++begin)
			{
				for (auto item : list) {

					if (begin->first != item.first)
					{
						maplist.insert((*begin));
					}
				}
			}
			ObjectPos vectorFromPoint{};
			if (!maplist.empty())
			{
				vectorFromPoint = maplist.begin()->second;
			}
			

			auto partOrigin = positionedFrame->GetOrigin();
			auto tempPos = ObjectPos{ fromAnchorPoint.x - partOrigin.x, fromAnchorPoint.y - partOrigin.y };
			positionedFramesList.push_back({ positionedFrame, tempPos });
		}
		auto minXPair = std::min_element(positionedFramesList.begin(),
			positionedFramesList.end(),
			[](const std::pair<SkinParts*, ObjectPos>& lhs, const std::pair<SkinParts*, ObjectPos>& rhs) {
				return lhs.second.x < rhs.second.x;
			});
		auto minX = minXPair->second.x;

		auto maxXPair = std::max_element(positionedFramesList.begin(),
			positionedFramesList.end(),
			[](const std::pair<SkinParts*, ObjectPos>& lhs, const std::pair<SkinParts*, ObjectPos>& rhs) {
				return lhs.second.x + lhs.first->GetBitmap()->GetWidth() < rhs.second.x + rhs.first->GetBitmap()->GetWidth();
			});
		auto maxX = maxXPair->second.x + maxXPair->first->GetBitmap()->GetWidth();

		auto minYPair = std::min_element(positionedFramesList.begin(),
			positionedFramesList.end(),
			[](const std::pair<SkinParts*, ObjectPos>& lhs, const std::pair<SkinParts*, ObjectPos>& rhs) {
				return lhs.second.y < rhs.second.y;
			});
		auto minY = minYPair->second.y;

		auto maxYPair = std::max_element(positionedFramesList.begin(),
			positionedFramesList.end(),
			[](const std::pair<SkinParts*, ObjectPos>& lhs, const std::pair<SkinParts*, ObjectPos>& rhs) {
				return lhs.second.y + lhs.first->GetBitmap()->GetHeight()  < rhs.second.y + rhs.first->GetBitmap()->GetHeight();
			});

		auto maxY = maxYPair->second.y + maxYPair->first->GetBitmap()->GetHeight();


		if (bodyFrame->GetMaps()->find("neck") != bodyFrame->GetMaps()->end())
		{
			int plus = (_is_prone ? 25 : 0);
			HBRUSH brush = CreateSolidBrush(RGB(255, 0, 255));
			HBRUSH brushPrev = (HBRUSH)SelectObject(_memDC, brush);
			Rectangle(_memDC, -30, -30, _info.cx + 120, _info.cy + 120);
			SelectObject(_memDC, brushPrev);
			DeleteObject(brush);
			DeleteObject(brushPrev);

			ObjectPos centerPos{ (maxX - minX) / 2, (maxY - minY) / 2 };

			ObjectPos destination{ (maxX - minX) + (2 * 2), (maxY - minY) + (2 * 2) };
			//int oldCx = _info.cx - static_cast<int>(destination.x);
			int oldCy = _info.cy - static_cast<int>(destination.y);
			//if (oldCx != 0) {
			//	//_info.x += oldCx >> 1;
			//}
			if (oldCy != 0) {
				_info.y += oldCy >> 1;
			}
			//_info.cx = static_cast<int>(destination.x);
			_info.cy = static_cast<int>(destination.y);
			UpdateRectGameObject();

			std::pair<SkinParts*, ObjectPos> lastBody;
			for (auto body : positionedFramesList)
			{
				if ((!strcmp(body.first->GetName().c_str(), "body")) || !strcmp(body.first->GetName().c_str(), "backBody") &&
					body.first->GetMaps()->find("neck") != body.first->GetMaps()->end() &&
					body.first->GetMaps()->find("navel") != body.first->GetMaps()->end()) {
					lastBody = body;
				}
			}
			//ObjectPos bodyShouldBe{ 36, 55 };
			//float tempX1 = lastBody.second.x - bodyShouldBe.x;
			//float tempY1 = lastBody.second.y - bodyShouldBe.y;
			//float tempX2 = minX;
			//float tempY2 = minY;



			//ObjectPos cropOrigin{ tempX1 - tempX2 , tempY1 - tempY2 };
			//RECT cropArea{ static_cast<int>(std::fmax(cropOrigin.x, 0)),
			//	 static_cast<int>(std::fmax(cropOrigin.y, 0)), 96, 96 };
			//ObjectPos cropOffsetFromOrigin{ cropArea.left - cropOrigin.x, cropArea.top - cropOrigin.y };

			for (auto draw : positionedFramesList)
			{
				draw.first->GetBitmap()->RenderBitmapImage(_memDC,
					static_cast<int>(std::floor(draw.second.x - minX)),
					static_cast<int>(std::floor(draw.second.y - minY)),
					static_cast<int>(draw.first->GetBitmap()->GetWidth()),
					static_cast<int>(draw.first->GetBitmap()->GetHeight()));
			}//var cropArea = new Rectangle((int)Math.Max(cropOrigin.X, 0), (int)Math.Max(cropOrigin.Y, 0), 96, 96);
			//ObjectPos Temp{ positionedFramesList}
			// Point.Subtract(Point.Subtract(body.Item2, bodyShouldBe), new Size((int)minX, (int)minY))
			if (GetFacingDirection())
			{
				int x = static_cast<int>(destination.x);
				int y = static_cast<int>(destination.y);
				StretchBlt(hdc, 0, 0, x, y, _memDC, x - 1, 0, -x, y, SRCCOPY);

				StretchBlt(_memDC, 0, 0, x, y, _memDC, x - 1, 0, -x, y, SRCCOPY);
			}
			else
			{
				int x = static_cast<int>(destination.x);
				int y = static_cast<int>(destination.y);
				StretchBlt(hdc, 0, 0, x, y, _memDC, 0, 0, x, y, SRCCOPY);
			}

			/*Rectangle(hdc,
				static_cast<int>(std::floor(_rect.left + ScrollManager::GetScrollX())),
				static_cast<int>(std::floor(_rect.top + ScrollManager::GetScrollY())),
				static_cast<int>(std::floor(_rect.right + ScrollManager::GetScrollX())),
				static_cast<int>(std::floor(_rect.bottom + ScrollManager::GetScrollY())));*/

			UpdateRectGameObject();

			auto footOriginX = lastBody.first->GetOrigin().x + (lastBody.second.x - minX);
			auto footOriginY = lastBody.first->GetOrigin().y + (lastBody.second.y - minY);
			float reduceX= _info.x - (_rect.right - footOriginX);
			float reduceY= _info.y - (_rect.bottom - footOriginY);
			if (GetFacingDirection())
			{
				auto reversOriginX = (std::abs(lastBody.first->GetOrigin().x - destination.x)) + ((lastBody.second.x - minX) * -1);
				float reduceX = _info.x - (_rect.right - reversOriginX);
				float reduceY = _info.y - (_rect.bottom - footOriginY);
				GdiTransparentBlt(hdc,
					static_cast<int>(_rect.left - reduceX + ScrollManager::GetScrollX()),
					static_cast<int>(_info.y - reduceY + ScrollManager::GetScrollY()),
					static_cast<int>(destination.x),
					static_cast<int>(destination.y),
					_memDC,
					0,
					0,
					static_cast<int>(destination.x),
					static_cast<int>(destination.y),
					RGB(255, 0, 255));
				/*Rectangle(hdc,
					static_cast<int>(_rect.left + ScrollManager::GetScrollX() + reduceX),
					static_cast<int>(_rect.top + ScrollManager::GetScrollY() + (�߿�����y)),
					static_cast<int>(_rect.left + ScrollManager::GetScrollX() + reduceX ) + 10,
					static_cast<int>(_rect.top + ScrollManager::GetScrollY() + (�߿�����y)) + 10);*/
			}
			else
			{
				GdiTransparentBlt(hdc,
					static_cast<int>(_rect.left - reduceX + ScrollManager::GetScrollX()) ,
					static_cast<int>(_info.y - reduceY + ScrollManager::GetScrollY()),
					static_cast<int>(destination.x),
					static_cast<int>(destination.y),
					_memDC,
					0,
					0,
					static_cast<int>(destination.x),
					static_cast<int>(destination.y),
					RGB(255, 0, 255));
				/*Rectangle(hdc,
					static_cast<int>(_rect.left + ScrollManager::GetScrollX() + (�߿�����x)),
					static_cast<int>(_rect.top + ScrollManager::GetScrollY() + (�߿�����y)),
					static_cast<int>(_rect.left + ScrollManager::GetScrollX() + (�߿�����x)) + 10,
					static_cast<int>(_rect.top + ScrollManager::GetScrollY() + (�߿�����y)) + 10);*/
			}
			std::wstring str;
			str.append(L"X: ").append(std::to_wstring(_info.x)).append(L"  Y : ").append(std::to_wstring(_info.y));
			TextOut(hdc, static_cast<int>(_rect.left - reduceX + ScrollManager::GetScrollX()),
				static_cast<int>(_info.y - reduceY + ScrollManager::GetScrollY()), str.c_str(), static_cast<int>(str.size()));
		}
	}
}

void Player::IsJumping()
{
	if(_is_rope) 
	{
		float outY = 0;
		_is_first_foothold = true;
		bool isFoothold = MapManager::GetInstance()->FootholdYCollision(this, &outY, &_next_foothold);
		return;
	}
	float outY = 0;
	bool isFoothold = MapManager::GetInstance()->FootholdYCollision(this, &outY, &_now_foothold);

	const int count = 17;
	const float speed = 6.f;
	if (_is_jump)
	{
		++_jump_count;
		if (_jump_count < count)
		{
			_info.y -= speed;
			//ScrollManager::GainScrollY(speed, true);
		}
		else
		{
			_is_jump = false;
			_is_fly = true;
			_jump_count = 0;
		}
	}
	else if (!_is_rope && isFoothold)
	{
		if (_info.y <= outY)
		{
			if (_info.y - outY <= speed && _info.y - outY >= -speed)
			{
				_info.y = outY;
				_is_fly = false;
				if (_is_attacking)
				{
					if (strcmp(GetFrameState(), "swingO2")) //���ݸ���̾ƴ϶�� ����ǰ
					{
						_is_attacking = false;
					}
				}
				//ScrollManager::SetScrollY(-_info.y + (768 / 2));
			}
			else
			{
				if (!_is_attacking)
				{
					this->ChangeFrameState("jump");
				}
				_is_fly = true;
				_info.y += speed;
				//ScrollManager::GainScrollY(-speed);
			}
		}
		else
		{
			_is_fly = false;
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

void Player::TryMeleeAttack()
{
	if (GetFacingDirection())
	{
		_melee_attack_hitbox = {
			static_cast<int>(_info.x + -5),
			static_cast<int>(_info.y + -50),
			static_cast<int>(_info.x + 73),
			static_cast<int>(_info.y + 17) };
	}
	else
	{
		_melee_attack_hitbox = {
			static_cast<int>(_info.x + -73),
			static_cast<int>(_info.y + -50),
			static_cast<int>(_info.x +  5),
			static_cast<int>(_info.y +  17) };
	}
	_is_attacking = true;
}

bool Player::IsAlertStateTick()
{
	auto tick = GetTickCount64();
	if (tick < _alert_tick + 5000) 
	{
		return true;
	}
	return false;
}

void Player::UpdateAlertTick()
{
	_alert_tick = GetTickCount64();
}

void Player::AttackMonster(Monster* monster)
{
	//TODO: ������������ ���⼭..ó���ϵ�������
	int32_t damage = rand();
	monster->GainHp(-damage);
	_damage_handler->InsertAttackDamageEffect(monster, damage, 1000);
}


void Player::RenderGameObject(HDC hdc)
{
	UpdateRectGameObject();
	RenderCharacter(hdc);
	_damage_handler->ShowDamages(hdc);
	
}

void Player::LateUpdateGameObject()
{
	Player::IsJumping();

	uint64_t tick = GetTickCount64();
	if (!_is_rope && tick > _frame_tick + _frame_this->GetDelay())
	{

		if (!strcmp(GetFrameState(), "stand1") || !strcmp(GetFrameState(), "swingO2") || !strcmp(GetFrameState(), "alert"))
		{
			if (_frame_revers)
			{
				if (--_frame_nummber == 0)
				{
					_frame_revers = false;
				}
			}
			else
			{
				if (++_frame_nummber >= _frame_this->GetPartner()->GetSkinFrames()->size())
				{
					_frame_revers = true;
				}
			}
			if (_is_attacking)
			{
				if (_frame_nummber >= _frame_this->GetPartner()->GetSkinFrames()->size())
				{
					this->ChangeFrameState("alert");
					auto mosnters = MapManager::GetInstance()->HitBoxMonsterCollision(_melee_attack_hitbox, 1);
					for (auto monster : mosnters)
					{
						AttackMonster(monster);
						//std::cout << "���� ������ �ǰ�!" << monster->GetHp() << std::endl;
					}
					_is_attacking = false;
				}
			}
		}
		else
		{
			++_frame_nummber;
		}
		_frame_tick = tick;
	}
}

void Player::ScrollMove()
{
	const int range = 50;

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
	_frame_this = frame;
}

SkinFrame* Player::GetFrameThis()
{
	return _frame_this;
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
	if (strcmp(GetFrameState(), frame.c_str()))
	{
		LoadCharacterFrame(frame);
		_frame_state = frame;
		_frame_nummber = 0;
		_frame_revers = false;
	}
}

const char* Player::GetFrameState() const
{
	//char findStr[30];
	//snprintf(findStr, 30, "%s/%d", _frameState.c_str(), _frameNummber);

	//return findStr;
	return _frame_state.c_str();
}

uint16_t Player::GetFrameNummber(uint16_t remain)
{
	return _frame_nummber % remain;
}

void Player::SetFacingDirection(uint8_t direction)
{
	_facing_direction = direction;
}

uint8_t Player::GetFacingDirection() const
{
	return _facing_direction;
}

FootHold* Player::GetNowFootHold()
{
	return _now_foothold;
}

void Player::SetNowFootHold(FootHold* hold)
{
	_now_foothold = hold;
}

ObjectInfo* Player::GetPlayerInfo()
{
	return &_player_info;
}