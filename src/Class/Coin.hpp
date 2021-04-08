class Coin
{
public:
	pair<int, int> position;
	bool isVirtual;
	Coin(int x, int y)
	{
		isVirtual = false;
		position = make_pair(x, y);
	}
};