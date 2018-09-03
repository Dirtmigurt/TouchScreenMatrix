#include"../include/stdafx.h"

void TwelveHour::Load()
{
	backButton.Load("../icons/Back9x9.bmp", 0, 0);
	backButton.ItemValue = DisplayMain::Exiting;
	timeFont.LoadFont("../fonts/9x18B.bdf");
}

// Should only return when TwelveHour is done being shown.
void TwelveHour::Show()
{
	while (true)
	{
		// Check if back button was pressed
		std::unique_ptr<InputEvent> currentEvent(InputEventReader::GetEvent());
		if (currentEvent != nullptr && currentEvent->State == InputEvent::Down)
		{
			if (HandleClick(currentEvent->PositionX, currentEvent->PositionY) == DisplayMain::Exiting)
			{
				return;
			}
		}

		// Draw the screen
		DrawScreen();

		std::this_thread::sleep_for(std::chrono::milliseconds(250));
	}
}

void TwelveHour::DrawScreen()
{
	rgb_matrix::FrameCanvas*  canvas = DisplayMain::GetCanvas();
	canvas->Clear();

	// Draw the back button
	backButton.Draw(canvas);

	// Draw the time
	time_t t = time(nullptr);
	tm* timePtr = localtime(&t);
	int hour = timePtr->tm_hour % 12;
	hour = hour == 0 ? 12 : hour;
	int minute = timePtr->tm_min;
	char buffer[10];
	sprintf(buffer, "%d:%d", hour, minute);
	redCounter += 1;
	greenCounter += 3;
	blueCounter += 5;
	int red = 255 * (cos(redCounter / 0.017453) + 1) / 2;
	int green = 255 * (cos(redCounter / 0.017453) + 1) / 2;
	int blue = 255 * (cos(redCounter / 0.017453) + 1) / 2;
	rgb_matrix::DrawText(canvas, timeFont, 21, 21, rgb_matrix::Color(red, green, blue), buffer);

	canvas = DisplayMain::GetWindow()->SwapOnVSync(canvas);
	DisplayMain::SetCanvas(canvas);
}

DisplayMain::GameState TwelveHour::HandleClick(int x, int y)
{
	// Loop through each menu option to determine if it was clicked
	if (backButton.IsClicked(x, y))
	{
		return backButton.ItemValue;
	}

	return DisplayMain::Noop;
}
