#pragma once

class InputEvent 
{
public:
    int PositionX;
    int PositionY;
    long long EventTime;
	constexpr static int    MinXRaw = 0x5DC;
	constexpr static double RangeX  = 0x34BC;
	constexpr static int    MinYRaw = 0xFA0;
	constexpr static double RangeY  = 0x2134;

	enum FingerState { Up, Down, Hold };
    FingerState State;
    InputEvent();
    InputEvent(int, int, FingerState, long long);
};
