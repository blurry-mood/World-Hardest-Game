#include "Obstacle.hpp"

#define ROTATION_id -11

class ObstacleRotation: public Obstacle
{
public:
	pair<int, int> center;
	int R;
	int theta;

	ObstacleRotation() {}
	ObstacleRotation(pair<int, int> center, int R, int theta)
	{
		this->center = center;
		this->R = R;
		this->theta = theta;
	}

	void save(ofstream &myfile) override
	{
		myfile << ROTATION_id << "\t";
		myfile << center.first << " " << center.second << " ";
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
		theta++;
	}

	static 	vector<Obstacle *> getSpiralObstacle(pair<int, int> center, int nbre, int len)
	{
		vector<Obstacle *> rots;
		for (int i = 1; i <= nbre; ++i)
			for (int j = 0; j < 4; ++j)
				rots.push_back(new ObstacleRotation(center, len / nbre * i, ROTATION_PARTITION / 4 * j));
		return rots;
	}

};