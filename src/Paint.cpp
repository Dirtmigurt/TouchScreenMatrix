#include"../include/stdafx.h"

Paint::~Paint()
{
    for(auto obj : paletteObjects)
    {
        delete obj;
    }
}

void Paint::Load()
{
	backButton.Load("../icons/Back9x9.bmp", 0, 0);
	backButton.ItemValue = DisplayMain::Exiting;

	// set up palette objects
    int xPosition = 9;
    int yPosition = 0;
	for (double i = 0; i < 360; i += 36)
	{
        // Create a palette object where color = HuetoRGB(i);
        MenuItem * obj = new MenuItem();
        obj->Load(TwelveHour::HueToRGBColor(i), 9, 9);
        if (xPosition > DisplayMain::SCREEN_WIDTH - 10)
        {
            xPosition = 0;
            yPosition += 9;
        }
        else
        {
            xPosition += 9;
        }

        obj->SetPosition(xPosition, yPosition);

        paletteObjects.push_back(obj);
	}
}

// Should only return when TwelveHour is done being shown.
void Paint::Show()
{
    // Set up paint canvas
    rgb_matrix::FrameCanvas*  canvas = DisplayMain::GetCanvas();
	canvas->Clear();
    backButton.Draw(canvas);
    for(auto obj : paletteObjects)
    {
        obj->Draw(canvas);
    }

    rgb_matrix::DrawLine(canvas, 0, 10, 128, 10, rgb_matrix::Color(255, 255, 255));
    canvas = DisplayMain::GetWindow()->SwapOnVSync(canvas);
	DisplayMain::SetCanvas(canvas);

	while (true)
	{
		// Check if back button was pressed
		std::unique_ptr<InputEvent> currentEvent(InputEventReader::GetEvent());
		if (currentEvent != nullptr && currentEvent->State == InputEvent::Down)
		{
			if (HandleDownClick(currentEvent->PositionX, currentEvent->PositionY) == DisplayMain::Exiting)
			{
				return;
			}
		}
        else if (currentEvent != nullptr && currentEvent->State == InputEvent::Hold)
        {
            UserDraw(currentEvent->PositionX, currentEvent->PositionY);
        }
	}
}

DisplayMain::GameState Paint::HandleDownClick(int x, int y)
{
	// Loop through each menu option to determine if it was clicked
	if (backButton.IsClicked(x, y))
	{
		return backButton.ItemValue;
	}

    for(auto obj : paletteObjects)
    {
        if (obj->IsClicked(x, y))
        {
            drawColor = obj->GetPixel(0, 0);
            break;
        }
    }

    // No menu items were click, user was trying to draw on the palette
    UserDraw(x, y);

	return DisplayMain::Noop;
}

void Paint::UserDraw(int x, int y)
{
    // Draw on the current canvas, no swapping
    rgb_matrix::RGBMatrix* matrix = DisplayMain::GetWindow();

    for(int i = -1; i <= 1; i++)
    {
        for(int j = -1; j <= 1; j++)
        {
            if (y > 10)
            {
                matrix->SetPixel(x + j, y + i, drawColor.r, drawColor.g, drawColor.b);
            }
        }
    }
}
