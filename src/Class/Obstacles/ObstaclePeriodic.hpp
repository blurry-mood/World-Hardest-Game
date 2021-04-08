#include "Obstacle.hpp"

#define PERIODIC_id -1111

class ObstaclePeriodic: public Obstacle
{
public:
	pair<int, int> center;
	int R;
	int theta;
	int countStop = 0;

	ObstaclePeriodic() {}
	ObstaclePeriodic(pair<int, int> center, int R, int theta)
	{
		this->center = center;
		this->R = R;
		this->theta = theta;
	}

	void save(ofstream &myfile) override
	{
		myfile << PERIODIC_id << "\t";
		myfile << center.first << " " << center.second << " " ;
		myfile << R << " "  << theta << "\n";
	}
	void load(ifstream &myfile) override
	{
		myfile >> center.first;
		myfile >> center.second;
		myfile >> R;
		myfile >> theta;
	}

	pair<int, int> getPosition() override
	{
		return make_pair(round(center.first + R * cos(2 * M_PI * theta / ROTATION_PARTITION)),
						 round(center.second + R * sin(2 * M_PI * theta / ROTATION_PARTITION)));
	}

	void nextMove() override
	{
		if(theta == ROTATION_PARTITION)
			theta = 0;
		if(countStop <= 40 &&
				( theta == ROTATION_PARTITION / 4 || theta == ROTATION_PARTITION / 2
				  || theta == 3 * ROTATION_PARTITION / 4 || theta == 0))
			countStop++;
		else
		{
			countStop = 0;
			theta++;
		}
	}

	static vector<Obstacle *> getPeriodicObstacle(pair<int, int> center, int nbre, int len)
	{
		vector<Obstacle *> rots;
		for (int i = 1; i <= nbre; ++i)
			for (int j = 0; j < 4; ++j)
				rots.push_back(new ObstaclePeriodic(center, len / nbre * i, ROTATION_PARTITION / 4 * j));
		return rots;
	}

};