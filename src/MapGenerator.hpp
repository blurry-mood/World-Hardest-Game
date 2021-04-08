#ifndef MAP_GENERATOR
#define MAP_GENERATOR

typedef struct _
{
	int x, y;
	struct _ *next;
} Point;

/////////////////////////////////////////////////////////////////////////// Prototypes
vector<vector<data>> generateLevel(int lvl, int mapW, int mapH);
int writeImage(string filename, vector<vector<data>> buffer);     //-1: Error, 0: Sucess


pair<int, int> getSnubbyPostion(vector<vector<data>> &map);
vector<Obstacle *> getObstacles(int w, int h, int xx, int yy);
vector<pair<int, int>> getCoins(vector<vector<data>> &map);
void addGreenZone(vector<vector<data>> &map);

static float distance(int x0, int y0, int x, int y);
static float uniformGenerator(long *seed);
static Point *shortestPath(int x0, int y0, int x1, int y1);
int nbreNeighboors(data value, int x, int y, vector<vector<data>> const &map);
void caveGeneration(vector<vector<data>> &map);
int deleteImage(char *filename);                    //0: Success

/////////////////////////////////////////////////////////////////////////// Functions
int deleteImage(char *filename)
{
	return remove(filename);
}

pair<int, int> getSnubbyPostion(vector<vector<data>> &map)
{
	for (int i = 0; i < map.size(); ++i)
		for (int j = 0; j < map[i].size(); ++j)
			if(map[i][j] == RETURN)
				return{j * SQUARE_SIZE, i * SQUARE_SIZE};
	return {0, 0};
}

vector<Obstacle *> getObstacles(int w, int h, int xx, int yy)
{
	vector<Obstacle *> obs;
	int step = 1 + (rand() % 3);

	for (int i = 0; i < w; i += step)
		if(i != xx )
			obs.push_back(new ObstacleTranslation(make_pair(i * SQUARE_SIZE, 0),
												  make_pair(i * SQUARE_SIZE, WINDOW_HEIGHT),
												  (float)rand() / RAND_MAX,
												  pow(-1, rand() % 2)));
	for (int j = 0; j < h; j += step)
		if(j != yy )
			obs.push_back(new ObstacleTranslation(make_pair(0, j * SQUARE_SIZE),
												  make_pair(WINDOW_WIDTH, j * SQUARE_SIZE),
												  (float)rand() / RAND_MAX,
												  pow(-1, rand() % 2)));
	cout << "SIZE: " << obs.size() << endl;

	return obs;
}

vector<pair<int, int>> getCoins(vector<vector<data>> &map)
{
	vector<pair<int, int>> coins;
	for (int i = 0; i < map.size(); ++i)
		for (int j = 0; j < map[i].size(); ++j)
			if(map[i][j] != RESTRICTED && rand() % 15 <= 1)
				coins.push_back(make_pair(j * SQUARE_SIZE + (rand() % SQUARE_SIZE / 2),
										  i * SQUARE_SIZE + (rand() % SQUARE_SIZE / 2)));
	return coins;
}

void addGreenZone(vector<vector<data>> &map)
{
	for (int i = 1; i < map.size() - 1; ++i)
		for (int j = 1; j < map[i].size() - 1; ++j)
		{
			if(map[i][j] == ALLOWED && map[i - 1][j] == RESTRICTED && map[i][j - 1] == RESTRICTED
					&& map[i][j + 2] == RESTRICTED)
				map[i][j] = RETURN;

			if(map[i][j] == ALLOWED && map[i - 1][j] == RESTRICTED && map[i][j + 1] == RESTRICTED
					&& map[i][j - 2] == RESTRICTED)
				map[i][j] = RETURN;

			if(map[i][j] == ALLOWED && map[i + 1][j] == RESTRICTED && map[i][j - 1] == RESTRICTED
					&& map[i][j + 2] == RESTRICTED)
				map[i][j] = RETURN;
			if(map[i][j] == ALLOWED && map[i + 1][j] == RESTRICTED && map[i][j + 1] == RESTRICTED
					&& map[i][j - 2] == RESTRICTED)
				map[i][j] = RETURN;
		}
}

