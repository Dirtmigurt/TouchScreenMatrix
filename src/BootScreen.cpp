#include"../include/stdafx.h"

void BootScreen::Show(rgb_matrix::RGBMatrix* matrix, rgb_matrix::FrameCanvas* off_screen_canvas)
{    
	// Fill screen with some type of color
	off_screen_canvas->Fill(0, 190, 190);

	rgb_matrix::Font  titleFont;
	if (!titleFont.LoadFont("../font/Title.bdf"))
	{
		fprintf(stderr, "Failed to load one of the title fonts!");
	}
	else
	{
		std::unique_ptr<rgb_matrix::Font> outlineFont(titleFont.CreateOutlineFont());
		rgb_matrix::Color titleColor(0xB8, 0x73, 0x33);
		rgb_matrix::Color subtitleColor(0x36, 0x45, 0x4F);
		rgb_matrix::DrawText(off_screen_canvas, titleFont, 0, 0, titleColor, nullptr, "TOUCH MATRIX", 0);
		rgb_matrix::DrawText(off_screen_canvas, *outlineFont, 0, 0, subtitleColor, nullptr, "TOUCH MATRIX", 0);
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