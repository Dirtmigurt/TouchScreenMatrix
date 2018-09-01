#pragma once
#include "stdafx.h"
#include "MenuItem.h"

class MainMenu
{

public:
	void Load();
	DisplayMain::GameState Show();

private:
	DisplayMain::GameState GetMenuResponse();
	DisplayMain::GameState HandleClick(int x, int y);
	std::vector<MenuItem> menuItems;
};
