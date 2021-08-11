#include "../../../pch.h"
#include "skill.h"
#include "../../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "skill_effect_image.h"
#include "skill_info.h"

Skill::Skill() :
	_icon(nullptr),
	_icon_disable(nullptr),
	_icon_mouse_over(nullptr),
	_skill_id(0)
{
}

Skill::~Skill()
{
	delete _icon;
	delete _icon_disable;
	delete _icon_mouse_over;
	for (const auto& data : _effect_image)
		delete data;
	for (const auto& data : _hit_image)
		delete data;

	for (const auto& data : _skill_info)
		delete data;

	_effect_image.clear();
	_hit_image.clear();
	_skill_info.clear();
}

void Skill::ReadySkill()
{
	wchar_t path[150];
	swprintf_s(path, 150, L"Client\\Skill\\%d.img\\skill.%d.icon.bmp",
		(_skill_id / 10000), _skill_id);
	const auto icon = new MyBitmap();
	icon->Insert_Bitmap(_hWnd, path);
	_icon = icon;
	swprintf_s(path, 150, L"Client\\Skill\\%d.img\\skill.%d.iconDisabled.bmp",
		(_skill_id / 10000), _skill_id);
	const auto icon_disable = new MyBitmap();
	icon_disable->Insert_Bitmap(_hWnd, path);
	_icon_disable = icon_disable;

	swprintf_s(path, 150, L"Client\\Skill\\%d.img\\skill.%d.iconMouseOver.bmp",
		(_skill_id / 10000), _skill_id);
	const auto icon_mouse_over = new MyBitmap();
	icon_mouse_over->Insert_Bitmap(_hWnd, path);
	_icon_mouse_over = icon_mouse_over;
}

void Skill::InsertSkillEffect(SkillEffectImage* image)
{
	_effect_image.emplace_back(image);
}

void Skill::InsertBallEffect(SkillEffectImage* image)
{
	_ball_image.emplace_back(image);
}

std::vector<SkillEffectImage*>& Skill::GetBallEffectImage()
{
	return _ball_image;
}

void Skill::InsertHitEffect(SkillEffectImage* image)
{
	_hit_image.emplace_back(image);
}

void Skill::InsertSkillInfo(SkillInfo* info)
{
	_skill_info.emplace_back(info);
}

void Skill::SetIconImage(MyBitmap* image)
{
	_icon = image;
}

void Skill::SetIconDisableImage(MyBitmap* image)
{
	_icon_disable = image;
}

void Skill::SetIconMouseOverImage(MyBitmap* image)
{
	_icon_mouse_over = image;
}

void Skill::SetSkillId(const int32_t skill_id)
{
	_skill_id = skill_id;
}

void Skill::SetAction(const std::string action)
{
	_action = action;
}

std::vector<SkillEffectImage*>& Skill::GetSkillEffectImage()
{
	return _effect_image;
}

std::vector<SkillEffectImage*>& Skill::GetHitImage()
{
	return _hit_image;
}

std::vector<SkillInfo*>& Skill::GetSkillInfo()
{
	return _skill_info;
}

MyBitmap* Skill::GetIconImage() const
{
	return _icon;
}

MyBitmap* Skill::GetIconDisableImage() const
{
	return _icon_disable;
}

MyBitmap* Skill::GetIconMouseOverImage() const
{
	return _icon_mouse_over;
}

std::string Skill::GetAction() const
{
	return _action;
}

int32_t Skill::GetSkillId() const
{
	return _skill_id;
}
