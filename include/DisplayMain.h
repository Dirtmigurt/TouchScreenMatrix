#pragma once
#include "stdafx.h"

class DisplayMain
{
    public:
        static void Start();
        static rgb_matrix::RGBMatrix* GetWindow();
		static rgb_matrix::FrameCanvas* GetCanvas();
		static void SetCanvas(rgb_matrix::FrameCanvas*);
        //const static GameObjectManager& GetGameObjectManager();

		static std::chrono::high_resolution_clock::time_point lastFrameTime;
        const static int SCREEN_WIDTH = 128;
        const static int SCREEN_HEIGHT = 64;

		// clock ideas
		// 1. 12 Hour large
		// 2. 24 hour large
		// 3. 12 AM/PM with day/month/year
		// 4. Binary clock
		// 5. Progress bar clock 
		// 6. https://www.onthisday.com/events/august/29 print scrolling list of events for the day
		// 7. 3d spinning cube just to try out 3d projections on 2d screen
		enum GameState
		{
			Uninitialized,
			ShowingBootScreen,
			ShowingMenu,
			Showing12H,
			Showing24H,
			ShowingDate,
			ShowingBinary,
			ShowingProgress,
			ShowingEvents,
			Showing3D,
			ShowingImageViewer,
			ShowingPaint,
			ShowingConfig,
			Exiting,
			Noop
		};

    private:
        static bool IsExiting();
        static void GameLoop();

        static void ShowBootScreen();
        static void ShowMenu();
		static void Show12H();

        static GameState gameState;
        static rgb_matrix::RGBMatrix* mainWindow;
        static rgb_matrix::FrameCanvas* off_screen_canvas;

        //static GameObjectManager gameObjectManager;
};