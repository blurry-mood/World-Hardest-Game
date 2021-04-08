#include "Obstacle.hpp"

#define TRANSLATION_id -1

class ObstacleTranslation: public Obstacle
{
public:

	pair<int, int> a, b;
	float lambda;
	int sens;

	ObstacleTranslation() {}
	ObstacleTranslation(pair<int, int> a, pair<int, int>b, float lambda, int sens)
	{
		this->a = a;
		this->b = b;
		this->lambda = lambda;
		this->sens = sens;
	}

	void save(ofstream &myfile) override
	{
		myfile <<TRANSLATION_id<<"\t";
		myfile << a.first << " " << a.second << " ";
		myfile << b.first << " " << b.second << " ";
		myfile << lambda << " "  << sens<<"\n";
	}
	void load(ifstream &myfile) override
	{
		myfile >> a.first ;
		myfile >> a.second ;
		myfile >> b.first ;
		myfile >> b.second ;
		myfile >> lambda;
		myfile >> sens;
	}

	pair<int, int> getPosition() override
	{
		return make_pair(round((1 - lambda) * a.first + lambda * b.first),
						 round((1 - lambda) * a.second + lambda * b.second));
	}

	void nextMove() override
	{
		lambda = lambda + sens * TRANSLATION_STEP;
		if(lambda > 1 || lambda < 0)
			sens *= -1;
	}
};