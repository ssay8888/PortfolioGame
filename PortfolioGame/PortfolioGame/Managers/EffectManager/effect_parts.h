#pragma once
class MyBitmap;

class EffectParts
{
public:
	EffectParts();
	~EffectParts();

	void SetImage(std::shared_ptr<MyBitmap> image);
	std::shared_ptr<MyBitmap> GetImage() const;
	void SetOrigin(POINT origin);
	POINT GetOrigin() const;
	void SetDelay(int32_t delay);
	int32_t GetDelay() const;
private:
	std::shared_ptr<MyBitmap> _image;
	POINT _origin;
	int32_t _delay;
};

