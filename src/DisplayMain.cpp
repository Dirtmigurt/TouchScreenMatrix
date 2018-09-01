#include"../include/stdafx.h"

// Initialize static members
DisplayMain::GameState DisplayMain::gameState = Uninitialized;
std::chrono::high_resolution_clock::time_point DisplayMain::lastFrameTime = std::chrono::high_resolution_clock::now();
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
    matrix_options.led_rgb_sequence = "rbg";
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

rgb_matrix::RGBMatrix* DisplayMain::GetWindow()
{
    return mainWindow;
}

rgb_matrix::FrameCanvas* DisplayMain::GetCanvas()
{
    return off_screen_canvas;
}

void DisplayMain::SetCanvas(rgb_matrix::FrameCanvas* canvas)
{
    off_screen_canvas = canvas;
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
            printf("Showing Boot Screen.\n");
            ShowBootScreen();
            break;
        }
        case ShowingMenu:
        {
            printf("Showing Menu.\n");
            ShowMenu();
            break;
        }
        case Showing12H:
        {
            printf("Showing 12H\n");
			break;
        }
		case Showing24H:
		{
            printf("Showing 24H.\n");
			break;
	    }
		case Showing3D:
		{
            printf("Showing 3D.\n");
			break;
		}
		case ShowingBinary:
		{
            printf("Showing Binary.\n");
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
    printf("bootScreenLoaded\n");
    bootScreen.Show();
    printf("bootScreen.Show returned.\n");
    gameState = ShowingMenu;
}

void DisplayMain::ShowMenu()
{
    MainMenu mainMenu;
	mainMenu.Load();
    printf("mainMenu loaded\n");
    gameState = mainMenu.Show();
    printf("mainMenu.Show returned.\n");
}
