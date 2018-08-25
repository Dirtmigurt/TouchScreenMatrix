#pragma once

class InputEvent 
{
public:
    int PositionX;
    int PositionY;
	const static int MinXRaw = 2000;
	const static double RangeX  = 12000;	
	const static int MinYRaw = 6000;
	const static double RangeY  = 9000;

	enum FingerState { Up, Down, Hold };
    FingerState State;
    InputEvent();
    InputEvent(int, int, FingerState);
};