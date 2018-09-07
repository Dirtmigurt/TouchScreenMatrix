#pragma once
#include"stdafx.h"

class BinaryClock
{
public:
	void Load();
	void Show();

private:
	void DrawScreen();
	bool RecentClick();
	DisplayMain::GameState HandleClick(int x, int y);
    void DrawBinaryColumn(rgb_matrix::FrameCanvas*, int, rgb_matrix::Color, int, int);
    void DrawSquare(rgb_matrix::FrameCanvas*, rgb_matrix::Color, int&, int&, int);
	MenuItem backButton;
	long long lastClickMillis = 0;
	double hueCounter = 0;
    rgb_matrix::Font legendFont;
};