#pragma once
#include<queue>
#include<mutex>
#include<InputEvent.h>

class InputEventReader
{
public:
	static void Start();
	static InputEvent* GetEvent();
	static void Kill();
private:
	static bool IsExiting();
	static void ReadLoop();
	static InputEvent* ReadEvent(int);
	enum ReaderState { Uninitialized, Reading, Exiting };
	static ReaderState readerState;
	static std::mutex eventListMutex;
	static std::queue<InputEvent*> inputEventQueue;
};