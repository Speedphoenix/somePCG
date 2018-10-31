#ifndef HEAD_H_INCLUDED
# define HEAD_H_INCLUDED

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <ctime>
#include <map>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

# define E(x) //{ EINF std::cerr << ": " << #x ": " << (x) << std::endl; }
# define E2(x) { EINF std::cerr << ": " << #x ": " << (x) << std::endl; }

# define EINF //{ std::cerr << "l" << __LINE__ << std::endl; }

# define ES(x) //{ std::cerr << std::endl << (x) << std::endl; }

# define RESULTDIR "results/"

const int RANGE = 256;
const int MINDISPW = 420;

extern int SIDE;

extern float STARTATTEN;
extern float DIFFATEN;
extern int VALNB;
extern int SMOOTHNB;

extern ALLEGRO_DISPLAY *currentDisplay;

struct Tile
{
	std::vector<unsigned> vals;
	float usedAttenuation;

	Tile(float attenuation = 1)
		:usedAttenuation(attenuation)
	{
		for (int i = 0;i < VALNB;i++)
			vals.push_back((rand() % RANGE));// / attenuation);
	}

	void med(const std::vector<Tile>& tab, bool useatten = true)
	{
		for (const auto& elem : tab)
			for (int i = 0;i < VALNB;i++)
				vals.at(i) += elem.vals.at(i);
		for (int i = 0;i < VALNB;i++)
			vals.at(i) /= (tab.size()) + (useatten ? (1 / usedAttenuation) : 1);
	}

	ALLEGRO_COLOR getcol() const
	{
		switch (VALNB)
		{
			case 0:
			return (al_map_rgb(0, 0, 0));
		break;
			case 1:
			return (al_map_rgb(vals.at(0), vals.at(0), vals.at(0)));
		break;
			case 2:
			return (al_map_rgb(vals.at(0), (vals.at(0) + vals.at(1)) / 2, vals.at(1)));
		break;
			default:
			case 3:
			return (al_map_rgb(vals.at(0), vals.at(1), vals.at(2)));
		break;
			case 4:
			return (al_map_rgba(vals.at(0), vals.at(1), vals.at(2), vals.at(3)));
		break;
		}
	}
};

void initAlleg(int flags, int w, int h, const char* window_name = "");
void closeAlleg();

ALLEGRO_BITMAP *showResult(std::map<int, std::map<int, Tile>>& sol, int ignoreLvl);
void pauseUntilEsc();
void saveToF(ALLEGRO_BITMAP *what, int seed, int num = 0);
void gettitle(std::string& dest, int seed);

#endif
