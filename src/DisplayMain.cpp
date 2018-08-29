#include"../include/stdafx.h"

// Initialize static members
DisplayMain::GameState DisplayMain::gameState = Uninitialized;
std::chrono::high_resolution_clock::time_point lastFrameTime = std::chrono::high_resolution_clock::now();
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
        case Showing12H:
        {
			break;
        }
		case Showing24H:
		{
			break;
	    }
		case Showing3D:
		{
			break;
		}
		case ShowingBinary:
		{
			break;
		}
		case ShowingDate:
		{
			break;
		}
		case ShowingEvents:
		{
			break;
		}
		case ShowingImageViewer:
		{
			break;
		}
		case ShowingPaint:
		{
			break;
		}
		case ShowingProgress:
		{
			break;
		}
		case ShowingConfig:
		{
			break;
		}
		case Exiting:
		{
			break;
		}
    default: 
		break;
    }
}

void DisplayMain::ShowBootScreen()
{
	BootScreen bootScreen;
	bootScreen.Load();
    bootScreen.Show(mainWindow, off_screen_canvas);
    gameState = ShowingMenu;
}

void DisplayMain::ShowMenu()
{
    MainMenu mainMenu;
	mainMenu.Load();
    gameState = mainMenu.Show(mainWindow, off_screen_canvas);
}
