enum Interface
{
	HOME, PAUSE, SETTINGS, LEVELS, GAME, WIN_LOSE
};
enum Mode
{
	UNCHANGED, NORMAL, OFFLINE, AUTOMATIC
};

class Game;
class Activity;

class WindowManager
{
public:
	Interface interface;
	Mode mode;
	int level;
	SDL_Window *window ;
	SDL_Renderer *render ;
	thread eventThread ;
	Game *game;
	Activity *home, *pause, *win_lose, *levels;

	static void eventHandler(WindowManager *data);
	void renderInterface();

	WindowManager();
	void goToGame(int level, bool resume);
	void goToHome();
	void goToPause();
	void goToWinLose(bool twop, bool player1won, bool player2won);
	void goToLevels(Mode mode);
};

#include "../Game.hpp"
#include "HomeActivity.hpp"
#include "PauseActivity.hpp"
#include "LevelActivity.hpp"
#include "WinLoseActivity.hpp"

WindowManager::WindowManager()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("SnubbyLand", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
							  WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);
	TTF_Init();


	game = nullptr;
	home = new HomeActivity(this, render);
	pause = new PauseActivity(this, render);
	win_lose = new WinLoseActivity(this, render);
	levels = new LevelActivity(this, render);
	// pause = ;
	// settings = ;

	goToHome();

	eventThread = thread(eventHandler, this);
	renderInterface();
}

void WindowManager::goToGame(int level, bool resume)
{
	if(!resume)
	{
		// if(	game != nullptr)
		// 	delete game;
		// if level==-1 restart same level
		// if level==0 start next level 		\\\\\\\\\\\\\ !!!!!!!!!!! \\\\\\\\\\\\\\\\\ !!!!!!!!!!!
		// if level==-2 restart previous level
		if(level > 0)
			this->level = level;
		else if(level == 0)
			this->level++;
		else if(level == -2 && this->level >= 2)
			this->level--;
		game = new Game(this, render, this->level, mode);
	}
	game->startGame();
	interface = GAME;
}
void WindowManager::goToHome()
{
	if(	game != nullptr) game->pauseGame();
	interface = HOME;
}
void WindowManager::goToPause()
{
	if(	game != nullptr) game->pauseGame();
	interface = PAUSE;
}
void WindowManager::goToWinLose(bool twop, bool player1won, bool player2won)
{
	if(	game != nullptr) game->pauseGame();
	vector<bool> *b = new vector<bool> {twop, player1won, player2won};
	win_lose->updateData(b);
	interface = WIN_LOSE;
}
void WindowManager::goToLevels(Mode mode)
{
	bool rend = true;
	levels->updateData(&rend);
	if(mode != UNCHANGED)
		this->mode = mode;
	if(	game != nullptr) game->pauseGame();
	interface = LEVELS;
}

void WindowManager::eventHandler(WindowManager *thi)
{
	SDL_Event *event = (SDL_Event *)malloc(sizeof(SDL_Event));
	while(1)
	{
		SDL_WaitEvent(event);
		if (!event)
			continue;
		if(event->type == SDL_QUIT)
			exit(0);

		// distribute
		switch(thi->interface)
		{
		case HOME:
			if(event->type == SDL_MOUSEBUTTONDOWN)
				thi->home->clickListener(event);
			break;
		case PAUSE:
			if(event->type == SDL_MOUSEBUTTONDOWN)
				thi->pause->clickListener(event);
			break;
		case WIN_LOSE:
			if(event->type == SDL_MOUSEBUTTONDOWN)
				thi->win_lose->clickListener(event);
			break;
		case LEVELS:
			if(event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEWHEEL)
				thi->levels->clickListener(event);
			break;
		case GAME:
			thi->game->eventHandler(event);
			break;
		}
	}
}

// interfaces should renderPresent and renderClear
void WindowManager::renderInterface()
{
	while(true)
	{
		switch(interface)
		{
		case HOME:
			home->renderActivity(render);
			while(interface == HOME)
				SDL_Delay(RENDERING_TIME);
			break;
		case PAUSE:
			pause->renderActivity(render);
			while(interface == PAUSE)
				SDL_Delay(RENDERING_TIME);
			break;
		case WIN_LOSE:
			win_lose->renderActivity(render);
			while(interface == WIN_LOSE)
				SDL_Delay(RENDERING_TIME);
			break;
		case LEVELS:
			while(interface == LEVELS)
			{
				levels->renderActivity(render);
				SDL_Delay(RENDERING_TIME);
			}
			break;
		case GAME:
			while(interface == GAME)
			{
				game->renderGame(render);
				SDL_Delay(RENDERING_TIME);
			}
			break;
		}
	}
}