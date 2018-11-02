#ifndef HEAD_H_INCLUDED
# define HEAD_H_INCLUDED

# include <iostream>
# include <sstream>
# include <cstdlib>
# include <vector>
# include <string>
# include <cmath>
# include <stack>
# include <ctime>
# include <list>
# include <map>

# include <allegro5/allegro5.h>
# include <allegro5/allegro_image.h>
# include <allegro5/allegro_primitives.h>

# ifndef NOTEST
#  define E(x) { EINF std::cerr << ": " << #x ": " << (x) << std::endl; }
#  define E2(x) { EINF std::cerr << ": " << #x ": " << (x) << std::endl; }

#  define EINF { std::cerr << "l" << __LINE__ << std::endl; }

#  define ES(x) { std::cerr << std::endl << (x) << std::endl; }
#  define SEPP { std::cerr << "________________________________" << std::endl; }
# endif

# define RESULTDIR "results/"

extern int SEED;

extern int WINDOW_HEIGHT;
extern int WINDOW_WIDTH;
extern float AIM_FILL;
extern float AIM_WALK_HEIGHT;
extern float AIM_MAX_HEIGHT;

extern ALLEGRO_DISPLAY *currentDisplay;
extern ALLEGRO_COLOR TREECOL;

inline double mod2PI(double orientation)
{
	if (orientation >= (2 * M_PI))
		return (orientation - (2 * M_PI) * (((int)(orientation * M_1_PI)) / 2));
	else if (orientation < 0)
		return (orientation - (2 * M_PI) * (-1 + (((int)(orientation * M_1_PI)) / 2)));
	else
		return (orientation);
}

// may change this to a gauss or something
inline double randOffset(double limit)
{
	return (((double)rand() / RAND_MAX) * limit);
}

enum Command
{
	forwardDraw,
	forwardNoDraw,
	rightTurn,
	leftTurn,
	stackCurr,
	popCurr,
	nothing
};

const std::map<char, Command> defaultCommands = 
{
	{'F', forwardDraw},
	{'f', forwardNoDraw},
	{'+', rightTurn},
	{'-', leftTurn},
	{'[', stackCurr},
	{']', popCurr}
};

struct TurtleState
{
	double x, y;
	double orientation;
};

// turtle system, a turtle with a pencil attached to it's tail
class Lsystem
{
	friend int main(int argc, char **argv);

	protected:
		std::list<char> commands;
		std::list<char> axiom;
		std::map<char, std::list<char>> grammars;
		std::map<char, Command> charToCmd;
		ALLEGRO_BITMAP *currentResult;
		double exceeding;
		double walklength;
		double walkvar;
		double anglechange;
		double anglevar;
		float thickness;
		TurtleState startState;

		void forward(TurtleState& what);
		void rotate(TurtleState& what, bool way);

	public:
		Lsystem();
		virtual ~Lsystem();

		void start();
		void expand();
		void draw();

		void makeTest(int type = 0);
		ALLEGRO_BITMAP *getresult() { return (currentResult); }
};

std::ostream& operator<<(std::ostream& theStream, std::list<char> what);

void getParams(int argc, char **argv);

void initAlleg(int flags, int w, int h, const char* window_name = "");
void closeAlleg();

ALLEGRO_BITMAP *showResult();
void pauseUntilEsc();
void saveToF(ALLEGRO_BITMAP *what, std::string title);
void gettitle(std::string& dest, int seed);

#endif
