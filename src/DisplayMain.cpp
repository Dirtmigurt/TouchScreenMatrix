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

    printf("killing input event reader.\n");
    InputEventReader::Kill();
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
            gameState = ShowingMenu;
			break;
        }
		case Showing24H:
		{
            printf("Showing 24H.\n");
            gameState = ShowingMenu;
			break;
	    }
		case Showing3D:
		{
            printf("Showing 3D.\n");
            gameState = ShowingMenu;
			break;
		}
		case ShowingBinary:
		{
            printf("Showing Binary.\n");
            gameState = ShowingMenu;
			break;
		}
		case ShowingDate:
		{
            printf("Showing DateTime.\n");
            gameState = ShowingMenu;
			break;
		}
		case ShowingEvents:
		{
            printf("Showing History events.\n");
            gameState = ShowingMenu;
			break;
		}
		case ShowingImageViewer:
		{
            printf("Showing Image Viewer.\n");
            gameState = ShowingMenu;
			break;
		}
		case ShowingPaint:
		{
            printf("Showing Paint.\n");
            gameState = ShowingMenu;
			break;
		}
		case ShowingProgress:
		{
            printf("Showing Progress Bars.\n");
            gameState = ShowingMenu;
			break;
		}
		case ShowingConfig:
		{
            printf("Showing Config.\n");
            gameState = ShowingMenu;
			break;
		}
		case Exiting:
		{
            printf("Showing Exiting.\n");
            gameState = ShowingMenu;
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
    bootScreen.Show();
    gameState = ShowingMenu;
}

void DisplayMain::ShowMenu()
{
    MainMenu mainMenu;
	mainMenu.Load();
    gameState = mainMenu.Show();
}
