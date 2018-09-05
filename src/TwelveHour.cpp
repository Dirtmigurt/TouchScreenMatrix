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
	int hour = timePtr->tm_hour;
	if (ShowTwelve)
	{
		hour = hour % 12;
		hour = hour == 0 ? 12 : hour;
	}

	int minute = timePtr->tm_min;
	char buffer[10];
	sprintf(buffer, "%d:%02d", hour, minute);
	hueCounter += 0.25;
	DrawClockText(canvas, HueToRGBColor(hueCounter), buffer);

	canvas = DisplayMain::GetWindow()->SwapOnVSync(canvas);
	DisplayMain::SetCanvas(canvas);
}

void TwelveHour::DrawClockText(rgb_matrix::FrameCanvas* canvas, rgb_matrix::Color color, char * text)
{
	int i = 0;
	int x = 0;
	int y = 0;
	int width = 0;
	while (text[i] != '\0')
	{
		width += clockFont.find(text[i])->second->GetWidth();
		y = std::max(y, static_cast<int>(clockFont.find(text[i])->second->GetHeight()));
		i++;
	}

	width += 2 * (strlen(text) - 1);
	x = (DisplayMain::SCREEN_WIDTH - width) / 2;
	y = (DisplayMain::SCREEN_HEIGHT - y) / 2;

	i = 0;
	while(text[i] != '\0')
	{
		// draw the char text[i] at (x,y) with the given color
		VisibleObject * obj = clockFont.find(text[i])->second;

		if (text[i] == ':')
		{
			obj->SetPosition(x, y + 10);
		}
		else
		{
			obj->SetPosition(x, y);
		}
		
		obj->Draw(canvas, color);
		x += obj->GetWidth() + 2;
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

rgb_matrix::Color TwelveHour::HueToRGBColor(double& hue)
{
	hue = DoubleMod(hue, 360.0);
	double x = ((DoubleMod(hue, 60.0)) / 60.0) * 255.0;
	rgb_matrix::Color returnColor(0, 0, 0);
	if (hue >= 0 && hue < 60)
	{
		returnColor.r = 255;
		returnColor.g = static_cast<int>(x);
		returnColor.b = 0;
	}
	else if (hue >= 60 && hue < 120)
	{
		returnColor.r = 255 - static_cast<int>(x);
		returnColor.g = 255;
		returnColor.b = 0;
	}
	else if (hue >= 120 && hue < 180)
	{
		returnColor.r = 0;
		returnColor.g = 255;
		returnColor.b = x;
	}
	else if (hue >= 180 && hue < 240)
	{
		returnColor.r = 0;
		returnColor.g = 255 - static_cast<int>(x);
		returnColor.b = 255;
	}
	else if (hue >= 240 && hue < 300)
	{
		returnColor.r = x;
		returnColor.g = 0;
		returnColor.b = 255;
	}
	else if (hue >= 300 && hue < 360)
	{
		returnColor.r = 255;
		returnColor.g = 0;
		returnColor.b = 255 - static_cast<int>(x);
	}

	return returnColor;
}

double TwelveHour::DoubleMod(double x, double y)
{
	int result = static_cast<int>(x / y);
	return x - static_cast<double>(result) * y;
}
