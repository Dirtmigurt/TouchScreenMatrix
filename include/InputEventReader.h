#pragma once
#include<queue>
#include<mutex>
#include<InputEvent.h>

class InputEventReader
{
public:
	static void Start();
	static InputEvent* GetEvent();
private:
	static bool IsExiting();
	static void ReadLoop();
	enum ReaderState { Uninitialized, Reading, Exiting };
	static ReaderState readerState;
	static std::mutex eventListMutex;
	static std::queue<InputEvent*> inputEventQueue;
};