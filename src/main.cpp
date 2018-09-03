#include"../include/stdafx.h"

int main(int argc, char * argv[])
{
	// Spawn thread to read input events
	std::thread inputThread (InputEventReader::Start);
	std::thread displayThread (DisplayMain::Start);
	// while (true)
	// {
	// 	InputEvent* ev = nullptr; //InputEventReader::GetEvent();
	// 	std::this_thread::sleep_for(std::chrono::milliseconds(10000));
	// 	if (ev != nullptr)
	// 	{
			
	// 		delete ev;
	// 	}
	// }

	inputThread.join();
	displayThread.join();
	return 0;
}
