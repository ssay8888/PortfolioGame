#include "../../pch.h"
#include "effect_parts.h"

EffectParts::EffectParts(): _origin(),
                            _delay(50)
{
}

EffectParts::~EffectParts()
{
}

void EffectParts::SetImage(const std::shared_ptr<MyBitmap> image)
{
    _image = image;
}

std::shared_ptr<MyBitmap> EffectParts::GetImage() const
{
    return _image;
}

void EffectParts::SetOrigin(const POINT origin)
{
    _origin = origin;
}

POINT EffectParts::GetOrigin() const
{
    return _origin;
}

void EffectParts::SetDelay(const int32_t delay)
{
    _delay = delay;
}

int32_t EffectParts::GetDelay() const
{
    return _delay;
}
