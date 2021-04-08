class Model
{
public :
	vector<Snubby> s;
	int length;
	void initPopulation(vector<vector<data>> *map, int len, vector<Coin> coins);
	void movePopulation();
	void updatePopulation();

	void naturalSelection();
	void mating();
	void mutate();

};
void Model::naturalSelection()
{
	int i = 0;
	while ( i < s.size() )
	{
		if(s[i].isDead())
			s.erase(s.begin() + i);
		else
			i++;
	}

}

void Model::mating()
{
	int len = s.size();
	if(len >= length * 4)
		return;
	for(int i = 0; i + 1 < len; i += 2)
	{
		Snubby s1 = Snubby(string(SNUBBY_IMAGE), s[i].position.first, s[i].position.second, 1, s[i].map);
		Snubby s2 = Snubby(string(SNUBBY_IMAGE), s[i].position.first, s[i].position.second, 1, s[i].map);

		if((rand() % 100) < MATE)
		{
			s1.sim.q1 *= s[i].sim.q1 * LAMBDA + (1 - LAMBDA) * s[i + 1].sim.q1;
			s2.sim.q1 *= s[i + 1].sim.q1 * LAMBDA + (1 - LAMBDA) * s[i].sim.q1;
		}
		if((rand() % 100) < MATE)
		{
			s1.sim.q2 *= s[i].sim.q2 * LAMBDA + (1 - LAMBDA) * s[i + 1].sim.q2;
			s2.sim.q2 *= s[i + 1].sim.q2 * LAMBDA + (1 - LAMBDA) * s[i].sim.q2;
		}
		if((rand() % 100) < MATE)
		{
			s1.sim.q3 *= s[i].sim.q3 * LAMBDA + (1 - LAMBDA) * s[i + 1].sim.q3;
			s2.sim.q3 *= s[i + 1].sim.q3 * LAMBDA + (1 - LAMBDA) * s[i].sim.q3;
		}
		if((rand() % 100) < MATE)
		{
			s1.sim.q4 *= s[i].sim.q4 * LAMBDA + (1 - LAMBDA) * s[i + 1].sim.q4;
			s2.sim.q4 *= s[i + 1].sim.q4 * LAMBDA + (1 - LAMBDA) * s[i].sim.q4;
		}
		if((rand() % 100) < MATE)
		{
			s1.sim.radius *= s[i].sim.radius * LAMBDA + (1 - LAMBDA) * s[i + 1].sim.radius;
			s2.sim.radius *= s[i + 1].sim.radius * LAMBDA + (1 - LAMBDA) * s[i].sim.radius;
		}

		s.push_back(s1);
		s.push_back(s2);
	}

}
void Model::mutate()
{
	for(int i = 0; i < s.size(); i += 2)
	{
		if((rand() % 1000) < MUTATE)
			s[i].sim.q1 *=  (1 + (rand() % 10)) / 5.;
		if((rand() % 1000) < MUTATE)
			s[i].sim.q2 *=  (1 + (rand() % 10)) / 5.;
		if((rand() % 1000) < MUTATE)
			s[i].sim.q3 *=  (1 + (rand() % 10)) / 5.;
		if((rand() % 1000) < MUTATE)
			s[i].sim.q4 *=  (1 + (rand() % 10)) / 5.;
		if((rand() % 1000) < MUTATE)
			s[i].sim.radius *=  (1 + (rand() % 10)) / 5.;

		if(s[i].sim.q1 >= 1)
			s[i].sim.q1 = 1;
		if(s[i].sim.q2 <= -1)
			s[i].sim.q2 = -1;
		if(s[i].sim.q3 >= 1)
			s[i].sim.q3 = 1;
		if(s[i].sim.q4 >= 1)
			s[i].sim.q4 = 1;
		if(s[i].sim.radius >= 100)
			s[i].sim.radius = 100;
	}

}

void Model::initPopulation(vector<vector<data>> *map, int len, vector<Coin> coins)
{
	length = len;
	for (int i = 0; i < len; ++i)
	{
		s.push_back(Snubby(string(SNUBBY_IMAGE),
						   650, 180, 1, map));
		s[i].coins = coins;
	}
}

void Model::updatePopulation()
{
	mating();
	mutate();
	naturalSelection();

	cout << "SIZE: " << s.size() << endl;
	if( s.size() <= length / 2)
		initPopulation(s[0].map, length, s[0].coins);
}