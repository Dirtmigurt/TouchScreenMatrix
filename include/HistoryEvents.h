#pragma once
#include"stdafx.h"

class HistoryEvents
{
public:
	void Load();
	void Show();

private:
	void DrawScreen();
	bool RecentClick();
	DisplayMain::GameState HandleClick(int x, int y);
	MenuItem backButton;
	long long lastClickMillis = 0;
    rgb_matrix::Font font;
    std::map<int, std::string> numberSuffixes;
    static std::map<int, std::string> monthNames;
    static std::map<int, std::string> dayNames;
    
};