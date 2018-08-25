#pragma once

#include<thread>
#include<memory>
#include<map>
#include <list>
#include<linux/input.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>

#include "led-matrix.h"
#include "threaded-canvas-manipulator.h"
#include "transformer.h"
#include "graphics.h"
#include "InputEvent.h"
#include "InputEventReader.h"
#include "DisplayMain.h"
#include "BootScreen.h"
#include "MainMenu.h"