#include"../include/stdafx.h"

void BootScreen::Load()
{
	bootImage.Load("../icons/BootScreen.bmp");
}

void BootScreen::Show()
{    
	// Fill screen with some type of color
	rgb_matrix::FrameCanvas*  canvas = DisplayMain::GetCanvas();
	bootImage.Draw(canvas);
    canvas = DisplayMain::GetWindow()->SwapOnVSync(canvas);
    DisplayMain::SetCanvas(canvas);

	std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    while(true)
    {
        std::unique_ptr<InputEvent> currentEvent(InputEventReader::GetEvent());
        if (currentEvent != nullptr && currentEvent->State == InputEvent::FingerState::Down)
        {
            return;
        }
    }
}
