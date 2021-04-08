int whenBlocked(vector<vector<data>> map, vector<Coin> &coins, int x, int y);
int getWhere(Snubby &s, vector<pair<int, int>> r, int obs);
int step(pair<double, double> b);
pair<double, double> gothere(Snubby &s, vector<pair<int, int>> r, int obs);
pair<double, double> force(int radius, double q1, double q2, pair<int, int> r);
pair<int, int> getNearest(vector<Coin> &coins, int x, int y);
void aitHammadi(vector<vector<data>> &map, pair<int, int> coin, pair<int, int> snubby);
vector<pair<int, int>> path(vector<vector<data>> &map, pair<int, int> coin, pair<int, int> snubby);

int whenBlocked(vector<vector<data>> map, vector<Coin> &coins, int x, int y)
{
	pair<int, int> nearest = getNearest(coins, x, y);
	nearest.first /= SQUARE_SIZE;
	nearest.second /= SQUARE_SIZE;
	if(nearest.first < 0)
		return 0;

	aitHammadi(map, nearest, make_pair(x / SQUARE_SIZE, y / SQUARE_SIZE));

	vector<pair<int, int>> paths = path(map, nearest, make_pair(x / SQUARE_SIZE, y / SQUARE_SIZE));
	for(auto &p : paths)
	{
		coins.push_back(Coin(p.first * SQUARE_SIZE + (rand() % SQUARE_SIZE),
							 p.second * SQUARE_SIZE + (rand() % SQUARE_SIZE)));
		coins[coins.size() - 1].isVirtual = true;
	}
	return paths.size();
}

vector<pair<int, int>> path(vector<vector<data>> &map, pair<int, int> coin, pair<int, int> snubby)
{
	vector<pair<int, int>> paths{snubby};
	while(coin != snubby)
	{
		paths.push_back(coin);
		switch(map[coin.second][coin.first])
		{
		case 6:
			coin = make_pair( coin.first + 1, coin.second);
			break;
		case 7:
			coin = make_pair( coin.first - 1, coin.second);
			break;
		case 8:
			coin = make_pair( coin.first, coin.second + 1);
			break;
		case 9:
			coin = make_pair( coin.first, coin.second - 1 );
			break;
		}
	}
	return paths;
}


void aitHammadi(vector<vector<data>> &map, pair<int, int> coin, pair<int, int> snubby)
{
	vector<pair<int, int>> queue;
	map[snubby.second][snubby.first] = -1;
	queue.push_back(snubby);

	while(queue.size() >= 1)
	{
		snubby = queue[0];
		queue.erase(queue.begin());
		if(snubby == coin)
			break;

		// we went left
		if(snubby.first - 1 >= 0 && (map[snubby.second][snubby.first - 1] == ALLOWED ||
									 map[snubby.second][snubby.first - 1] == RETURN) )
		{
			queue.push_back(make_pair(snubby.first - 1, snubby.second));
			map[snubby.second][snubby.first - 1] = 6;
		}

		// we went right
		if(snubby.first + 1 < map[0].size() && (map[snubby.second][snubby.first + 1] == ALLOWED ||
												map[snubby.second][snubby.first + 1] == RETURN) )
		{
			queue.push_back(make_pair( snubby.first + 1, snubby.second));
			map[snubby.second][snubby.first + 1] = 7;
		}

		// we went up
		if(snubby.second - 1 >= 0 && (map[snubby.second - 1][snubby.first] == ALLOWED ||
									  map[snubby.second - 1][snubby.first] == RETURN) )
		{
			queue.push_back(make_pair(snubby.first, snubby.second - 1));
			map[snubby.second - 1][snubby.first] = 8;
		}

		// we went down
		if(snubby.second + 1 < map.size() && (map[snubby.second + 1][snubby.first] == ALLOWED ||
											  map[snubby.second + 1][snubby.first] == RETURN) )
		{
			queue.push_back(make_pair( snubby.first, snubby.second + 1));
			map[snubby.second + 1][snubby.first] = 9;
		}
	}

}

pair<int, int> getNearest(vector<Coin> &coins, int x, int y)
{
	pair<int, int> near(-10000, -10000);
	double D = Distance(x, y, near.first, near.second);
	for(auto &c : coins)
		if(!c.isVirtual && D > Distance(x, y, c.position.first, c.position.second))
		{
			D = Distance(x, y, c.position.first, c.position.second);
			near = c.position;
		}
	return near;
}


//////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\\\ \  \\\\\\\\\\\\ 

int getWhere(Snubby &s, vector<pair<int, int>> r, int obs)
{
	return step(gothere(s, r, obs));
}

int step(pair<double, double> b)
{
	int i = 0;
	// go right
	if(b.first > 0)
		i = i | 1;
	// go left
	else if(b.first < 0)
		i = i | 2;
	// go down
	if(b.second > 0)
		i = i | 4;
	// go up
	else if(b.second < 0)
		i = i | 8;
	// cout << b.first << " - " << b.second << " - " << i << endl;
	return i;
}

pair<double, double> gothere(Snubby &s, vector<pair<int, int>> r, int obs)
{
	pair<double, double> a(0, 0);
	pair<double, double> b(0, 0);

	int i;
	for (i = 0; i < obs; ++i)
	{
		a = force(OBSTACLE_SIZE / 2, s.sim.q1, s.sim.q4, r[i]);
		b.first += a.first;
		b.second += a.second;
	}
	for(; i < obs + 4; i++)
	{
		a = force(0, s.sim.q1, s.sim.q3, r[i]);
		b.first += a.first;
		b.second += a.second;
	}
	for(; i < r.size(); i++)
	{
		a = force(COIN_SIZE / 2, s.sim.q1, s.sim.q2, r[i]);
		b.first += a.first;
		b.second += a.second;
	}

	return b;
}

