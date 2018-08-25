#pragma once
#include "stdafx.h"

class MainMenu
{

public:
	enum MenuResult { Nothing, Exit, Play };
	MenuResult Show(rgb_matrix::RGBMatrix*, rgb_matrix::FrameCanvas*);

private:
	MenuResult GetMenuResponse();
	MenuResult HandleClick(int x, int y);
};