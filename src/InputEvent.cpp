#include"../include/stdafx.h"

InputEvent::InputEvent()
{
    PositionX = 0;
    PositionY = 0;
    State = Hold;
}

InputEvent::InputEvent(int x, int y, FingerState state, long long eventTime)
{
	// Scale x
	double xPercent = (x - MinXRaw) / RangeX;
	double yPercent = (y - MinYRaw) / RangeY;
    PositionX = std::max(xPercent * DisplayMain::SCREEN_WIDTH, 0.0);
    PositionX = std::min(PositionX, DisplayMain::SCREEN_WIDTH - 1);

    PositionY = std::max(yPercent * DisplayMain::SCREEN_HEIGHT, 0.0);
    PositionY = std::min(PositionY, DisplayMain::SCREEN_HEIGHT - 1);
    State = state;
    EventTime = eventTime;
}