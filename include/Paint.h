#pragma once
#include"stdafx.h"

class Paint
{
public:
    ~Paint();
	void Load();
	void Show();

private:
	void DrawScreen();
	DisplayMain::GameState HandleDownClick(int, int);
    void UserDraw(int, int);
	MenuItem backButton;
    rgb_matrix::Color drawColor = rgb_matrix::Color(255, 0, 0);
    std::vector<MenuItem*> paletteObjects;
};