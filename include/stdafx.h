#pragma once

// System libraries
#include<thread>
#include<memory>
#include<map>
#include<list>
#include<chrono>
#include<cmath>
#include<string>
#include <algorithm>
#include<linux/input.h>
#include<fcntl.h>
#include<unistd.h>
#include<cstdio>

// LED Matrix Libraries
#include "led-matrix.h"
#include "threaded-canvas-manipulator.h"
#include "transformer.h"
#include "graphics.h"

// My Classes
#include "InputEvent.h"
#include "InputEventReader.h"
#include "DisplayMain.h"
#include "BootScreen.h"
#include "MainMenu.h"
#include "Point.h"
#include "VisibleObject.h"
#include "VisibleObjectManager.h"
#include "MenuItem.h"