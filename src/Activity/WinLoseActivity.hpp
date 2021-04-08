#include "Activity.hpp"

/////////////////////////////////////////////////////////////////////////

// buttons
#define WIN_LOSE_button_width 130
#define WIN_LOSE_button_height 60
#define WIN_LOSE_button_x (WINDOW_WIDTH-WIN_LOSE_button_width)/2
#define WIN_LOSE_button_margin 20

// WIN LOSE IMAGE
#define WIN_LOSE_image_WIN "../images/images/you win.png"
#define WIN_LOSE_image_LOSE "../images/images/you lose.png"
#define WIN_LOSE_image_width 400
#define WIN_LOSE_image_height 270
#define WIN_LOSE_image_x (WINDOW_WIDTH-WIN_LOSE_image_width)/2
#define WIN_LOSE_image_y 30

// restart
#define WIN_LOSE_restart "../images/buttons/restart.png"
#define WIN_LOSE_restart_y WIN_LOSE_button_margin*2+WIN_LOSE_image_height+WIN_LOSE_image_y

// levels
#define WIN_LOSE_levels "../images/buttons/levels.png"
#define WIN_LOSE_levels_y WIN_LOSE_restart_y+WIN_LOSE_button_height+WIN_LOSE_button_margin

// quit
#define WIN_LOSE_quit "../images/buttons/quit.png"
#define WIN_LOSE_quit_y  WIN_LOSE_levels_y+WIN_LOSE_button_height+WIN_LOSE_button_margin

// prev_next
#define WIN_LOSE_prev_next_y  WIN_LOSE_quit_y+WIN_LOSE_button_height+WIN_LOSE_button_margin

// previous
#define WIN_LOSE_previous "../images/buttons/previous.png"
#define WIN_LOSE_previous_x (WINDOW_WIDTH -2*WIN_LOSE_button_width-WIN_LOSE_button_margin)/2

// next
#define WIN_LOSE_next "../images/buttons/next.png"
#define WIN_LOSE_next_x WIN_LOSE_previous_x+WIN_LOSE_button_width+WIN_LOSE_button_margin



/////////////////////////////////////////////////////////////////////////

class WinLoseActivity: public Activity
{
private:
	WindowManager *manager;
	SDL_Texture *image_WIN, *image_LOSE,
				*restart, *levels, *quit,
				*previous, *next;
	TTF_Font *font;
	SDL_Rect rect;
	vector<bool> win;


public :
	WinLoseActivity(WindowManager *manager, SDL_Renderer *render)
	{
		this->manager = manager;
		image_WIN =  IMG_LoadTexture(render, WIN_LOSE_image_WIN );
		image_LOSE =  IMG_LoadTexture(render, WIN_LOSE_image_LOSE );
		restart = IMG_LoadTexture(render, WIN_LOSE_restart );
		levels =  IMG_LoadTexture(render, WIN_LOSE_levels );
		quit = IMG_LoadTexture(render, WIN_LOSE_quit );
		previous =  IMG_LoadTexture(render, WIN_LOSE_previous );
		next = IMG_LoadTexture(render, WIN_LOSE_next );
		win = vector<bool>(3, false);
		font = TTF_OpenFont(LEVELS_font, 37);
	}

	void updateData(void *data) override
	{
		win = *((vector<bool> *)data);
	}

