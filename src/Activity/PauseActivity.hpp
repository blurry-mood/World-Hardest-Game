#include "Activity.hpp"

/////////////////////////////////////////////////////////////////////////

// buttons
#define PAUSE_button_width 170
#define PAUSE_button_height 70
#define PAUSE_button_x (WINDOW_WIDTH-PAUSE_button_width)/2
#define PAUSE_button_margin 20

// resume
#define PAUSE_resume "../images/buttons/resume.png"
#define PAUSE_resume_y (WINDOW_HEIGHT-4*PAUSE_button_height-3*PAUSE_button_margin)/2

// restart
#define PAUSE_restart "../images/buttons/restart.png"
#define PAUSE_restart_y PAUSE_resume_y+PAUSE_button_height+PAUSE_button_margin

// levels
#define PAUSE_levels "../images/buttons/levels.png"
#define PAUSE_levels_y PAUSE_restart_y+PAUSE_button_height+PAUSE_button_margin

// quit
#define PAUSE_quit "../images/buttons/quit.png"
#define PAUSE_quit_y PAUSE_levels_y+PAUSE_button_height+PAUSE_button_margin

/////////////////////////////////////////////////////////////////////////

class PauseActivity: public Activity
{
private:
	WindowManager *manager;
	SDL_Texture *resume;
	SDL_Texture *restart;
	SDL_Texture *levels;
	SDL_Texture *quit;
	SDL_Rect rect;

public :
	PauseActivity(WindowManager *manager, SDL_Renderer *render)
	{
		this->manager = manager;
		resume = IMG_LoadTexture(render, PAUSE_resume );
		restart = IMG_LoadTexture(render, PAUSE_restart);
		levels = IMG_LoadTexture(render, PAUSE_levels);
		quit = IMG_LoadTexture(render, PAUSE_quit);
	}

	void updateData(void *data) override
	{
	}

	void renderActivity(SDL_Renderer *render) override
	{
		// Background
		SDL_SetRenderDrawColor(render, 0, 0, 0, 170);
		SDL_RenderFillRect(render, NULL);

		// Resume
		rect = {PAUSE_button_x, PAUSE_resume_y, PAUSE_button_width, PAUSE_button_height};
		SDL_RenderCopy(render, resume, NULL, &rect);

		// Restart
		rect = {PAUSE_button_x, PAUSE_restart_y, PAUSE_button_width, PAUSE_button_height};
		SDL_RenderCopy(render, restart, NULL, &rect);

		// Levels
		rect = {PAUSE_button_x, PAUSE_levels_y, PAUSE_button_width, PAUSE_button_height};
		SDL_RenderCopy(render, levels, NULL, &rect);

		// Quit
		rect = {PAUSE_button_x, PAUSE_quit_y, PAUSE_button_width, PAUSE_button_height};
		SDL_RenderCopy(render, quit, NULL, &rect);

		SDL_RenderPresent(render);
	}

	void clickListener(SDL_Event *event) override
	{
		if(SDL_BUTTON_LEFT == event->button.button)
		{
			int x, y;
			SDL_GetMouseState(&x, &y);

			if( PAUSE_button_x <= x &&  x <= PAUSE_button_x + PAUSE_button_width &&
					PAUSE_resume_y <= y && y <= PAUSE_resume_y + PAUSE_button_height)
			{
				// Resume
				cout << "Pause - Resume" << endl;
				manager->goToGame(-1, true);
			}
			else if( PAUSE_button_x <= x &&  x <= PAUSE_button_x + PAUSE_button_width &&
					 PAUSE_restart_y <= y && y <= PAUSE_restart_y + PAUSE_button_height)
			{
				// Restart
				cout << "Pause - Restart" << endl;
				manager->goToGame(-1, false);
			}
			else if( PAUSE_button_x <= x &&  x <= PAUSE_button_x + PAUSE_button_width &&
					 PAUSE_levels_y <= y && y <= PAUSE_levels_y + PAUSE_button_height)
			{
				// Levels
				cout << "Pause - Levels" << endl;
				manager->goToLevels(UNCHANGED);
			}
			else if( PAUSE_button_x <= x &&  x <= PAUSE_button_x + PAUSE_button_width &&
					 PAUSE_quit_y <= y && y <= PAUSE_quit_y + PAUSE_button_height)
			{
				// Quit
				cout << "Pause - Quit" << endl;
				manager->goToHome();
			}
		}
	}
};