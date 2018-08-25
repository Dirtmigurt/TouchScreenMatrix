#include "BootScreen.h"

void BootScreen::Show(rgb_matrix::RGBMatrix* matrix, rgb_matrix::FrameCanvas* off_screen_canvas)
{
    // Fill screen with some type of color
    off_screen_canvas->Fill(80, 40, 40);
    off_screen_canvas = matrix->SwapOnVSync(off_screen_canvas);

    while(true)
    {
        std::unique_ptr<InputEvent> currentEvent(InputEventReader::GetEvent());
        if (currentEvent != nullptr)
        {
            return;
        }
    }
}