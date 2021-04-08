#include "Activity.hpp"

/////////////////////////////////////////////////////////////////////////

// background
#define HOME_background "../images/images/background.jpg"

// buttons
#define HOME_button_width 170
#define HOME_button_height 70
#define HOME_button_x 51
#define HOME_button_margin 20

// automatic
#define HOME_automatic "../images/buttons/automatic.png"
#define HOME_automatic_y WINDOW_HEIGHT-HOME_button_height-HOME_button_margin

// offline
#define HOME_offline "../images/buttons/2Players.png"
#define HOME_offline_y HOME_automatic_y-HOME_button_height-HOME_button_margin

// normal
#define HOME_normal "../images/buttons/normal.png"
#define HOME_normal_y  HOME_offline_y-HOME_button_height-HOME_button_margin

// createLevel
#define HOME_createLevel "../images/buttons/createLevel.png"
#define HOME_createLevel_width 170
#define HOME_createLevel_height 100
#define HOME_createLevel_x WINDOW_WIDTH-HOME_button_margin-HOME_createLevel_width
#define HOME_createLevel_y WINDOW_HEIGHT-HOME_createLevel_height-HOME_button_margin


/////////////////////////////////////////////////////////////////////////

class HomeActivity: public Activity
{
private:
	WindowManager *manager;
	SDL_Texture *background;
	SDL_Texture *normal;
	SDL_Texture *offline;
	SDL_Texture *automatic;
	SDL_Texture *createLevel;
	SDL_Rect rect;
public :
	HomeActivity(WindowManager *manager, SDL_Renderer *render)
	{
		this->manager = manager;
		background = IMG_LoadTexture(render, HOME_background );
		normal = IMG_LoadTexture(render, HOME_normal);
		offline = IMG_LoadTexture(render, HOME_offline);
		automatic = IMG_LoadTexture(render, HOME_automatic);
		createLevel = IMG_LoadTexture(render, HOME_createLevel);
	}

	void updateData(void *data) override
	{
	}

	void renderActivity(SDL_Renderer *render) override
	{
		SDL_RenderClear(render);

		// Background
		SDL_RenderCopy(render, background, NULL, NULL);

		// Normal mode
		rect = {HOME_button_x, HOME_normal_y, HOME_button_width, HOME_button_height};
		SDL_RenderCopy(render, normal, NULL, &rect);

		// Offline mode
		rect = {HOME_button_x, HOME_offline_y, HOME_button_width, HOME_button_height};
		SDL_RenderCopy(render, offline, NULL, &rect);

		// Automatic mode
		rect = {HOME_button_x, HOME_automatic_y, HOME_button_width, HOME_button_height};
		SDL_RenderCopy(render, automatic, NULL, &rect);

		// Create mode
		rect = {HOME_createLevel_x, HOME_createLevel_y, HOME_createLevel_width, HOME_createLevel_height};
		SDL_RenderCopy(render, createLevel, NULL, &rect);

		SDL_RenderPresent(render);
	}

	void clickListener(SDL_Event *event) override
	{
		if(SDL_BUTTON_LEFT == event->button.button)
		{
			int x, y;
			SDL_GetMouseState(&x, &y);

			if( HOME_button_x <= x &&  x <= HOME_button_x + HOME_button_width &&
					HOME_normal_y <= y && y <= HOME_normal_y + HOME_button_height)
			{
				// Normal mode
				cout << "Normal Mode" << endl;
				manager->goToLevels(NORMAL);
			}
			else if( HOME_button_x <= x &&  x <= HOME_button_x + HOME_button_width &&
					 HOME_offline_y <= y && y <= HOME_offline_y + HOME_button_height)
			{
				// Offline mode
				cout << "Offline Mode" << endl;
				manager->goToLevels(OFFLINE);
			}
			else if( HOME_button_x <= x &&  x <= HOME_button_x + HOME_button_width &&
					 HOME_automatic_y <= y && y <= HOME_automatic_y + HOME_button_height)
			{
				// Automatic mode
				cout << "Automatic Mode" << endl;
				manager->goToLevels( AUTOMATIC);
			}
			else if( HOME_createLevel_x <= x &&  x <= HOME_createLevel_x + HOME_createLevel_width &&
					 HOME_createLevel_y <= y && y <= HOME_createLevel_y + HOME_createLevel_height)
			{
				// Create mode
				cout << "Create Level" << endl;
			}



		}
	}
};