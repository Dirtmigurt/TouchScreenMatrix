#pragma once
#include "stdafx.h"
#include "MenuItem.h"

class MainMenu
{

public:
	void Load();
	DisplayMain::GameState Show(rgb_matrix::RGBMatrix*, rgb_matrix::FrameCanvas*);

private:
	DisplayMain::GameState GetMenuResponse();
	DisplayMain::GameState HandleClick(int x, int y);
	std::vector<MenuItem> menuItems;
};
