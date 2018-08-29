#pragma once
#include "stdafx.h"

class MenuItem :
	public VisibleObject
{
public:
	void Update(double elapsedTime) override;
	bool IsClicked(int x, int y);
	DisplayMain::GameState ItemValue;
};

