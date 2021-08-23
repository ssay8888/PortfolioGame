#pragma once
namespace ObjectType
{
	enum class ObjectType : uint16_t
	{
		kPlayer,
		kEnd
	};
	enum class InventoryTabState { kEqp, kConsume, kInstall, kEtc, kCash, kEnd };
	enum EquipPosition { kCap, kCoat, kPants, kShoes, kWeapon, kEquipEnd };
	enum class NpcTalkType { kNext, kOk, kQYesNo };

	enum class BuffFlag
	{
		kEnd = 0,
		kMagicGuard = 0x1,
		kMeditation = 0x2,
		kinfinity = 0x4,
	};

}