#pragma once
#include"stdafx.h"

class TwelveHour
{
public:
	~TwelveHour();
	void Load();
	void Show();
	bool ShowTwelve = true;

private:
	void DrawScreen();
	void DrawClockText(rgb_matrix::FrameCanvas*, rgb_matrix::Color, char*);
	bool RecentClick();
	DisplayMain::GameState HandleClick(int x, int y);
	static rgb_matrix::Color HueToRGBColor(double& hue);
	static double DoubleMod(double x, double y);
	MenuItem backButton;
	rgb_matrix::Font timeFont;
	double hueCounter = 0;
	long long lastClickMillis = 0;
	std::map<int, VisibleObject*> clockFont;

};
