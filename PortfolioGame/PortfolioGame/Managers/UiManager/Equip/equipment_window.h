#pragma once
#include "../Base/base_window.h"
class EquipmentWindow :
	public BaseWindow
{
public:
	EquipmentWindow();
	virtual ~EquipmentWindow();
	void EquipmentItemRender(HDC hdc) const;
	void UnEquipItem(POINT mouse);
private:
	void ReadyWindow() override;
	void UpdateWindow() override;
	void RenderWinodw(HDC hdc) override;


	static POINT FindEquipRenderPoint(ObjectType::EquipPosition type);
private:

};

