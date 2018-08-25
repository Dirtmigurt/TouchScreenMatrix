#pragma once

class InputEvent 
{
public:
    int PositionX;
    int PositionY;
	constexpr static int MinXRaw = 2000;
	constexpr static double RangeX  = 12000;
	constexpr static int MinYRaw = 6000;
	constexpr static double RangeY  = 9000;

	enum FingerState { Up, Down, Hold };
    FingerState State;
    InputEvent();
    InputEvent(int, int, FingerState);
};