pair<double, double> force(int radius, double q1, double q2, pair<int, int> r)
{
	double dist = Distance(r.first, r.second, 0, 0);
	if(dist <= radius)
		dist = pow(dist, 1) * pow(10, -10);
	if (dist == 0)
		dist = pow(10, -1);
	double norm = -9 * pow(10, 9) * q1 * q2 / pow(dist, 3);
	return make_pair(r.first * norm, r.second * norm);
}



/*pair<int, int> Level::checkDirections( int x, int y, pair<int, int> center, int radius)
{

	int difference = SNUBBY_SIZE / 2 + radius;
	// look at Up
	int distUp = y - SNUBBY_SIZE / 2 - (center.second + radius);// distance from snubby and osbstak from top.
	// cout<<distUp<<" - ";
	if(distUp >= 0 && abs(x - center.first) <= difference ) return make_pair(x - center.first, -distUp);

	// look at Down
	int distDown = y + SNUBBY_SIZE / 2 - (center.second - radius);// distance from snubby and osbstak from bottom.
	if(distDown <= 0 && abs(x - center.first) <= difference) return make_pair(x - center.first, -distDown);

	// look at Left
	int distLeft = x - SNUBBY_SIZE / 2 - (center.first + radius);// distance from snubby and osbstak from left.
	if(distLeft >= 0 && abs(y - center.second) <= difference) return make_pair(-distLeft, y - center.second);

	// look at Right
	int distRight = x + SNUBBY_SIZE / 2 - (center.first - radius);// distance from snubby and osbstak from right.
	if(distRight <= 0 && abs(y - center.second) <= difference) return make_pair(-distRight, y - center.second);

	// look at Left Top
	if(x - SNUBBY_SIZE / 2 > center.first + radius && y - SNUBBY_SIZE / 2 > center.second + radius )
		return make_pair(center.first + radius - (x - SNUBBY_SIZE / 2), center.second + radius - (y - SNUBBY_SIZE / 2));

	// look at Left Bottom
	if(x - SNUBBY_SIZE / 2 > center.first + radius && y + SNUBBY_SIZE / 2 > center.second - radius )
		return make_pair(center.first + radius - (x - SNUBBY_SIZE / 2), center.second - radius - (y + SNUBBY_SIZE / 2));

	// look at Right Top
	if(x + SNUBBY_SIZE / 2 > center.first - radius && y - SNUBBY_SIZE / 2 > center.second + radius )
		return make_pair(center.first - radius - (x + SNUBBY_SIZE / 2), center.second + radius - (y - SNUBBY_SIZE / 2));

	// look at Right Bottom
	if(x + SNUBBY_SIZE / 2 > center.first - radius && y + SNUBBY_SIZE / 2 > center.second - radius )
		return make_pair(center.first - radius - (x + SNUBBY_SIZE / 2), center.second - radius - (y + SNUBBY_SIZE / 2));

}*/

/*
pair<int, int> Level::checkDirections( int x, int y, pair<int, int> center, int radius)
{
	double a = (double)(x - center.first) / (y - center.second);
	double b = (y + center.second - a * (x + center.first)) / 2.0;
	vector<pair<double, double>> pts;
	{
		double del1 = 4 * pow((x + a * b - a * y), 2) -
					  4 * (1 + pow(a, 2)) * (pow(x, 2) + pow(y, 2) - pow(SNUBBY_SIZE / 2, 2) - 2 * y * b);
		double x1 = (-2 * (x + a * b - a * y) + sqrt(del1)) / (2 * (1 + pow(a, 2)));
		double x2 = (-2 * (x + a * b - a * y) - sqrt(del1)) / (2 * (1 + pow(a, 2)));
		double y1 = a * x1 + b;
		double y2 = a * x2 + b;
		pts.push_back(make_pair(x1, y1));
		pts.push_back(make_pair(x2, y2));
	}

	{
		double del1 = 4 * pow((center.first + a * b - a * center.second), 2) -
					  4 * (1 + pow(a, 2)) * (pow(center.first, 2) + pow(center.second, 2) - pow(radius, 2) - 2 * center.second * b);
		double x1 = (-2 * (center.first + a * b - a * center.second) + sqrt(del1)) / (2 * (1 + pow(a, 2)));
		double x2 = (-2 * (center.first + a * b - a * center.second) - sqrt(del1)) / (2 * (1 + pow(a, 2)));
		double y1 = a * x1 + b;
		double y2 = a * x2 + b;
		pts.push_back(make_pair(x1, y1));
		pts.push_back(make_pair(x2, y2));
	}

	if(abs(pts[0].first - pts[2].first) < abs(pts[0].first - pts[3].first))
	{
		if(abs(pts[0].first - pts[2].first) < abs(pts[1].first - pts[2].first))
			return make_pair(pts[0].first - pts[2].first, pts[0].second - pts[2].second);
		else
			return make_pair(pts[1].first - pts[2].first, pts[1].second - pts[2].second);
	}
	else
	{
		if(abs(pts[0].first - pts[3].first) < abs(pts[1].first - pts[3].first))
			return make_pair(pts[0].first - pts[3].first, pts[0].second - pts[3].second);
		else
			return make_pair(pts[1].first - pts[3].first, pts[1].second - pts[3].second);
	}
}
*/