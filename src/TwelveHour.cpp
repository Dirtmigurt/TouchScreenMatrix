#include"../include/stdafx.h"

TwelveHour::~TwelveHour()
{
	for(auto pair : clockFont)
	{
		delete pair.second;
	}
}

void TwelveHour::Load()
{
	backButton.Load("../icons/Back9x9.bmp", 0, 0);
	backButton.ItemValue = DisplayMain::Exiting;

	// set up visible object for 0, 1, 2, ... 9 + :

	for (int i = 0; i < 10; i++)
	{
		char filename[256];
		sprintf(filename, "../clockImages/%d.bmp", i);
		VisibleObject* obj = new VisibleObject();
		obj->Load(filename, 0, 0);
		clockFont.insert(std::pair<int, VisibleObject*>(i + '0', obj));
	}

	VisibleObject* obj = new VisibleObject();
	obj->Load("../clockImages/Colon.bmp", 0, 0);
	clockFont.insert(std::pair<int, VisibleObject*>(':', obj));
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
	if (RecentClick())
	{
		backButton.Draw(canvas);
	}

	// Draw the time
	time_t t = time(nullptr);
	tm* timePtr = localtime(&t);
	int hour = timePtr->tm_hour % 12;
	hour = hour == 0 ? 12 : hour;
	int minute = timePtr->tm_min;
	char buffer[10];
	sprintf(buffer, "%d:%02d", hour, minute);
	redCounter = (redCounter + 1) % 360;
	greenCounter = (greenCounter + 3) % 360;
	blueCounter = (blueCounter + 5) % 360;
	int red = 255 * (cos(redCounter * 0.017453) + 1) / 2;
	int green = 255 * (cos(greenCounter * 0.017453) + 1) / 2;
	int blue = 255 * (cos(blueCounter * 0.017453) + 1) / 2;
	int max = std::max(red, green);
	max = std::max(max, blue);
	double scale = 255.0 / std::max(1, max);
	DrawClockText(canvas, 0, 45, rgb_matrix::Color(red * scale, green * scale, blue * scale), buffer);

	canvas = DisplayMain::GetWindow()->SwapOnVSync(canvas);
	DisplayMain::SetCanvas(canvas);
}

void TwelveHour::DrawClockText(rgb_matrix::FrameCanvas* canvas, int x, int y, rgb_matrix::Color color, char * text)
{
	int i = 0;
	while(text[i] != '\0')
	{
		printf("printing char %c\n", text[i]);
		// draw the char text[i] at (x,y) with the given color
		VisibleObject * obj = clockFont.find(text[i])->second;

		obj->SetPosition(x, y);
		obj->Draw(canvas, color);
		x += obj->GetWidth() + 4;
		i++;
	}
}

DisplayMain::GameState TwelveHour::HandleClick(int x, int y)
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

bool TwelveHour::RecentClick()
{
	auto now = std::chrono::system_clock::now();
	long long nowMillis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

	// A click was recent if it occurred within the last 10 seconds.
	return (nowMillis - lastClickMillis) < 10000;
}
