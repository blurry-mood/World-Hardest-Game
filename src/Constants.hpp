#ifndef Constants
#define Constants

/////////////////////////////////////////////////////////////////////////// Preprocessors
#include <cmath>
#include <stdlib.h>
#include <stdbool.h>
#include <bits/stdc++.h>
#include <algorithm>
#include <iostream>
#include <thread>

#include <png++/png.hpp>
#include <png++/rgb_pixel.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
	
/////////////////////////////////////////////////////////////////////////// Namespaces
using namespace png;
using namespace std;
typedef int data;
class WindowManager;

///////////////////////////////////////////////////////  SNUBBY
#define SNUBBY_LIVE 3
#define SNUBBY_STEP 3
#define SNUBBY_SIZE 14
#define SNUBBY1_IMAGE "../images/snubby.png"
#define SNUBBY2_IMAGE "../images/snubby2.png"

///////////////////////////////////////////////////////  COINS
#define COIN_SIZE 10
#define COIN_IMAGE "../images/coin.png"
#define COIN2_IMAGE "../images/coin2.png"

///////////////////////////////////////////////////////  MAP
#define SQUARE_SIZE 23
#define MAP_IMAGE "../images/map.png"

#define RESTRICTED  0
#define ALLOWED 1
#define RETURN 2

#define BORDER 3
#define MAP_HEIGHT 30
#define MAP_WIDTH 40

static const int RESTRICTED_COLOR[3] = {155, 160, 255};
static const int ALLOWED_COLOR[3] = {155, 160, 180};
static const int RETURN_COLOR[3] = {100, 100, 190};
static const int BORDER_COLOR[3] = {0, 0, 0};

static const int BLUEE_COLOR[3] = {155, 160, 255};
static const int BLUE_COLOR[3] = {155, 160, 180};
static const int GREEN_COLOR[3] = {155, 255, 180};


///////////////////////////////////////////////////////  GAME
#define OBSTACLES_TIME 1000/60

///////////////////////////////////////////////////////  OBSTACLES
#define OBSTACLE_SIZE 10
#define OBSTACLE_IMAGE "../images/obstacle.png"
#define ROTATION_PARTITION 240
#define TRANSLATION_STEP 0.006

///////////////////////////////////////////////////////  WINDOW MANAGER
#define WINDOW_WIDTH SQUARE_SIZE*MAP_WIDTH
#define WINDOW_HEIGHT SQUARE_SIZE*MAP_HEIGHT
#define RENDERING_TIME 1000/60


///////////////////////////////////////////////////////  MODEL
#define MATE 20
#define LAMBDA 0.5
#define MUTATE 55


#endif