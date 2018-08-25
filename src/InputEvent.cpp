#include<stdafx.h>

InputEvent::InputEvent()
{
    PositionX = 0;
    PositionY = 0;
    State = Hold;
}

InputEvent::InputEvent(int x, int y, FingerState state)
{

    PositionX = x;
    PositionY = y;
    State = state;
}
