#include "Obstacle.hpp"

#define SQAURE_id -111

class ObstacleSquare: public Obstacle
{
public:
	vector<pair<int, int>> square;
	float lambda;
	int index;

	ObstacleSquare(){}
	ObstacleSquare(vector<pair<int, int>> pts, float lambda, int index)
	{
		square = pts;
		this->lambda = (1 > lambda >= 0) ? lambda : 0;
		this->index = (pts.size() > index >= 0) ? index : 0;
	}

	void save(ofstream &myfile) override
	{
		myfile << SQAURE_id << "\t";
		myfile << (int)square.size() << "\t";
		for(auto center : square)
			myfile << center.first << " " << center.second << " ";
		myfile << lambda << " "  << index<<"\n";
	}
	void load(ifstream &myfile) override
	{
		int size;
		myfile >> size;
		for (int i = 0; i < size; ++i)
		{
			pair<int, int> pt;
			myfile >> pt.first;
			myfile >> pt.second;
			square.push_back(pt);
		}
		myfile >> lambda;
		myfile >> index;
	}

	pair<int, int> getPosition() override
	{
		return make_pair(round((1 - lambda) * square[index].first + lambda * square[(1 + index) % square.size()].first),
						 round((1 - lambda) * square[index].second +  lambda * square[(1 + index) % square.size()].second));
	}

	void nextMove() override
	{
		lambda = lambda + TRANSLATION_STEP;
		if(lambda >= 1 )
		{
			lambda = 0;
			index++;
			index %= square.size();
		}
	}
};