#pragma once

class InputEvent 
{
public:
    int PositionX;
    int PositionY;
	constexpr static int    MinXRaw = 0x00;
	constexpr static double RangeX  = 0x4000;
	constexpr static int    MinYRaw = 0x00;
	constexpr static double RangeY  = 0x4000;

	enum FingerState { Up, Down, Hold };
    FingerState State;
    InputEvent();
    InputEvent(int, int, FingerState);
};
