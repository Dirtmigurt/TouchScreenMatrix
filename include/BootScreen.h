#pragma once
#include "stdafx.h"
#include "VisibleObject.h"

class BootScreen
{
public:
	void Load();
	void Show(rgb_matrix::RGBMatrix*, rgb_matrix::FrameCanvas*);
	VisibleObject bootImage;
};
