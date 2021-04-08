class Keys
{
public :
	SDL_Keycode up, down, left, right;
};

class Simulator
{
public:
	// snubby,coins,walls,obstacles
	double q1, q2, q3, q4;
	int radius;
	vector<int> stop;
	Simulator()
	{
		init();
	}
	void init()
	{
		radius = 40;
		q1 = 1;
		q2 = -1100;
		q3 = 50;
		q4 = 5200;
	}
};

class Snubby
{
public :
	Simulator sim;
	string icon;
	int lives;
	int coins;
	pair<int, int> lastReturn;
	pair<int, int> position;
	Keys keys;
	bool upActivated, downActivated, leftActivated, rightActivated;
	vector<vector<data>> *map;

	int changePosition(int x, int y);

	Snubby();
	Snubby(string ico, int x, int y, int key, vector<vector<data>> *ma);
	bool isDead();
	int moveUp();
	int moveDown();
	int moveLeft();
	int moveRight();
	int resurrect();
};


Snubby::Snubby()
{
	coins = 0;
	lives = SNUBBY_LIVE;
	upActivated = false;
	downActivated = false;
	leftActivated = false;
	rightActivated = false;
}

Snubby::Snubby(string ico, int x, int y, int key, vector<vector<data>> *ma): Snubby()
{
	icon = ico;
	position = make_pair(x, y);
	lastReturn = make_pair(x + 1, y);
	if(key == 0)
	{
		keys.up = SDLK_UP;
		keys.down = SDLK_DOWN;
		keys.left = SDLK_LEFT;
		keys.right = SDLK_RIGHT;
	}
	else
	{
		keys.up = SDLK_w;
		keys.down = SDLK_s;
		keys.left = SDLK_a;
		keys.right = SDLK_d;
	}
	map = ma;

}
bool Snubby::isDead()
{
	return 	lives == -1;
}
int Snubby::moveUp()
{
	return changePosition(position.first, position.second - SNUBBY_STEP);
}
int Snubby::moveDown()
{
	return changePosition(position.first, position.second + SNUBBY_STEP);
}
int Snubby::moveLeft()
{
	return changePosition(position.first - SNUBBY_STEP, position.second);
}
int Snubby::moveRight()
{
	return changePosition(position.first + SNUBBY_STEP, position.second);
}

int Snubby::changePosition(int x, int y)
{
	// cout << position.second << position.first << endl;

	int w = (*map)[0].size(), h = (*map).size();
	int squareSize = SQUARE_SIZE;

	//Snubby Out Of Range
	if(x < 0 || x + SNUBBY_SIZE >= (*map)[0].size() * squareSize)
		return -1;
	if(y < 0 || y + SNUBBY_SIZE >= (*map).size() * squareSize)
		return -1;

	int cote[8][2] =
	{
		// !!!!!!!!!!!! tuple (yyyyyy , xxxxxxxx)
		// coordinates of eight square cotes, ordered from left to right, top to bottom.

		{ y / squareSize, x / squareSize},
		{ y / squareSize, (SNUBBY_SIZE / 2 + x) / squareSize},
		{ y / squareSize, (SNUBBY_SIZE + x) / squareSize},
		{(SNUBBY_SIZE / 2 + y) / squareSize, x / squareSize},
		{ (SNUBBY_SIZE / 2 + y) / squareSize, (SNUBBY_SIZE + x) / squareSize},
		{(SNUBBY_SIZE + y) / squareSize, x / squareSize},
		{ (SNUBBY_SIZE + y) / squareSize, (SNUBBY_SIZE / 2 + x) / squareSize},
		{ (SNUBBY_SIZE + y) / squareSize, (SNUBBY_SIZE + x) / squareSize}
	};

	//Check if at least one snubby's corner is in the restricted zone!left-top,right-bot,rigth-top,left_bot & middle of each segment
	if(		(*map)[cote[0][0]][cote[0][1]] == RESTRICTED ||
			(*map)[cote[2][0]][cote[2][1]] == RESTRICTED ||
			(*map)[cote[5][0]][cote[5][1]] == RESTRICTED ||
			(*map)[cote[7][0]][cote[7][1]] == RESTRICTED ||

			(*map)[cote[1][0]][cote[1][1]] == RESTRICTED ||
			(*map)[cote[3][0]][cote[3][1]] == RESTRICTED ||
			(*map)[cote[4][0]][cote[4][1]] == RESTRICTED ||
			(*map)[cote[6][0]][cote[6][1]] == RESTRICTED
	  )
	{

		//If STEP is too large and gets to RESTRICTED zone! change "STEP" to fit the available area
		int diff = 1;   //Difference between current pos and frontier

		// If Move is to Left
		if(position.first > x )
		{
			while(position.first - diff >= 0
					&& (*map)[cote[0][0]][(position.first - diff) / squareSize] != RESTRICTED
					&& (*map)[cote[3][0]][(position.first - diff) / squareSize] != RESTRICTED
					&& (*map)[cote[5][0]][(position.first - diff) / squareSize] != RESTRICTED)
				diff++;
			x = position.first - diff + 1;

		}// If Move is to Right
		else if(position.first < x )
		{
			while(position.first + diff + SNUBBY_SIZE < w * squareSize
					&& (*map)[cote[2][0]][(position.first + diff + SNUBBY_SIZE) / squareSize] != RESTRICTED
					&& (*map)[cote[4][0]][(position.first + diff + SNUBBY_SIZE) / squareSize] != RESTRICTED
					&& (*map)[cote[7][0]][(position.first + diff + SNUBBY_SIZE) / squareSize] != RESTRICTED)
				diff++;
			x = position.first + diff - 1;

		}// If Move is to Down
		else if(position.second < y )
		{
			while( position.second + diff + SNUBBY_SIZE < h * squareSize
					&& (*map)[(position.second + diff + SNUBBY_SIZE) / squareSize][cote[5][1]] != RESTRICTED
					&& (*map)[(position.second + diff + SNUBBY_SIZE) / squareSize][cote[6][1]] != RESTRICTED
					&& (*map)[(position.second + diff + SNUBBY_SIZE) / squareSize][cote[7][1]] != RESTRICTED)
				diff++;
			y = position.second + diff - 1;
		}// If Move is to Up
		else if(position.second > y )
		{
			while( position.second - diff >= 0
					&& (*map)[(position.second - diff) / squareSize][cote[0][1]] != RESTRICTED
					&& (*map)[(position.second - diff) / squareSize][cote[1][1]] != RESTRICTED
					&& (*map)[(position.second - diff) / squareSize][cote[2][1]] != RESTRICTED)
				diff++;
			y = position.second - diff + 1;
		}
		if(diff == 1)
			return -1;

	}
	// new Position is legitimate
	position.first = x;
	position.second = y;

	//Check if all snubby's corners are all in the return zone!left-top,right-bot,rigth-top,left_bot
	if((*map)[cote[0][0]][cote[0][1]] == RETURN &&
			(*map)[cote[2][0]][cote[2][1]] == RETURN &&
			(*map)[cote[5][0]][cote[5][1]] == RETURN &&
			(*map)[cote[7][0]][cote[7][1]] == RETURN
	  )
	{
		lastReturn = position;
	}

	return 0;
}

// returns remaining lives of snubby
int Snubby::resurrect()
{
	if (lastReturn == position )
		return -1;
	if(lives > 0)
	{
		position = lastReturn;
		--lives;
	}
	else
		lives = -1;
	return lives;
}