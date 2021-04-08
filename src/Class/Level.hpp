#include "Coin.hpp"
#include "Snubby.hpp"
// #include "../Model.hpp"
#include "Obstacles/ObstacleRotation.hpp"
#include "Obstacles/ObstacleTranslation.hpp"
#include "Obstacles/ObstacleSquare.hpp"
#include "Obstacles/ObstaclePeriodic.hpp"
#include "../MapGenerator.hpp"

class Level;

int minList(vector<double> arr);
double Distance(int x0, int y0, int x1, int y1);
int distanceSnubbyObstacle(Snubby &s, Obstacle &obstak);
int distanceSnubbyCoin(Snubby &s, Coin &c);

#include "../solver.hpp"


class Level
{
public :
	Game *game;
	Mode mode;
	Snubby s1, s2;
	// Model model;
	vector<vector<data>> map;
	vector<Coin> coins;
	int virtuals;	// count of virtual coins
	vector<Obstacle *> obs;
	thread *lvlThread;
	bool stop;

	pair<int, int> getNearestSafeZone(Snubby &s);
	void commandSnubby(Snubby &s);
	vector<pair<int, int>> getEnvironment(Snubby &s);
	pair<int, int> checkDirections( int x, int y, pair<int, int> center, int radius);
	vector<pair<int, int>> wallDirections( int x, int y);

	int eatCoin(Snubby &s);
	int eatSnubby(Snubby &s);
	vector<bool> isOver();

	static void levelThread(Level *thi);

	Level() {;}
	~Level()
	{
		delete lvlThread;
		for(auto t : obs)
			delete t;
	}
	Level(Game *gam, int level, Mode mode);
	void startLevel();
	void pauseLevel();

};

Level::Level(Game *gam, int level, Mode mode)
{
	game = gam;
	this->mode = mode;

	map = generateLevel(level, MAP_WIDTH / 2 - 1, MAP_HEIGHT - 2);
	for(auto x : getCoins(map))
		coins.push_back(Coin(x.first, x.second));
	virtuals = 0;

	pair<int, int> position = getSnubbyPostion(map);

	s1 = Snubby(string(SNUBBY1_IMAGE), position.first, position.second, 0, &map);
	s2 = Snubby(string(SNUBBY2_IMAGE), position.first, position.second, 1, &map);
	if(mode == NORMAL)
		s2.lives = -1;

	vector<Obstacle *> tmp =  getObstacles(map[0].size(), map.size(),
										   position.first / SQUARE_SIZE, position.second / SQUARE_SIZE);
	obs.insert(obs.end(), tmp.begin(), tmp.end());


	writeImage(MAP_IMAGE, map);
	stop = true;
	lvlThread = new thread(&levelThread, this);

}

void Level::startLevel()
{
	stop = false;
}
void Level::pauseLevel()
{
	stop = true;
}

void Level::levelThread(Level *thi)
{
	int i = 0;
	vector<bool> over;
	while(1)
	{
		SDL_Delay(OBSTACLES_TIME);
		if(thi->stop)
		{
			SDL_Delay(10 * OBSTACLES_TIME);
			continue;
		}
		i++;
		// Move obstacles
		for(auto x : thi->obs)
			x->nextMove();

		if(!thi->s1.isDead())
		{
			if(thi->s1.upActivated)
				thi->s1.moveUp();
			if(thi->s1.downActivated)
				thi->s1.moveDown();
			if(thi->s1.leftActivated)
				thi->s1.moveLeft();
			if(thi->s1.rightActivated)
				thi->s1.moveRight();
			thi->eatCoin(thi->s1);
			thi->eatSnubby(thi->s1);
		}
		if(!thi->s2.isDead())
		{
			if(thi->mode == AUTOMATIC)
			{
				if(i == 40)
				{
					i = 0;
					cout << "Starting Add" << endl;
					thi->virtuals += whenBlocked(thi->map, thi->coins, thi->s2.position.first, thi->s2.position.second);
				}
				thi->commandSnubby(thi->s2);

			}
			if(thi->s2.upActivated)
				thi->s2.moveUp();
			if(thi->s2.downActivated)
				thi->s2.moveDown();
			if(thi->s2.leftActivated)
				thi->s2.moveLeft();
			if(thi->s2.rightActivated)
				thi->s2.moveRight();

			thi->eatCoin(thi->s2);
			thi->eatSnubby(thi->s2);
		}

		/// check if level is validated
		over = thi->isOver();
		if(!over.empty())
		{
			cout << "DONE Playing !" << endl;
			thi->game->gameOver(over[0], over[1], over[2]);
		}
	}

}


