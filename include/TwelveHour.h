#pragma once
#include"stdafx.h"

class TwelveHour
{
public:
	~TwelveHour();
	void Load();
	void Show();

private:
	void DrawScreen();
	void DrawClockText(rgb_matrix::FrameCanvas*, int, int, rgb_matrix::Color, char*);
	bool RecentClick();
	DisplayMain::GameState HandleClick(int x, int y);
	MenuItem backButton;
	rgb_matrix::Font timeFont;
	int redCounter = 0;
	int greenCounter = 0;
	int blueCounter = 0;
	long long lastClickMillis = 0;
	std::map<int, VisibleObject*> clockFont;

};
