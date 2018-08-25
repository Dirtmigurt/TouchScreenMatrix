#pragma once
#include "stdafx.h"

class DisplayMain
{
    public:
        static void Start();
        static rgb_matrix::RGBMatrix& GetWindow();
        //const static GameObjectManager& GetGameObjectManager();

        const static int SCREEN_WIDTH = 128;
        const static int SCREEN_HEIGHT = 64;

    private:
        static bool IsExiting();
        static void GameLoop();

        static void ShowBootScreen();
        static void ShowMenu();

        enum GameState { Uninitialized, ShowingBootScreen, ShowingMenu, Playing, Exiting };

        static GameState gameState;
        static rgb_matrix::RGBMatrix* mainWindow;
        static rgb_matrix::FrameCanvas* off_screen_canvas;

        //static GameObjectManager gameObjectManager;
};