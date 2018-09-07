#pragma once

class InputEvent 
{
public:
    int PositionX;
    int PositionY;
    long long EventTime;
	constexpr static int MinXRaw = 1500;
	constexpr static int MaxXRaw = 15104;
	constexpr static int MinYRaw = 4500;
	constexpr static int MaxYRaw = 12500;
    constexpr static double RangeX = MaxXRaw - MinXRaw;
    constexpr static double RangeY = MaxYRaw - MinYRaw;

	enum FingerState { Up, Down, Hold };
    FingerState State;
    InputEvent();
    InputEvent(int, int, FingerState, long long);
};
