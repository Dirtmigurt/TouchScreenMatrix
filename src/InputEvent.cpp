#include"../include/stdafx.h"

InputEvent::InputEvent()
{
    PositionX = 0;
    PositionY = 0;
    State = Hold;
}

InputEvent::InputEvent(int x, int y, FingerState state)
{
	// Scale x
	double xPercent = (x - MinXRaw) / RangeX;
	double yPercent = (y - MinYRaw) / RangeY;
    PositionX = xPercent * DisplayMain::SCREEN_WIDTH;
    PositionY = yPercent * DisplayMain::SCREEN_HEIGHT;
    State = state;
}