int Level::eatSnubby(Snubby &s)
{
	for(auto x : obs)
		if(distanceSnubbyObstacle( s, *x) <= 0)
			s.resurrect();
	return 0;
}

int Level::eatCoin(Snubby &s)
{
	for(int i = 0 ; i < coins.size(); )
	{
		if(distanceSnubbyCoin(s, coins[i]) <= 0)
		{
			if(!coins[i].isVirtual)
				s.coins++;
			else
				virtuals--;
			coins.erase(coins.begin() + i);
		}
		else
			i++;
	}
	return 0;
}

vector<bool> Level::isOver()
{
	vector<bool> win(3, false);
	if(mode != NORMAL)
		win[0] = true;

	if(s1.isDead())
		win[2] = true;
	else if(mode != NORMAL && s2.isDead())
		win[1] = true;
	else if(coins.size() == virtuals)
	{
		if(s1.position == s1.lastReturn || s2.position == s2.lastReturn )
		{
			if(s1.coins == s2.coins)
				win[1] = (s1.lives >= s2.lives);
			else
				win[1] = (s1.coins >= s2.coins);
			win[2] = (!win[1]);
		}
		else win = {};
	}
	else win = {};
	return win;
}

vector<pair<int, int>> Level::getEnvironment(Snubby &s)
{
	vector<pair<int, int>> env;
	pair<int, int> dir;
	int x = s.position.first + SNUBBY_SIZE / 2;
	int y = s.position.second + SNUBBY_SIZE / 2;

	//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\  OBSTACLES
	for(auto &obstak : obs)
	{
		pair<int, int> center = obstak->getPosition();
		center.first += OBSTACLE_SIZE / 2;
		center.second += OBSTACLE_SIZE / 2;
		dir = checkDirections( x, y, center, OBSTACLE_SIZE / 2);
		// cout<<dir.second<<endl;
		env.push_back(dir);
	}

	//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\   WALL
	vector<pair<int, int>> nn = wallDirections( s.position.first, s.position.second);
	env.insert(env.end(), nn.begin(), nn.end());


	//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\   COINS
	if(coins.size() == virtuals)
	{
		pair<int, int> pp = getNearestSafeZone(s);
		coins.push_back(Coin(pp.first * SQUARE_SIZE + (rand() % SQUARE_SIZE / 2),
							 pp.second * SQUARE_SIZE + (rand() % SQUARE_SIZE / 2)));
	}

	for(auto &coin : coins)
	{
		pair<int, int> center = coin.position;
		center.first += COIN_SIZE / 2;
		center.second += COIN_SIZE / 2;
		dir = checkDirections( x, y, center, COIN_SIZE / 2);
		env.push_back(dir);
	}

	return env;
}

pair<int, int> Level::checkDirections( int x, int y, pair<int, int> center, int radius)
{
	return make_pair(center.first - x, center.second - y );
}
vector<pair<int, int>> Level::wallDirections( int x, int y)
{

	// if no mobile obstacle is found at top of snubby.
	int w0 = (x / SQUARE_SIZE), w1 = ((x + SNUBBY_SIZE) / SQUARE_SIZE), h0 = (y / SQUARE_SIZE), h1;

	while(h0 >= 0 && map[h0][w0] != RESTRICTED && map[h0][w1] != RESTRICTED )
		h0--;
	int firstUp = y - (1 + h0) * SQUARE_SIZE;

	// if no mobile obstacle is found at bottom of snubby.
	w0 = (x / SQUARE_SIZE);
	w1 = ((x + SNUBBY_SIZE) / SQUARE_SIZE);
	h0 = (y + SNUBBY_SIZE) / SQUARE_SIZE;
	while(h0 < MAP_HEIGHT  && map[h0][w0] != RESTRICTED && map[h0][w1] != RESTRICTED )
		h0++;
	int firstDown =  h0 * SQUARE_SIZE - (y + SNUBBY_SIZE);

	// if no mobile obstacle is found at Left of snubby.
	w0 = (x / SQUARE_SIZE);
	h1 = ((y + SNUBBY_SIZE) / SQUARE_SIZE);
	h0 = (y / SQUARE_SIZE);
	while(w0 >= 0 && map[h0][w0] != RESTRICTED && map[h1][w0] != RESTRICTED )
		w0--;
	int firstLeft = x - (1 + w0) * SQUARE_SIZE;

	// if no mobile obstacle is found at right of snubby.
	w0 = (x + SNUBBY_SIZE) / SQUARE_SIZE;
	h1 = ((y + SNUBBY_SIZE) / SQUARE_SIZE);
	h0 = (y / SQUARE_SIZE);
	while(w0 < MAP_WIDTH && map[h0][w0] != RESTRICTED && map[h1][w0] != RESTRICTED )
		w0++;
	int firstRight = w0 * SQUARE_SIZE - (x + SNUBBY_SIZE );
	return vector<pair<int, int>> {make_pair(0	, -firstUp),
								   make_pair(0	, firstDown),
								   make_pair(-firstLeft, 0	),
								   make_pair(firstRight, 0	)
								  };
} 

