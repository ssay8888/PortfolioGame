#include "../../../pch.h"
#include "skill_effect_image.h"

SkillEffectImage::SkillEffectImage() :
	_delay(0),
	_origin(),
	_image(nullptr)
{
}

SkillEffectImage::~SkillEffectImage()
{
}

int32_t SkillEffectImage::GetDelay() const
{
	return _delay;
}

ObjectPos SkillEffectImage::GetOrigin() const
{
	return _origin;
}

MyBitmap* SkillEffectImage::GetImage() const
{
	return _image;
}

void SkillEffectImage::SetDelay(const int32_t delay)
{
	_delay = delay;
}

void SkillEffectImage::SetOrigin(const ObjectPos origin)
{
	_origin = origin;
}

void SkillEffectImage::SetImage(MyBitmap* image)
{
	_image = image;
}
