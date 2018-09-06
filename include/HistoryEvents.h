#pragma once
#include"stdafx.h"

class HistoryEvents
{
public:
	~HistoryEvents();
	void Load();
	void Show();

private:
	void DrawScreen();
	bool RecentClick();
	DisplayMain::GameState HandleClick(int x, int y);
	void WrapText(const std::string &, std::vector<std::string> &, int);
	int TextLength(char*, int);
	MenuItem backButton;
	long long lastClickMillis = 0;
	double counter = 0;
	int monthNumber = 0;
	int dayOfMonth = 0;
	double hueCounter = 0;
	rgb_matrix::Font largeFont;
	rgb_matrix::Font smallFont;
	std::vector<std::string> factVector;
	double factIndex = 0;
    std::map<int, std::string> numberSuffixes;
    static std::map<int, std::string> monthNames;
    static std::map<int, std::string> dayNames;

    
};