void caveGeneration(vector<vector<data>> &map)
{
	bool stop = false;
	int i = 1;
	string n = "img";
	while(stop == false )
	{
		i++;
		stop = true;
		writeImage(n + to_string(i) + ".png", map);
		for (int y = 1; y < map.size() - 1; ++y)
			for (int x = 1; x < map[y].size() - 1; ++x)
				if(map[y][x] == RESTRICTED && ( nbreNeighboors(RESTRICTED,  x,  y, map) < 3 ||
												((map[y + 1][x] == RESTRICTED) + (map[y - 1][x] == RESTRICTED) +
												 (map[y][x + 1] == RESTRICTED) + (map[y][x - 1] == RESTRICTED)) < 2))
				{
					stop = false;
					map[y][x] = ALLOWED;
				}
				else if(map[y][x] == ALLOWED && (map[y + 1][x] == RESTRICTED) && (map[y - 1][x] == RESTRICTED))
				{
					map[y + 1][x] = ALLOWED ;
					map[y - 1][x] = ALLOWED;
				}
	}
}

int nbreNeighboors(data value, int x, int y, vector<vector<data>> const &map)
{
	return (map[y - 1][x] == value ) + (map[y - 1][x - 1] == value ) + (map[y - 1][x + 1] == value ) +
		   (map[y][x - 1] == value ) + (map[y][x + 1] == value ) +
		   (map[y + 1][x] == value ) + (map[y + 1][x - 1] == value ) + (map[y + 1][x + 1] == value);
}

int writeImage(string filename, vector<vector<data>> buffer)
{
	int unit = SQUARE_SIZE;
	image< rgb_pixel > image(unit * buffer[0].size(), unit * buffer.size());
	int i;
	for (int y = 0; y < image.get_height(); ++y)
	{
		i = 0;
		for (int x = 0; x < image.get_width(); ++x)
		{


			if(buffer[y / unit][x / unit] == RETURN)
				image[y][x] = rgb_pixel(GREEN_COLOR[0], GREEN_COLOR[1], GREEN_COLOR[2]);
			else if(buffer[y / unit][x / unit] == RESTRICTED)
			{
				i = 0;
				if((x + BORDER < image.get_width() && buffer[y / unit][(x + BORDER) / unit] != RESTRICTED) ||
						(x - BORDER >= 0 && buffer[y / unit][(x - BORDER) / unit] != RESTRICTED) ||
						(y - BORDER >= 0 && buffer[(y - BORDER) / unit][x / unit] != RESTRICTED) ||
						(y + BORDER < image.get_height() && buffer[(y + BORDER) / unit][x / unit] != RESTRICTED) ||

						(y + BORDER < image.get_height() && x + BORDER < image.get_width() && buffer[(y + BORDER) / unit][(x + BORDER) / unit] != RESTRICTED) ||
						(y + BORDER < image.get_height() && x - BORDER >= 0 && buffer[(y + BORDER) / unit][(x - BORDER) / unit] != RESTRICTED ) ||
						(y - BORDER >= 0 && x + BORDER < image.get_width() && buffer[(y - BORDER) / unit][(x + BORDER) / unit] != RESTRICTED ) ||
						(y - BORDER >= 0 && x - BORDER >= 0 && buffer[(y - BORDER) / unit][(x - BORDER) / unit] != RESTRICTED )
				  )
					image[y][x] = rgb_pixel(BORDER_COLOR[0], BORDER_COLOR[1], BORDER_COLOR[2]);
				else
					image[y][x] = rgb_pixel(RESTRICTED_COLOR[0], RESTRICTED_COLOR[1], RESTRICTED_COLOR[2]);

			}
			else if(buffer[y / unit][x / unit] == ALLOWED)
			{
				if(y % unit > unit / 2 )
				{
					if (i == unit)
						i = 0;
					if(i < unit / 2)
						image[y][x] = rgb_pixel(BLUE_COLOR[0], BLUE_COLOR[1], BLUE_COLOR[2]);

					else if(i < unit)
						image[y][x] = rgb_pixel(BLUEE_COLOR[0], BLUEE_COLOR[1], BLUEE_COLOR[2]);

					i++;
				}
				else
				{
					if (i == 0)
						i = unit;
					if(i <= unit / 2)
						image[y][x] = rgb_pixel(BLUE_COLOR[0], BLUE_COLOR[1], BLUE_COLOR[2]);
					else if(i <= unit)
						image[y][x] = rgb_pixel(BLUEE_COLOR[0], BLUEE_COLOR[1], BLUEE_COLOR[2]);
					i--;
				}

			}
		}
	}
	image.write(filename);
	return 0;
}

