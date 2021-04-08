#include "Activity.hpp"

/////////////////////////////////////////////////////////////////////////



// Propreties
const int LEVELS_scrolling_step = 20;
const int LEVELS_cells = 3;
const int LEVELS_card_margin = 35;
const int LEVELS_card_width = (WINDOW_WIDTH - (LEVELS_cells + 1) * LEVELS_card_margin) / LEVELS_cells;
const int LEVELS_card_height = LEVELS_card_width / 2;
const int LEVELS_card_height_total = LEVELS_card_height + LEVELS_card_margin;

// background
const char *LEVELS_background = "../images/images/levelBackground.jpg";

// font
const char *LEVELS_font = "../images/font.ttf";

/////////////////////////////////////////////////////////////////////////
inline void draw_text( SDL_Renderer *render, TTF_Font *font, char const *t, SDL_Rect rect);
/////////////////////////////////////////////////////////////////////////

class LevelActivity: public Activity
{
private:
	WindowManager *manager;
	SDL_Rect rect;
	SDL_Texture *background;
	TTF_Font *font;
	long y = 0; // (first level - marign) with top of window
	int i = 0; // row the smallest level that is displayed; p.s. it is a mutliple of LEVELS_cells
	int taken = 0; // the part taken of the i-th level (include the margin also!!)
	int maxi;	// max row to display
	bool rend = true;
	void getConvenientRect(int l, int m);	// edit rect to fit at l '0->' row, m column

public :
	LevelActivity(WindowManager *manager, SDL_Renderer *render)
	{
		this->manager = manager;
		background = IMG_LoadTexture(render, LEVELS_background );
		font = TTF_OpenFont(LEVELS_font, 37);

		maxi = i + (WINDOW_HEIGHT - (LEVELS_card_height_total - taken)) / LEVELS_card_height_total +
			   (((WINDOW_HEIGHT - (LEVELS_card_height_total - taken)) % LEVELS_card_height_total) != 0);
	}

	void updateData(void *data) override
	{
		rend=*(bool*)data;
	}	

	void renderActivity(SDL_Renderer *render) override
	{
		if(!rend)
			return;

		// calculate mini i-row, maximun maxi-row
		i = y / (LEVELS_card_height + LEVELS_card_margin);
		taken = y % (LEVELS_card_height + LEVELS_card_margin);
		maxi = i + (WINDOW_HEIGHT - (LEVELS_card_height_total - taken)) / LEVELS_card_height_total +
			   (((WINDOW_HEIGHT - (LEVELS_card_height_total - taken)) % LEVELS_card_height_total) != 0);

		SDL_RenderClear(render);
		SDL_RenderCopy(render, background, NULL, NULL);
		for (int l = i; l < maxi + 1; ++l)
			for (int m = 0; m < LEVELS_cells; ++m)
			{
				getConvenientRect(l - i, m);
				SDL_SetRenderDrawColor(render, 255, 0, 0, 170);
				SDL_RenderFillRect(render, &rect );
				draw_text( render, font, to_string(l * LEVELS_cells + m + 1).c_str(), rect);
			}

		rend = false;
		SDL_RenderPresent(render);
	}

	void clickListener(SDL_Event *event) override
	{
		if(SDL_MOUSEWHEEL == event->type)
		{
			rend = true;
			if( event->wheel.y >= 1)
			{
				y += LEVELS_scrolling_step;
			}
			else if(event->wheel.y <= -1)
			{
				if(y - LEVELS_scrolling_step >= 0 )
				{
					y -= LEVELS_scrolling_step;
				}
				else
				{
					rend = false;
					return;
				}
			}

		}
		else if(SDL_BUTTON_LEFT == event->button.button)
		{
			int x, y;
			SDL_GetMouseState(&x, &y);

			for (int l = i; l < maxi + 1; ++l)
				for (int m = 0; m < LEVELS_cells; ++m)
				{
					getConvenientRect(l - i, m);
					if(rect.x <= x && x <= rect.x + rect.w &&
							rect.y <= y && y <= rect.y + rect.h)
					{
						manager->goToGame(l * LEVELS_cells + m + 1, false);
						cout << "Selected level:  " << l *LEVELS_cells + m + 1 << endl;
					}
				}
		}

	}

};

void LevelActivity::getConvenientRect(int l, int m)
{
	rect = {LEVELS_card_margin *(m + 1) + LEVELS_card_width * m,
			LEVELS_card_height_total *l + LEVELS_card_margin - taken,
			LEVELS_card_width, LEVELS_card_height
		   };
}

inline void draw_text( SDL_Renderer *render, TTF_Font *font, char const *t, SDL_Rect rect)
{
	int w, h;
	TTF_SizeText( font, t, &w, &h);
	rect.x += (rect.w - w) / 2;
	rect.y += (rect.h - h) / 2;
	rect.w = w;
	rect.h = h;

	SDL_Surface *surf = TTF_RenderText_Solid(font, t, {255, 255, 255});
	SDL_Texture *textu = SDL_CreateTextureFromSurface(render, surf);
	SDL_RenderCopy(render, textu, NULL, &rect);
	SDL_FreeSurface(surf);
	SDL_DestroyTexture(textu);
}