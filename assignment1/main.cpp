#include "head.h"

using namespace std;

int SEED = 0;

int WINDOW_HEIGHT = 500;
int WINDOW_WIDTH = 500;
float AIM_FILL = 0.5;
float AIM_WALK_HEIGHT = 75;
float AIM_MAX_HEIGHT = 500;
double VARY_AMOUNT = 10;

void getParams(int argc, char **argv)
{
	stringstream ss;

	for (int i = 1;i < argc;i++)
		ss << argv[i] << " ";
	if (argc >= 2)
		ss >> AIM_FILL;
	if (argc >= 3)
		ss >> AIM_WALK_HEIGHT;
	if (argc >= 4)
		ss >> WINDOW_HEIGHT;
	if (argc >= 5)
		ss >> WINDOW_WIDTH;
	if (argc >= 6)
		ss >> AIM_MAX_HEIGHT;
	else
		AIM_MAX_HEIGHT = WINDOW_HEIGHT;
	if (argc >= 7)
		ss >> SEED;
	else
		SEED = time(nullptr);
}

int main(int argc, char **argv)
{
	Lsystem sys;
	string theString;

	getParams(argc, argv);

	initAlleg(ALLEGRO_WINDOWED, WINDOW_WIDTH, WINDOW_HEIGHT, theString.c_str());
	//initAlleg(0, 0, 0, "");

	cout << endl << "Using seed: " << SEED << endl;
	srand(SEED);

	for (int j = 0;j < 3;j++)
	{
		sys.makeTest(j);
		sys.start();
		for (int i = 1;i < 8;i++)
		{
			sys.expand();
			sys.draw();
			al_set_target_backbuffer(currentDisplay);
			al_draw_bitmap(sys.getresult(), 0, 0, 0);
			al_flip_display();
			pauseUntilEsc();
		}
	}

	closeAlleg();
	return (0);
}
