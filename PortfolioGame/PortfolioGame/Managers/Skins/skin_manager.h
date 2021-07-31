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
	size_t GetSize();

	uint32_t FindPosition(std::string z);
	 
private:
	void LoadBase();
private:
	std::map<std::string, SkinInfo*> _skins;
	std::list<std::pair<std::string, std::string>> _smap;
	std::list<std::string> _zmap;
};