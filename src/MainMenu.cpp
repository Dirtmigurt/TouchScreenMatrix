#include"../include/stdafx.h"

void MainMenu::Load()
{
	// create 12h menu item
	MenuItem twelveHour;
	twelveHour.ItemValue = DisplayMain::Showing12H;
	twelveHour.Load("../icons/12H.bmp", 10, 11);
	menuItems.push_back(twelveHour);
	printf("loaded ../icons/12H.bmp\n");

	// create 24h menu item
	MenuItem twentyfourHour;
	twentyfourHour.ItemValue = DisplayMain::Showing24H;
	twentyfourHour.Load("../icons/24H.bmp", 30, 11);
	menuItems.push_back(twentyfourHour);

	// create 3d menu item
	MenuItem threeD;
	threeD.ItemValue = DisplayMain::Showing3D;
	threeD.Load("../icons/3D.bmp", 50, 11);
	menuItems.push_back(threeD);

	// create binary menu item
	MenuItem binary;
	binary.ItemValue = DisplayMain::ShowingBinary;
	binary.Load("../icons/Binary.bmp", 70, 11);
	menuItems.push_back(binary);

	// create dateTime
	MenuItem date;
	date.ItemValue = DisplayMain::ShowingDate;
	date.Load("../icons/DateTime.bmp", 90, 11);
	menuItems.push_back(date);

	// create history
	MenuItem history;
	history.ItemValue = DisplayMain::ShowingEvents;
	history.Load("../icons/History.bmp", 10, 37);
	menuItems.push_back(history);

	// create image viewer
	MenuItem imageViewer;
	imageViewer.ItemValue = DisplayMain::ShowingImageViewer;
	imageViewer.Load("../icons/ImageViewer.bmp", 30, 37);
	menuItems.push_back(imageViewer);

	// create paint
	MenuItem paint;
	paint.ItemValue = DisplayMain::ShowingPaint;
	paint.Load("../icons/Paint.bmp", 50, 37);
	menuItems.push_back(paint);

	// create progress
	MenuItem progress;
	progress.ItemValue = DisplayMain::ShowingProgress;
	progress.Load("../icons/Progress.bmp", 70, 37);
	menuItems.push_back(progress);

	// create config
	MenuItem config;
	config.ItemValue = DisplayMain::ShowingConfig;
	config.Load("../icons/Config.bmp", DisplayMain::SCREEN_WIDTH - 15 - 1, DisplayMain::SCREEN_HEIGHT - 15 - 1);
	menuItems.push_back(config);

	// create exit
	MenuItem exit;
	exit.ItemValue = DisplayMain::Exiting;
	exit.Load("../icons/Exit.bmp", DisplayMain::SCREEN_WIDTH - 15 - 1, 0);
	menuItems.push_back(exit);
}

DisplayMain::GameState MainMenu::Show()
{
	rgb_matrix::FrameCanvas*  canvas = DisplayMain::GetCanvas();
	for(auto item : menuItems)
	{
		item.Draw(canvas);
	}

    canvas = DisplayMain::GetWindow()->SwapOnVSync(canvas);
    DisplayMain::SetCanvas(canvas);

	DisplayMain::GameState result = DisplayMain::Noop;

	while (result == DisplayMain::Noop)
	{
		result = GetMenuResponse();
		printf("MenuResult = %d\n", result);
	}

	return result;
}

DisplayMain::GameState MainMenu::HandleClick(int x, int y)
{
	// Loop through each menu option to determine if it was clicked
	for(auto item : menuItems)
	{
		if (item.IsClicked(x, y))
		{
			return item.ItemValue;
		}
	}

	return DisplayMain::Noop;
}

DisplayMain::GameState MainMenu::GetMenuResponse()
{
    while(true)
    {
        std::unique_ptr<InputEvent> currentEvent(InputEventReader::GetEvent());
        if (currentEvent != nullptr && currentEvent->State == InputEvent::Down)
        {
            return HandleClick(currentEvent->PositionX, currentEvent->PositionY);
        }
    }
}