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
extern double VARY_AMOUNT;

extern ALLEGRO_DISPLAY *currentDisplay;
extern ALLEGRO_COLOR TREECOL;

const double STACK_CHANCE = 0.2;

const double CLOSE_EARLY_CHANCE = 0.3;
const double NEW_SYMBOL_CHANCE = 0.03;
const double ST_NEW_SYMBOL_CHANCE = 0.08;
const unsigned MAX_SYMBOLS = 40;

const unsigned ST_MAX_AXIOMLEN = 10;
const unsigned ST_MIN_AXIOMLEN = 1;
const unsigned ST_MAX_GRAMMS = 1;
const unsigned ST_MIN_GRAMMS = 1;
const double ST_MAX_WALKLEN = 20;
const double ST_MIN_WALKLEN = 1;
const double ST_MAX_WALKVAR = ST_MAX_WALKLEN / 4;
const double ST_MIN_WALKVAR = 0;
const double ST_MAX_ANGLE = M_PI;
const double ST_MIN_ANGLE = M_PI / 20;
const double ST_MAX_ANGLEVAR = ST_MAX_ANGLE / 10;
const double ST_MIN_ANGLEVAR = 0;

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
template<typename T>
inline T randOffset(T limit, T lowest = 0.0)
{
	if (limit <= lowest)
		return ((T)0);
	return ((((T)rand() / RAND_MAX) * (limit - lowest)) + lowest);
}

template<>
inline int randOffset<int>(int limit, int lowest = 0)
{
	if (limit <= lowest)
		return (0);
	return (rand() % (limit - lowest) + lowest);
}

const unsigned RULEABLE_COMMANDS = 5;
enum Command : unsigned
{
	forwardDraw = 0,
	forwardNoDraw = 1,
	rightTurn = 2,
	leftTurn = 3,
	nothing = 4,
	stackCurr,
	popCurr
};

const char STACKSTATE = '[';
const char POPSTATE = ']';

const std::map<char, Command> defaultCommands = 
{
	{'F', forwardDraw},
	{'f', forwardNoDraw},
	{'+', rightTurn},
	{'-', leftTurn},
	{'[', stackCurr},
	{']', popCurr}
};

const std::vector<char> defaultPossibleRules = {'F', 'f', '+', '-'};

struct TurtleState
{
	double x, y;
	double orientation;
	TurtleState()
	{ }
	TurtleState(int _x, int _y, double _ori)
		:x(_x), y(_y), orientation(_ori)
	{ }
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
		std::vector<char> possibleRules;
		double walklength;
		double walkvar;
		double anglechange;
		double anglevar;
		float thickness;
		TurtleState startState;
		ALLEGRO_BITMAP *currentResult;
		double fitness;

		void forward(TurtleState& what);
		void rotate(TurtleState& what, bool way);

		void randomCmds(std::list<char>& dest, std::list<char>::iterator where,
			int length)

	public:
		Lsystem();
		virtual ~Lsystem();
		Lsystem(const Lsystem& other);
		Lsystem& operator=(const Lsystem& other);

		void start();
		void expand();
		void randomize();
		void makeVary(double varyAmount);

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
