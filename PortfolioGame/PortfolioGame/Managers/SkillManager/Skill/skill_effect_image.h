#pragma once
class MyBitmap;

class SkillEffectImage
{
public:
	SkillEffectImage();
	~SkillEffectImage();

	int32_t GetDelay() const;
	ObjectPos GetOrigin() const;
	MyBitmap* GetImage() const;

	void SetDelay(int32_t delay);
	void SetOrigin(ObjectPos origin);
	void SetImage(MyBitmap* image);

private:
	int32_t _delay;
	ObjectPos _origin;
	MyBitmap* _image;
};
