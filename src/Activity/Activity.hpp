#ifndef OOOOOO
#define OOOOOO


class Activity
{
public :
	virtual void updateData(void* data) = 0;
	virtual void renderActivity(SDL_Renderer *render) = 0;
	virtual void clickListener(SDL_Event *render) = 0;
};

#endif