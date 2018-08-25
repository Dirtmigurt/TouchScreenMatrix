#include"../include/stdafx.h"

void BootScreen::Show(rgb_matrix::RGBMatrix* matrix, rgb_matrix::FrameCanvas* off_screen_canvas)
{    
	// Fill screen with some type of color
	off_screen_canvas->Fill(0xFF, 0, 0);

	rgb_matrix::Font  titleFont;
	if (!titleFont.LoadFont("../fonts/9x15.bdf"))
	{
		fprintf(stderr, "Failed to load one of the title fonts!");
	}
	else
	{
		std::unique_ptr<rgb_matrix::Font> outlineFont(titleFont.CreateOutlineFont());
		rgb_matrix::Color titleColor(0, 0xFF, 0);
		rgb_matrix::Color subtitleColor(0x00, 0x00, 0xFF);
		rgb_matrix::DrawText(off_screen_canvas, titleFont, 0, 22, titleColor, nullptr, "TOUCH MATRIX", 0);
		rgb_matrix::DrawText(off_screen_canvas, *outlineFont, 0, 44, subtitleColor, nullptr, "TOUCH MATRIX", 0);
	}

    off_screen_canvas = matrix->SwapOnVSync(off_screen_canvas);

	std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    while(true)
    {
        std::unique_ptr<InputEvent> currentEvent(InputEventReader::GetEvent());
        if (currentEvent != nullptr)
        {
            return;
        }
    }
}