void Level::commandSnubby(Snubby &s)
{
	int cmd = getWhere(s, getEnvironment(s), obs.size());
	s.rightActivated = (cmd & 1) != 0;
	s.leftActivated = (cmd & 2) != 0;
	s.downActivated = (cmd & 4) != 0;
	s.upActivated = (cmd & 8) != 0;
}

pair<int, int> Level::getNearestSafeZone(Snubby &s)
{
	pair<int, int> pp;
	int x = s.position.first / SQUARE_SIZE;
	int y = s.position.second / SQUARE_SIZE;
	double dist = 100000;

	for (int yy = 0; yy < map.size(); ++yy)
		for (int xx = 0; xx < map[yy].size(); ++xx)
			if(map[yy][xx] == RETURN && Distance(x, y, xx, yy) < dist)
			{
				dist = Distance(x, y, xx, yy);
				pp = make_pair(xx, yy);
			}
	return pp;
}

////////////////////////////////////////////////////////////////////////////////// non object functions


int distanceSnubbyObstacle(Snubby &s, Obstacle &obstak)
{
	pair<int, int> centObs = obstak.getPosition();
	int x1 = centObs.first + OBSTACLE_SIZE / 2;
	int y1 = centObs.second + OBSTACLE_SIZE / 2;
	int x0 = s.position.first;
	int y0 = s.position.second;

	return minList( vector<double> {Distance(x0, y0, x1, y1) - OBSTACLE_SIZE / 2,
									Distance(x0 + SNUBBY_SIZE, y0, x1, y1) - OBSTACLE_SIZE / 2,
									Distance(x0, y0 + SNUBBY_SIZE, x1, y1) - OBSTACLE_SIZE / 2,
									Distance(x0 + SNUBBY_SIZE, y0 + SNUBBY_SIZE, x1, y1) - OBSTACLE_SIZE / 2,
									Distance(x0 + SNUBBY_SIZE / 2, y0 + SNUBBY_SIZE / 2, x1, y1) - OBSTACLE_SIZE / 2 - SNUBBY_SIZE / 2
								   });
}
int distanceSnubbyCoin(Snubby &s, Coin &c)
{
	int x1 = c.position.first + COIN_SIZE / 2;
	int y1 = c.position.second + COIN_SIZE / 2;
	int x0 = s.position.first;
	int y0 = s.position.second;

	return minList( vector<double> {Distance(x0, y0, x1, y1) - COIN_SIZE / 2,
									Distance(x0 + SNUBBY_SIZE, y0, x1, y1) - COIN_SIZE / 2,
									Distance(x0, y0 + SNUBBY_SIZE, x1, y1) - COIN_SIZE / 2,
									Distance(x0 + SNUBBY_SIZE, y0 + SNUBBY_SIZE, x1, y1) - COIN_SIZE / 2,
									Distance(x0 + SNUBBY_SIZE / 2, y0 + SNUBBY_SIZE / 2, x1, y1) - COIN_SIZE / 2 - SNUBBY_SIZE / 2
								   });
}

int minList(vector<double> arr)
{
	int mi = arr[0];
	for (int i = 1 ; i < arr.size(); ++i)
		if(mi > arr[i])
			mi = arr[i];
	return static_cast<int>(mi);
}

double Distance(int x0, int y0, int x1, int y1)
{
	return sqrt(pow(x0 - x1, 2) + pow(y0 - y1, 2));
}