#pragma once
namespace ObjectType
{
	enum class ObjectType : uint16_t
	{
		kPlayer,
		kEnd
	};
	enum InventoryTabState { kEqp, kConsume, kInstall, kEtc, kCash, kEnd };
	enum EquipPosition { kCap, kCoat, kPants, kShoes, kWeapon, kEquipEnd };
	enum class NpcTalkType { kNext, kOk, kQYesNo };
}