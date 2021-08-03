#include <vector>
#include <memory>
#include <map>

class SkinInfo;
class SkinItem;
class SkinManager
{
private:
	SkinManager();
	~SkinManager() = default;
public:
	static SkinManager* GetInstance()
	{
		static SkinManager instance;
		return &instance;
	}
	void LoadSkin();
	void InsertBodySkin(SkinInfo* skin);
	void InsertHeadSkin(SkinInfo* skin);
	SkinInfo* GetBodySkinInfo(std::string key) const;
	SkinInfo* GetHeadSkinInfo(std::string key) const;
	size_t GetSize();

	uint32_t FindPosition(std::string z);

private:
	void LoadBase();
	void UolSetting(uint16_t skinId, std::map<std::string, SkinInfo*> info, SkinItem* skinitem);
private:
	std::map<std::string, SkinInfo*> _bodySkins;
	std::map<std::string, SkinInfo*> _headSkins;
	std::list<std::pair<std::string, std::string>> _smap;
	std::list<std::string> _zmap;
};