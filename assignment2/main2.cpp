#include "head.h"

using namespace std;

int SIDE = 513;

int NEIGHB = 1;

int VALNB = 3;

int SMOOTHNB = 0;

float STARTATTEN = 1.00;
float DIFFATEN = 1.15;

void square(map<int, map<int, Tile>>& sol, int x, int y, int diff, float atten)
{
	vector<Tile> tab;
	if (sol.count(x) && sol.at(x).count(y))
		return;
	sol[x][y] = Tile(atten);
	if (sol.count(x - diff) && sol.at(x - diff).count(y))
		tab.push_back(sol.at(x - diff).at(y));
	if (sol.count(x + diff) && sol.at(x + diff).count(y))
		tab.push_back(sol.at(x + diff).at(y));
	if (sol.count(x))
	{
		if (sol.at(x).count(y - diff))
			tab.push_back(sol.at(x).at(y - diff));
		if (sol.at(x).count(y + diff))
			tab.push_back(sol.at(x).at(y + diff));
	}
	sol[x][y].med(tab);
}

void diamonds(map<int, map<int, Tile>>& sol, int top, int bot, int lef, int rig,
				float attenuation)
{
	vector<Tile> tab;
	float newAtten = attenuation;
	int halfSide = (rig - lef) / 2;
	int ver = lef + halfSide;
	int hor = top + halfSide;

	sol[ver][hor] = Tile(attenuation);
	tab.push_back(sol[lef][top]);
	tab.push_back(sol[lef][bot]);
	tab.push_back(sol[rig][top]);
	tab.push_back(sol[rig][bot]);
	sol[ver][hor].med(tab);

	newAtten *= DIFFATEN;

	square(sol, ver, top, halfSide, newAtten);
	square(sol, ver, bot, halfSide, newAtten);
	square(sol, lef, hor, halfSide, newAtten);
	square(sol, rig, hor, halfSide, newAtten);

/*	use these for funky results
	square(sol, top, ver, halfSide, newAtten);
	square(sol, bot, ver, halfSide, newAtten);
	square(sol, hor, lef, halfSide, newAtten);
	square(sol, hor, rig, halfSide, newAtten);
*/

	if (halfSide <= 1)
		return;

	diamonds(sol, top, hor, lef, ver, newAtten);
	diamonds(sol, top, hor, ver, rig, newAtten);
	diamonds(sol, hor, bot, lef, ver, newAtten);
	diamonds(sol, hor, bot, ver, rig, newAtten);
}

void generateAll(map<int, map<int, Tile>>& sol, int rndseed)
{
	srand(rndseed);
	sol[0][0]				= Tile(STARTATTEN);
	sol[0][SIDE - 1]		= Tile(STARTATTEN);
	sol[SIDE - 1][0]		= Tile(STARTATTEN);
	sol[SIDE - 1][SIDE - 1]	= Tile(STARTATTEN);

	diamonds(sol, 0, SIDE - 1, 0, SIDE - 1, STARTATTEN * DIFFATEN);
}

void smooth(map<int, map<int, Tile>>& sol)
{
	vector<Tile> tab;
	for (auto& elemx : sol)
	{
		for (auto& elemy : elemx.second)
		{
			tab.clear();
		 	for (int i = -1;i <= 1;i++)
			{
				for (int j = -1;j <= 1;j++)
				{
					if (sol.count(elemx.first + i)
						&& sol.at(elemx.first + i).count(elemy.first + j))
					{
						tab.push_back(sol.at(elemx.first + i).at(elemy.first + j));
					}
				}
			}
			elemy.second.med(tab, false);
		}
	}
}

int main(int argc, char **argv)
{
	ALLEGRO_BITMAP *btm;
	map<int, map<int, Tile>> sol;
	int seed, ignoreDisp;
	string theString;
	stringstream ss;

	for (int i = 1;i < argc;i++)
		ss << argv[i] << " ";
	if (argc >= 2)
		ss >> STARTATTEN;
	if (argc >= 3)
		ss >> DIFFATEN;
	if (argc >= 4)
		ss >> SMOOTHNB; 
	if (argc >= 5)
		ss >> SIDE;
	if (argc >= 6)
		ss >> VALNB;
	if (argc >= 7)
		ss >> seed;
	else
		seed = time(nullptr);
	if (argc >= 8)
		ss >> ignoreDisp;
	else
		ignoreDisp = 0;

	gettitle(theString, seed);

	if (ignoreDisp < 2)
		initAlleg(ALLEGRO_WINDOWED, SIDE, SIDE, theString.c_str());
	else
		initAlleg(0, 0, 0, "");

	cout << endl << "Using seed: " << seed << endl;
	generateAll(sol, seed);
	if (!ignoreDisp)
		btm = showResult(sol, ignoreDisp);
	for (int i = 0;i < SMOOTHNB;i++)
	{
		if (!ignoreDisp)
			al_destroy_bitmap(btm);
		smooth(sol);
		if (!ignoreDisp)
			btm = showResult(sol, ignoreDisp);
	}
	if (ignoreDisp)
		btm = showResult(sol, ignoreDisp);
	saveToF(btm, seed, SMOOTHNB);
	if (ignoreDisp < 2)
		pauseUntilEsc();
	al_destroy_bitmap(btm);

	closeAlleg();
	return (0);
}
