#include"../include/stdafx.h"

// Initialize static members
DisplayMain::GameState DisplayMain::gameState = Uninitialized;
rgb_matrix::RGBMatrix* DisplayMain::mainWindow = nullptr;
rgb_matrix::FrameCanvas* DisplayMain::off_screen_canvas = nullptr;
void DisplayMain::Start()
{
    if (gameState != Uninitialized)
    {
        return;
    }

    // Initialize RGBMatrix
    rgb_matrix::RGBMatrix::Options matrix_options;
    rgb_matrix::RuntimeOptions runtime_opt;
    runtime_opt.gpio_slowdown = 1;
    matrix_options.rows = SCREEN_HEIGHT;
    matrix_options.cols = SCREEN_HEIGHT;
    matrix_options.chain_length = 2;
    matrix_options.parallel = 1;
    matrix_options.hardware_mapping = "adafruit-hat-pwm";
    matrix_options.brightness = 50;
    mainWindow = rgb_matrix::CreateMatrixFromOptions(matrix_options, runtime_opt);
    off_screen_canvas = mainWindow->CreateFrameCanvas();

    // Add game objects to the object manager

    gameState = DisplayMain::ShowingBootScreen;

    while(!IsExiting())
    {
        GameLoop();
    }

    delete mainWindow;
}

bool DisplayMain::IsExiting()
{
    return gameState == DisplayMain::Exiting;
}

rgb_matrix::RGBMatrix& DisplayMain::GetWindow()
{
    return *mainWindow;
}

// GameObjectManager& DisplayMain::GetGameObjectManager()
// {
//     return gameObjectManager;
// }

void DisplayMain::GameLoop()
{
    
    switch(gameState)
    {
        case ShowingBootScreen:
        {
            ShowBootScreen();
            break;
        }
        case ShowingMenu:
        {
            ShowMenu();
            break;
        }
        case Playing:
        {
            // clear the display
            int x = 23;
            // update all objects in the game object manager
            // draw all objects in the game object manager

            // show off_screen_canvas

            // if currentEvent == exit then gameState = Exiting;
            // if currentEvent == back then gameState = ShowingMenu;
        }
    }
}

void DisplayMain::ShowBootScreen()
{
    BootScreen bootScreen;
    bootScreen.Show(mainWindow, off_screen_canvas);
    gameState = DisplayMain::ShowingMenu;
}

void DisplayMain::ShowMenu()
{
    MainMenu mainMenu;
    MainMenu::MenuResult result = mainMenu.Show(mainWindow, off_screen_canvas);
    switch(result)
    {
        case MainMenu::Exit:
        {
            gameState  = Exiting;
            break;
        }
        case MainMenu::Play:
        {
            gameState = Playing;
            break;
        }
    }
}
