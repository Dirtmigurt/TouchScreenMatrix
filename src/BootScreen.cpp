#include"../include/stdafx.h"

void BootScreen::Load()
{
	bootImage.Load("../icons/BootScreen.bmp");
}

void BootScreen::Show(rgb_matrix::RGBMatrix* matrix, rgb_matrix::FrameCanvas* off_screen_canvas)
{    
	// Fill screen with some type of color
	off_screen_canvas->Clear();
	bootImage.Draw(*off_screen_canvas);
    off_screen_canvas = matrix->SwapOnVSync(off_screen_canvas);

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
