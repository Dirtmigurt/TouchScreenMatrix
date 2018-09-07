#include "../include/stdafx.h"

void BinaryClock::Load()
{
    backButton.Load("../icons/Back9x9.bmp", 0, 0);
	backButton.ItemValue = DisplayMain::Exiting;
    legendFont.LoadFont("../fonts/6x9.bdf");
}

void BinaryClock::Show()
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

void BinaryClock::DrawScreen()
{
    rgb_matrix::FrameCanvas*  canvas = DisplayMain::GetCanvas();
	canvas->Clear();

	// Draw the back button
    bool drawLegend = false;
	if (RecentClick())
	{
		backButton.Draw(canvas);
        drawLegend = true;
	}

    // Draw the binary clock
    time_t t = time(nullptr);
	tm* timePtr = localtime(&t);
	int hour = timePtr->tm_hour;
	int minute = timePtr->tm_min;
    int second = timePtr->tm_sec;
    hueCounter += 0.25;
    rgb_matrix::Color timeColor = TwelveHour::HueToRGBColor(hueCounter);
    int x = 20;
    int y = 4;
    DrawBinaryColumn(canvas, hour / 10, timeColor, x, y);
    x += 12;
    DrawBinaryColumn(canvas, hour % 10, timeColor, x, y);
    x += 24;
    DrawBinaryColumn(canvas, minute / 10, timeColor, x, y);
    x += 12;
    DrawBinaryColumn(canvas, minute % 10, timeColor, x, y);
    x += 24;
    DrawBinaryColumn(canvas, second / 10, timeColor, x, y);
    x += 12;
    DrawBinaryColumn(canvas, second % 10, timeColor, x, y);

    if (drawLegend)
    {
        // Draw 8/4/2/1
        int baseX = 14;
        int baseY = 11;
        std::vector<std::string> strings = {"8", "4", "2", "1"};
        for(auto str : strings)
        {
            rgb_matrix::DrawText(canvas, legendFont, baseX, baseY, timeColor, str.c_str());
            baseY += 12;
        }

        baseX = 22;
        baseY = 57;
        strings = {"H", "H", ":", "M", "M", ":", "S", "S"};
        for(auto str : strings)
        {
            rgb_matrix::DrawText(canvas, legendFont, baseX, baseY, timeColor, str.c_str());
            baseX += 12;
        }
    }


    canvas = DisplayMain::GetWindow()->SwapOnVSync(canvas);
	DisplayMain::SetCanvas(canvas);
}

// X,Y is the TOP left coord of the column to draw
void BinaryClock::DrawBinaryColumn(rgb_matrix::FrameCanvas* canvas, int n, rgb_matrix::Color color, int x, int y)
{
    for(int i = 3; i >= 0; i--)
    {
        // Draw a 1
        if (n & (1 << i))
        {
            DrawSquare(canvas, color, x, y, 9);
        }
        // Draw a 0
        else
        {
            DrawSquare(canvas, rgb_matrix::Color(0, 0, 0), x, y, 9);
        }

        y += 3;
    }
}

void BinaryClock::DrawSquare(rgb_matrix::FrameCanvas* canvas, rgb_matrix::Color color, int& x, int& y, int n)
{
    int finalY = y + n;
    for(y; y < finalY; y++)
    {
        rgb_matrix::DrawLine(canvas, x, y, x+n, y, color);
    }
}

DisplayMain::GameState BinaryClock::HandleClick(int x, int y)
{
	// Loop through each menu option to determine if it was clicked
	if (backButton.IsClicked(x, y))
	{
		return backButton.ItemValue;
	}

	auto now = std::chrono::system_clock::now();
	lastClickMillis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

	return DisplayMain::Noop;
}

bool BinaryClock::RecentClick()
{
	auto now = std::chrono::system_clock::now();
	long long nowMillis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

	// A click was recent if it occurred within the last 10 seconds.
	return (nowMillis - lastClickMillis) < 10000;
}