	void renderActivity(SDL_Renderer *render) override
	{
		// Background
		SDL_SetRenderDrawColor(render, 0, 0, 0, 150);
		SDL_RenderFillRect(render, NULL);

		// Image for winner or loser
		if(win[0])
		{
			SDL_Rect rectLose;
			cout << "player 1 : " << win[1] << endl;
			string p1, p2;
			if(win[1])
			{
				rect = {WIN_LOSE_image_x - ( WIN_LOSE_image_width / 2 + 50), WIN_LOSE_image_y, WIN_LOSE_image_width, WIN_LOSE_image_height};
				rectLose = {WIN_LOSE_image_x + ( WIN_LOSE_image_width / 2 + 50), WIN_LOSE_image_y, WIN_LOSE_image_width, WIN_LOSE_image_height};
				p1 = "Player 1";
				p2 = "Player 2";
			}
			else
			{
				rectLose = {WIN_LOSE_image_x - (WIN_LOSE_image_width / 2 + 50), WIN_LOSE_image_y, WIN_LOSE_image_width, WIN_LOSE_image_height};
				rect = {WIN_LOSE_image_x + ( WIN_LOSE_image_width / 2 + 50), WIN_LOSE_image_y, WIN_LOSE_image_width, WIN_LOSE_image_height};
				p1 = "Player 2";
				p2 = "Player 1";
			}
			SDL_RenderCopy(render, image_WIN, NULL, &rect);
			SDL_RenderCopy(render, image_LOSE, NULL, &rectLose);
			rect.y += rect.h;
			rect.h = 70;
			rectLose.y += rectLose.h;
			rectLose.h = 70;
			draw_text(render, font, p1.c_str(), rect);
			draw_text(render, font, p2.c_str(), rectLose);

		}
		else
		{
			rect = {WIN_LOSE_image_x, WIN_LOSE_image_y, WIN_LOSE_image_width, WIN_LOSE_image_height};
			if(win[1])
				SDL_RenderCopy(render, image_WIN, NULL, &rect);
			else
				SDL_RenderCopy(render, image_LOSE, NULL, &rect);
		}



		// Restart
		rect = {WIN_LOSE_button_x, WIN_LOSE_restart_y, WIN_LOSE_button_width, WIN_LOSE_button_height};
		SDL_RenderCopy(render, restart, NULL, &rect);

		// Levels
		rect = {WIN_LOSE_button_x, WIN_LOSE_levels_y, WIN_LOSE_button_width, WIN_LOSE_button_height};
		SDL_RenderCopy(render, levels, NULL, &rect);

		// Quit
		rect = {WIN_LOSE_button_x, WIN_LOSE_quit_y, WIN_LOSE_button_width, WIN_LOSE_button_height};
		SDL_RenderCopy(render, quit, NULL, &rect);

		// Previous
		rect = {WIN_LOSE_previous_x, WIN_LOSE_prev_next_y, WIN_LOSE_button_width, WIN_LOSE_button_height};
		SDL_RenderCopy(render, previous, NULL, &rect);

		// Next
		rect = {WIN_LOSE_next_x, WIN_LOSE_prev_next_y, WIN_LOSE_button_width, WIN_LOSE_button_height};
		SDL_RenderCopy(render, next, NULL, &rect);

		SDL_RenderPresent(render);
	}

	void clickListener(SDL_Event *event) override
	{
		if(SDL_BUTTON_LEFT == event->button.button)
		{
			int x, y;
			SDL_GetMouseState(&x, &y);

			if(WIN_LOSE_button_x <= x && x <= WIN_LOSE_button_x + WIN_LOSE_button_width &&
					WIN_LOSE_restart_y <= y && y <= WIN_LOSE_restart_y + WIN_LOSE_button_height)
			{
				// restart button
				cout << "Win_Lose - restart" << endl;
				manager->goToGame(-1, false);
			}
			else if(WIN_LOSE_button_x <= x && x <= WIN_LOSE_button_x + WIN_LOSE_button_width &&
					WIN_LOSE_levels_y <= y && y <= WIN_LOSE_levels_y + WIN_LOSE_button_height)
			{
				// levels button
				cout << "Win_Lose - levels" << endl;
				manager->goToLevels(UNCHANGED);
			}
			else if(WIN_LOSE_button_x <= x && x <= WIN_LOSE_button_x + WIN_LOSE_button_width &&
					WIN_LOSE_quit_y <= y && y <= WIN_LOSE_quit_y + WIN_LOSE_button_height)
			{
				// Quit button
				cout << "Win_Lose - Quit" << endl;
				manager->goToHome();
			}

			else if(WIN_LOSE_previous_x <= x && x <= WIN_LOSE_previous_x + WIN_LOSE_button_width &&
					WIN_LOSE_prev_next_y <= y && y <= WIN_LOSE_prev_next_y + WIN_LOSE_button_height)
			{
				// Previous button
				cout << "Win_Lose - Previous" << endl;
				manager->goToGame(-2, false);
			}
			else if(WIN_LOSE_next_x <= x && x <= WIN_LOSE_next_x + WIN_LOSE_button_width &&
					WIN_LOSE_prev_next_y <= y && y <= WIN_LOSE_prev_next_y + WIN_LOSE_button_height)
			{
				// Next button
				cout << "Win_Lose - Next" << endl;
				manager->goToGame(0, false);
			}
		}
	}
};