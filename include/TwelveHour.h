#pragma once
#include"stdafx.h"

class TwelveHour
{
public:
	void Load();
	void Show();

private:
	void DrawScreen();
	bool RecentClick();
	DisplayMain::GameState HandleClick(int x, int y);
	MenuItem backButton;
	rgb_matrix::Font timeFont;
	int redCounter = 0;
	int greenCounter = 0;
	int blueCounter = 0;
	long long lastClickMillis = 0;
};
