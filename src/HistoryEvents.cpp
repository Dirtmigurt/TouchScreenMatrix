#include "../include/stdafx.h"

std::map<int, std::string> HistoryEvents::dayNames = {{0, "Sunday"}, {1, "Monday"}, {2, "Tuesday"}, {3, "Wednesday"}, {4, "Thursday"}, {5, "Friday"}, {6, "Saturday"}};
std::map<int, std::string> HistoryEvents::monthNames = {{1, "January"}, {2, "February"}, {3, "March"}, {4, "April"}, {5, "May"}, {6, "June"}, {7, "July"}, {8, "August"}, {9, "September"}, {10, "October"}, {11, "November"}, {12, "December"}};

HistoryEvents::~HistoryEvents()
{
}

void HistoryEvents::Load()
{
	backButton.Load("../icons/Back9x9.bmp", 0, 0);
	backButton.ItemValue = DisplayMain::Exiting;

    largeFont.LoadFont("../fonts/6x12.bdf");
	smallFont.LoadFont("../fonts/tom-thumb.bdf");

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

    // Set month/day so march 18th -> 3/
    time_t t = time(nullptr);
	tm* timePtr = localtime(&t);
    monthNumber = timePtr->tm_mon + 1;
    dayOfMonth = timePtr->tm_mday;

    char filename[128];
    sprintf(filename, "../events/%d/%d.txt", monthNumber, dayOfMonth);
    std::ifstream inFile(filename);

    if (!inFile)
    {
        printf("Failed to open '%s'\n", filename);
        return;
    }

    std::string str;
    while(std::getline(inFile, str))
    {
        factVector.push_back(str);
    }

    inFile.close();
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
	char line1[256];
	char line2[256];
    std::string amPm = timePtr->tm_hour < 12 ? "AM" : "PM";
    // 9:09 PM Tuesday, April 11th 2018 
	sprintf(line1, "%d:%02d %s %s",
		hour,
		minute,
		amPm.c_str(),
		dayNames.find(timePtr->tm_wday)->second.c_str());
	sprintf(line2, "%s %d%s %d",
            monthNames.find(timePtr->tm_mon + 1)->second.c_str(), 
            timePtr->tm_mday, 
            numberSuffixes.find(timePtr->tm_mday)->second.c_str(), 
            1900 + timePtr->tm_year);

    hueCounter += 0.25;
    rgb_matrix::Color timeColor = TwelveHour::HueToRGBColor(hueCounter);
    rgb_matrix::DrawText(canvas, largeFont, (DisplayMain::SCREEN_WIDTH - TextLength(line1, 6)) / 2, largeFont.baseline(), timeColor, line1);
	rgb_matrix::DrawText(canvas, largeFont, (DisplayMain::SCREEN_WIDTH - TextLength(line2, 6)) / 2, 2 * (largeFont.baseline()), timeColor, line2);

    // Should our fact file change?
    if (dayOfMonth != timePtr->tm_mday || monthNumber != timePtr->tm_mon + 1)
    {
        factVector.clear();

        dayOfMonth = timePtr->tm_mday;
        monthNumber = timePtr->tm_mon + 1;
        char filename[128];
        sprintf(filename, "../events/%d/%d.txt", monthNumber, dayOfMonth);
        std::ifstream inFile(filename);

        if (!inFile)
        {
            printf("Failed to open '%s'\n", filename);
            return;
        }

        std::string line;
        while(std::getline(inFile, line))
        {
            factVector.push_back(line);
        }
        inFile.close();
        factIndex = 0;
    }
    // Draw the fact!
    factIndex += 1.0 / 250.0;
    factIndex = TwelveHour::DoubleMod(factIndex, factVector.size());
	std::string fact = factVector[factIndex];
    std::vector<std::string> lines;
    WrapText(fact, lines, 4);
    int y = 32;
    for(auto line : lines)
    {
        rgb_matrix::DrawText(canvas, smallFont, 0, y, timeColor, line.c_str());
        y += smallFont.height();
    }

	canvas = DisplayMain::GetWindow()->SwapOnVSync(canvas);
	DisplayMain::SetCanvas(canvas);
}

void HistoryEvents::WrapText(const std::string & txt, std::vector<std::string> &lines, int characterWidth)
{
    int i = 0;
    const int MAX_WIDTH = DisplayMain::SCREEN_WIDTH;
    int validEndOfLine = -1;
    int startOfLine = 0;
    int lineWidth = 0;
    while(i < txt.length())
    {
        lineWidth += characterWidth;
        if (txt[i] == ' ' && lineWidth <= MAX_WIDTH)
        {
            validEndOfLine = i;
        }
        else if (lineWidth >= MAX_WIDTH)
        {
            // Oh the substring is too long to draw and we never hit any spaces!
            if (validEndOfLine <= startOfLine)
            {
                validEndOfLine = i - 1;
            }

            // Oh no the substring of [StartOfLine:i] is too long to draw, so only draw [StartOfLine:validEndOfLine]
            lines.push_back(txt.substr(startOfLine, validEndOfLine - startOfLine));
            i = validEndOfLine + 1;
            startOfLine = validEndOfLine + 1;
            lineWidth = 0;
        }

        i++;
    }

    lines.push_back(txt.substr(startOfLine, i - startOfLine));
}

int HistoryEvents::TextLength(char* str, int characterWidth)
{
    return strlen(str) * characterWidth;
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