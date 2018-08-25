#include "stdafx.h"

MainMenu::MenuResult MainMenu::Show(rgb_matrix::RGBMatrix* matrix, rgb_matrix::FrameCanvas* canvas)
{
    for(int i = 0; i < DisplayMain::SCREEN_WIDTH; i++)
    {
        for(int j = 0; j < DisplayMain::SCREEN_HEIGHT; j++)
        {
            if (j < 32)
            {
                canvas->SetPixel(i, j, 0, 150, 0);
            }
            else
            {
                canvas->SetPixel(i, j, 150, 0, 0);
            }
        }
    }

    canvas = matrix->SwapOnVSync(canvas);

    return GetMenuResponse();
}

MainMenu::MenuResult MainMenu::HandleClick(int x, int y)
{
    if (y < 32)
    {
        return Play;
    }

    return Exit;
}

MainMenu::MenuResult MainMenu::GetMenuResponse()
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