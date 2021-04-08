#include <exception>

Level *loadLevel(string filename)
{
	Level *lvl = new Level();
	int x, y;
	ifstream myfile;
	myfile.open(filename);

	// load snubby initial position
	myfile >> x >> y;

	// load coins
	unsigned int count;
	myfile >> count;
	for (int i = 0; i < count; ++i)
	{
		Coin *c = new Coin(-1,-1);
		myfile >> c->position.first >> c->position.second;
		lvl->coins.push_back(*c);
	}

	// load obstacles
	myfile >> count;
	Obstacle *o;
	for (int i = 0; i < count; ++i)
	{
		unsigned int id;
		myfile >> id;
		switch(id)
		{
		case TRANSLATION_id:
			o = new ObstacleTranslation();
			o->load(myfile);
			break;
		case ROTATION_id:
			o = new ObstacleRotation();
			o->load(myfile);
			break;
		case SQAURE_id:
			o = new ObstacleSquare();
			o->load(myfile);
			break;
		case PERIODIC_id:
			o = new ObstaclePeriodic();
			o->load(myfile);
			break;
		}
		lvl->obs.push_back(o);
	}

	// load map
	int w, h;

	myfile >> h >> w;
	for (int i = 0; i < h; ++i)
	{
		lvl->map.push_back(vector<data>(w, 0));
		for (int j = 0; j < w; ++j)
			myfile >> lvl->map[i][j];
	}
	lvl->s1 = Snubby(string(SNUBBY1_IMAGE), x, y, 0, &lvl->map);
	lvl->s2 = Snubby(string(SNUBBY2_IMAGE), x, y, 1, &lvl->map);

	myfile.close();
	return lvl;
}

void saveLevel(string filename, Level *lvl)
{
	ofstream myfile;
	myfile.open(filename);
	// save snubby initial position
	myfile << lvl->s1.lastReturn.first << " " << lvl->s1.lastReturn.second ;

	// save coins
	myfile << "\n" << lvl->coins.size() << "\n";
	for(auto s : lvl->coins)
		myfile << s.position.first << " " << s.position.second << " ";

	// save obstacles
	myfile << "\n" << lvl->obs.size() << "\n";
	for(auto s : lvl->obs)
		s->save(myfile);

	// save map
	myfile << "\n" << lvl->map.size() << " " << lvl->map[0].size() << "\n";
	for(auto line : lvl->map)
	{
		for(auto t : line)
			myfile << t << " ";
		myfile << "\n";
	}

	myfile.close();
}