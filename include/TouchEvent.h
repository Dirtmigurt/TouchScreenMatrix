#ifndef TOUCHEVENT_H
#define TOUCHEVENT_H

#include<vector>
#include<linux/input.h>
#include<FingerState.h>

class TouchEvent 
{
    public:
        int PositionX;
        int PositionY;
        FingerState State;
        TouchEvent();
        TouchEvent(int, int, FingerState);
};

#endif