vector<vector<data>> generateLevel(int lvl, int mapW, int mapH)
{
	vector<vector<data>> map(mapH, vector<data>());
	srand(lvl);
	long seed = rand();

	// mapW=mapW/2-1;
	// mapH-=2;

	int start_x = mapW * uniformGenerator(&seed) / 2, start_y = mapH * uniformGenerator(&seed) / 2;
	int stop_x = mapW - 1, stop_y = mapH * (1 + uniformGenerator(&seed)) / 2;


	float n;
	int x = start_x, y = start_y;		// Start Position
	float left, right, down;				// Directions Proba
	Point *pt;							// Shortest Path next point
	for (int i = 0; i < mapH; ++i)
		map[i] = vector<data>(mapW, RESTRICTED);

	map[start_y][start_x] = 2;

	while(1)
	{
		pt = shortestPath(x, y, stop_x, stop_y);
		if(pt == NULL)
		{
			// map[y][x] = RETURN;
			map[y][x] = ALLOWED;
			break;
		}

		n = uniformGenerator(&seed);

		if(pt->x == x)
		{
			if(pt->y < y)
			{
				left = 0.2;
				right = 0.4;
				down = 0.5;
			}
			else
			{
				left = 0.2;
				right = 0.4;
				down = 0.9;
			}
		}
		else
		{
			if(pt->x < x)
			{
				left = 0.1;
				right = 0.6;
				down = 0.8;
			}
			else
			{
				left = 0.5;
				right = 0.6;
				down = 0.8;
			}
		}

		if(n < left && x - 1 >= 0  && map[y][x - 1] == 0)
		{
			map[y][x - 1] = ALLOWED;
			x -= 1;
		}
		else if(n < right && x + 1 < mapW && map[y][x + 1] == 0)
		{
			map[y][x + 1] = ALLOWED;
			x += 1;
		}
		else if(n < down && y - 1 >= 0 && map[y - 1][x] == 0)
		{
			map[y - 1][x] = ALLOWED;
			y -= 1;
		}
		else if( y + 1 < mapH && map[y + 1][x] == 0)
		{
			map[y + 1][x] = ALLOWED;
			y += 1;
		}
		else
		{
			map[pt->y][pt->x] = ALLOWED;
			x = pt->x;
			y = pt->y;
		}

		free(pt);
	}

	vector<data> tobo(mapW, RESTRICTED);
	map.insert(map.begin(), tobo);
	map.insert(map.end(), tobo);

	for (int y = 0; y < map.size() ; ++y)
	{
		map[y].insert(map[y].begin(), RESTRICTED);
		int len = map[y].size();
		for (int x = 0 ; x < len ; ++x)
			map[y].push_back(map[y][len - x - 1]);

	}

	caveGeneration(map);
	addGreenZone(map);

	return map;
}

static Point *shortestPath(int x0, int y0, int x1, int y1)
{
	if (x0 == x1 && y0 == y1)
		return NULL;
	Point *pt = (Point *) malloc(sizeof(Point));
	//// Determine the nearest point to (x1,y1) starting from (x0,y0)
	int d1 = distance(x0 + 1, y0, x1, y1);
	int d2 = distance(x0 - 1, y0, x1, y1);
	int d3 = distance(x0, y0 + 1, x1, y1);
	int d4 = distance(x0, y0 - 1, x1, y1);
	if(d1 > d2)
	{
		if(d2 > d3)
		{
			if(d3 > d4)
			{
				pt->x = x0;
				pt->y = y0 - 1;
			}
			else
			{
				pt->x = x0;
				pt->y = y0 + 1;
			}
		}
		else
		{
			if(d2 > d4)
			{
				pt->x = x0;
				pt->y = y0 - 1;
			}
			else
			{
				pt->x = x0 - 1;
				pt->y = y0;
			}
		}

	}
	else
	{
		if(d1 > d3)
		{
			if(d3 > d4)
			{
				pt->x = x0;
				pt->y = y0 - 1;
			}
			else
			{
				pt->x = x0;
				pt->y = y0 + 1;
			}
		}
		else
		{
			if(d1 > d4)
			{
				pt->x = x0;
				pt->y = y0 - 1;
			}
			else
			{
				pt->x = x0 + 1;
				pt->y = y0;
			}
		}
	}

	//pt->next = shortestPath(pt->x, pt->y, x1, y1);
	return pt;
}

static float distance(int x0, int y0, int x, int y)
{
	return sqrt(pow(x - x0, 2) + pow(y - y0, 2));
}

static float uniformGenerator(long *seed)
{
	long a = 75, b = 596145, m = 2147483647;
	*seed = (a * *seed + b) % m;
	return 1.0 * *seed / m;
}


#endif