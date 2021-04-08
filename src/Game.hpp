#define GAME_PAUSE "../images/buttons/pause.png"
#define GAME_PAUSE_x 0
#define GAME_PAUSE_y 0
#define GAME_PAUSE_width 30
#define GAME_PAUSE_height 20

class WindowManager;
class Level ;

class Game
{
public:
	Level *lvl;
	WindowManager *manager;
	SDL_Texture *background, *pause, *snubby1, *snubby2, *obstacle, *coin, *coin2;
	SDL_Rect rect;
	~Game();
	Game(WindowManager *manage, SDL_Renderer *render, int level, Mode mode);
	void initGame();
	void renderGame(SDL_Renderer *render);
	void eventHandler(SDL_Event *event);
	void pauseGame();
	void startGame();
	void gameOver(bool twop, bool player1won, bool player2won);

};

#include "Class/Level.hpp"
#include "Files.hpp"


Game::~Game()
{
	delete lvl;
}
Game::Game(WindowManager *manage, SDL_Renderer *render, int level, Mode mode)
{
	manager = manage;
	lvl = new Level(this, level, mode);
	snubby1 = IMG_LoadTexture(render, SNUBBY1_IMAGE);
	snubby2 = IMG_LoadTexture(render, SNUBBY2_IMAGE);
	obstacle = IMG_LoadTexture(render, OBSTACLE_IMAGE);
	coin = IMG_LoadTexture(render, COIN_IMAGE);
	coin2 = IMG_LoadTexture(render, COIN2_IMAGE);
	background = IMG_LoadTexture(render, MAP_IMAGE);
	pause = IMG_LoadTexture(render, GAME_PAUSE);
}

void Game::renderGame(SDL_Renderer *render)
{
	SDL_RenderClear(render);

	// map
	SDL_RenderCopy(render, background, NULL, NULL);

	// snubbies
	if(!lvl->s1.isDead())
	{
		rect = {lvl->s1.position.first, lvl->s1.position.second, SNUBBY_SIZE, SNUBBY_SIZE};
		SDL_RenderCopy(render, snubby1, NULL, &rect);
	}
	if(!lvl->s2.isDead())
	{
		rect = {lvl->s2.position.first, lvl->s2.position.second, SNUBBY_SIZE, SNUBBY_SIZE};
		SDL_RenderCopy(render, snubby2, NULL, &rect);
	}

	// coins
	for(auto c : lvl->coins)
	{
		rect = {c.position.first, c.position.second, COIN_SIZE, COIN_SIZE};
		if(!c.isVirtual)
			SDL_RenderCopy(render, coin, NULL, &rect);
		else
			SDL_RenderCopy(render, coin2, NULL, &rect);
	}

	// obstacles
	for(auto o : lvl->obs)
	{
		pair<int, int > pos = o->getPosition();
		rect = {pos.first, pos.second, OBSTACLE_SIZE, OBSTACLE_SIZE};
		SDL_RenderCopy(render, obstacle, NULL, &rect);
	}

	// GAME_PAUSE button
	rect = {GAME_PAUSE_x, GAME_PAUSE_y, GAME_PAUSE_width, GAME_PAUSE_height};
	SDL_RenderCopy(render, pause, NULL, &rect);

	SDL_RenderPresent(render);
}

void Game::eventHandler(SDL_Event *event)
{
	if(SDL_BUTTON_LEFT == event->button.button)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		rect = {GAME_PAUSE_x, GAME_PAUSE_y, GAME_PAUSE_width, GAME_PAUSE_height};

		if(GAME_PAUSE_x <= x && x <= GAME_PAUSE_x + GAME_PAUSE_width &&
				GAME_PAUSE_y <= y && y <= GAME_PAUSE_y + GAME_PAUSE_height)
			manager->goToPause();

	}
	else if(event->type == SDL_KEYDOWN || event->type == SDL_KEYUP)
	{
		if(event->key.keysym.sym == lvl->s1.keys.left)
			lvl->s1.leftActivated = (event->type == SDL_KEYDOWN);
		else if(event->key.keysym.sym == lvl->s1.keys.right)
			lvl->s1.rightActivated = (event->type == SDL_KEYDOWN);
		else if(event->key.keysym.sym == lvl->s1.keys.up)
			lvl->s1.upActivated = (event->type == SDL_KEYDOWN);
		else if(event->key.keysym.sym == lvl->s1.keys.down)
			lvl->s1.downActivated = (event->type == SDL_KEYDOWN);

		if(event->key.keysym.sym == lvl->s2.keys.left)
			lvl->s2.leftActivated = (event->type == SDL_KEYDOWN);
		else if(event->key.keysym.sym == lvl->s2.keys.right)
			lvl->s2.rightActivated = (event->type == SDL_KEYDOWN);
		else if(event->key.keysym.sym == lvl->s2.keys.up)
			lvl->s2.upActivated = (event->type == SDL_KEYDOWN);
		else if(event->key.keysym.sym == lvl->s2.keys.down)
			lvl->s2.downActivated = (event->type == SDL_KEYDOWN);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::pauseGame()
{
	lvl->pauseLevel();
}
void Game::startGame()
{
	lvl->startLevel();
}
void Game::gameOver(bool twop, bool player1won, bool player2won)
{
	cout << "Game Over! " << endl;
	manager->goToWinLose(twop, player1won, player2won);
}