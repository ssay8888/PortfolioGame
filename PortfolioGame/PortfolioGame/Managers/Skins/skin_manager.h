#include <vector>
#include <memory>
#include <map>

class SkinInfo;
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
	void AddSkin(SkinInfo* skin);
	SkinInfo* GetSkinInfo(std::string key) const;
	size_t GetSize() { return _skins.size(); }
	 
private:
	std::map<std::string, SkinInfo*> _skins;
};