#include"../include/stdafx.h"

int main(int argc, char * argv[])
{
	// Spawn thread to read input events
	std::thread inputThread (InputEventReader::Start);
	std::thread displayThread (DisplayMain::Start);
	while (true)
	{
		InputEvent* ev = nullptr; // InputEventReader::GetEvent();

		if (ev != nullptr)
		{
			printf("X = %d, Y = %d, Type = %d\n", ev->PositionX, ev->PositionY, ev->State);
			delete ev;
		}
	}

	inputThread.join();
	displayThread.join();
	return 0;
}
