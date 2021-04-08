#ifndef OOOOOOOO
#define OOOOOOOO


class Obstacle
{
public :
	virtual pair<int, int> getPosition() = 0;
	virtual void nextMove() = 0;
	virtual void save(ofstream &myfile) = 0;
	virtual void load(ifstream &myfile) = 0;
};



#endif