#pragma once

class InputEvent 
{
public:
    int PositionX;
    int PositionY;
	enum FingerState { Up, Down, Hold };
    FingerState State;
    InputEvent();
    InputEvent(int, int, FingerState);
};