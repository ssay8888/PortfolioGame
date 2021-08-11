#pragma once
class MyBitmap;
class SkillInfo;
class SkillEffectImage;
class Skill
{
public:
	Skill();
	~Skill();

	void ReadySkill();

	void InsertSkillEffect(SkillEffectImage* image);
	void InsertBallEffect(SkillEffectImage* image);
	void InsertHitEffect(SkillEffectImage* image);
	void InsertSkillInfo(SkillInfo* info);
	void SetIconImage(MyBitmap* image);
	void SetIconDisableImage(MyBitmap* image);
	void SetIconMouseOverImage(MyBitmap* image);
	void SetSkillId(int32_t skill_id);
	void SetAction(std::string action);

	std::vector<SkillEffectImage*>& GetSkillEffectImage();
	std::vector<SkillEffectImage*>& GetBallEffectImage();
	std::vector<SkillEffectImage*>& GetHitImage();
	std::vector<SkillInfo*>& GetSkillInfo();
	MyBitmap* GetIconImage() const;
	MyBitmap* GetIconDisableImage() const;
	MyBitmap* GetIconMouseOverImage() const;
	std::string GetAction() const;
	int32_t GetSkillId() const;

private:
	std::vector<SkillEffectImage*> _effect_image;
	std::vector<SkillEffectImage*> _ball_image;
	std::vector<SkillEffectImage*> _hit_image;
	std::vector<SkillInfo*> _skill_info; // 레벨별 능력치가 들어감.
	MyBitmap* _icon;
	MyBitmap* _icon_disable;
	MyBitmap* _icon_mouse_over;
	std::string _action;
	int32_t _skill_id;
};

