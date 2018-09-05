#include "../include/stdafx.h"

std::map<int, std::string> HistoryEvents::dayNames = {{0, "Sunday"}, {1, "Monday"}, {2, "Tuesday"}, {3, "Wednesday"}, {4, "Thursday"}, {5, "Friday"}, {6, "Saturday"}};
std::map<int, std::string> HistoryEvents::monthNames = {{1, "January"}, {2, "February"}, {3, "March"}, {4, "April"}, {5, "May"}, {6, "June"}, {7, "July"}, {8, "August"}, {9, "September"}, {10, "October"}, {11, "November"}, {12, "December"}};
void HistoryEvents::Load()
{
	backButton.Load("../icons/Back9x9.bmp", 0, 0);
	backButton.ItemValue = DisplayMain::Exiting;

    font.LoadFont("../fonts/9x15B.bdf");

    for(int i = 1; i <= 31; i++)
    {
        int tensPlace = i / 10;
        int onesPlace = i % 10;
        if (onesPlace == 1 && tensPlace != 1)
        {
            // add {i, "st"}
            numberSuffixes.insert(std::pair<int, std::string>(i, "st"));
        }
        else if (onesPlace == 2 && tensPlace != 1)
        {
            // add {i, "nd"}
            numberSuffixes.insert(std::pair<int, std::string>(i, "nd"));
        }
        else if (onesPlace == 3 && tensPlace != 1)
        {
            // add {i, "rd"}
            numberSuffixes.insert(std::pair<int, std::string>(i, "rd"));
        }
        else
        {
            // add {i, "th"}
            numberSuffixes.insert(std::pair<int, std::string>(i, "th"));
        }
    }
}

// Should only return when HistoryEvents is done being shown.
void HistoryEvents::Show()
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

void HistoryEvents::DrawScreen()
{
	rgb_matrix::FrameCanvas*  canvas = DisplayMain::GetCanvas();
	canvas->Clear();

	// Draw the back button
	if (RecentClick())
	{
		backButton.Draw(canvas);
	}

    // Draw the time
    time_t t = time(nullptr);
	tm* timePtr = localtime(&t);
	int hour = timePtr->tm_hour;
    hour = hour % 12;
    hour = hour == 0 ? 12 : hour;
	int minute = timePtr->tm_min;
	char buffer[1024];
    std::string amPm = timePtr->tm_hour < 12 ? "AM" : "PM";
    // 9:09 PM Tuesday, April 11th 2018 
	sprintf(buffer, "%d:%02d %s %s, %s %d%s %d", 
            hour, 
            minute, 
            amPm, 
            dayNames.find(timePtr->tm_wday)->second, 
            monthNames.find(timePtr->tm_mon + 1)->second, 
            timePtr->tm_mday, 
            numberSuffixes.find(timePtr->tm_mday)->second, 
            1900 + timePtr->tm_year);

    rgb_matrix::Color timeColor(255, 255, 255);
    rgb_matrix::DrawText(canvas, font, 12, 20, timeColor, buffer);

    // Draw the fact!
    rgb_matrix::DrawText(canvas, font, 12, 40, timeColor, "1999 Sample fact from the year 1999 on this day! It can be long though so it may need to be line wrapped.");

	canvas = DisplayMain::GetWindow()->SwapOnVSync(canvas);
	DisplayMain::SetCanvas(canvas);
}

DisplayMain::GameState HistoryEvents::HandleClick(int x, int y)
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

bool HistoryEvents::RecentClick()
{
	auto now = std::chrono::system_clock::now();
	long long nowMillis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

	// A click was recent if it occurred within the last 10 seconds.
	return (nowMillis - lastClickMillis) < 10000;
}