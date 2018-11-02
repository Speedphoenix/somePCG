#include "head.h"

using namespace std;

Lsystem::Lsystem()
	:commands(), axiom(), grammars(), charToCmd(defaultCommands),
	currentResult(nullptr), exceeding(0.0), walklength(20), walkvar(1),
	anglechange(M_PI / 6), anglevar(M_PI / 16), thickness(1.0)
{
	startState.x = 0;
	startState.y = 0;
	startState.orientation = M_PI_2;
}

Lsystem::~Lsystem()
{
	if (currentResult != nullptr)
		al_destroy_bitmap(currentResult);
}

ostream& operator<<(ostream& theStream, list<char> what)
{
	for (auto elem : what)
		theStream << elem;
	return (theStream);
}

void listAdd(list<char>& where, const char* what)
{
	while (*what != '\0')
	{
		where.push_back(*what);
		what++;
	}
}

void Lsystem::makeTest(int type)
{
	axiom.clear();
	grammars.clear();
	walklength = 10;
	walkvar = 0;
	anglechange = M_PI / 6;
	anglevar = 0;
	thickness = 1.0;
	charToCmd = defaultCommands;
	startState.x = WINDOW_WIDTH / 2;
	startState.y = WINDOW_HEIGHT;
	startState.orientation = M_PI_2;
	switch (type)
	{
	default:
	case 0:
		listAdd(axiom, "F");
		//F -> F[-F]F[+F][F]
		listAdd(grammars['F'], "F[-F]F[+F][F]");
		break;
	case 1:
		listAdd(axiom, "X");
		listAdd(grammars['X'], "F+[[X]-X]-F[-FX]+X");
		listAdd(grammars['F'], "FF");
		walklength = 5;
		anglechange = 0.436332;
		charToCmd['X'] = nothing;
		startState.x = 0;
		startState.orientation = M_PI_4;
		break;
	case 2:
		listAdd(axiom, "F-G-G");
		listAdd(grammars['F'], "F-G+F+G-F");
		listAdd(grammars['G'], "GG");
		walklength = 7;
		anglechange = 2.0944;
		charToCmd['G'] = forwardDraw;
		startState.x = 0;
		startState.orientation = 0;
		break;
	}
}

void Lsystem::forward(TurtleState& what)
{
	double dist = walklength + randOffset(walkvar);
	what.x += cos(what.orientation) * dist;
	what.y -= sin(what.orientation) * dist;
}

void Lsystem::rotate(TurtleState& what, bool way)
{
	what.orientation += (anglechange + randOffset(anglevar)) * (way ? -1 : 1);
}

void Lsystem::start()
{
	commands = axiom;
}

void Lsystem::expand()
{
	list<char>::iterator it = commands.begin();
	list<char>::iterator inter;

	while (it != commands.end())
	{
		inter = it;
		it++;
		if (grammars.count(*inter) > 0)
		{
			const list<char>& toAdd = grammars.at(*inter);
			commands.insert(inter, toAdd.begin(), toAdd.end());
			commands.erase(inter);
		}
	}
}

void Lsystem::draw()
{
	list<char>::iterator it;
	stack<TurtleState> comeBackTo;
	TurtleState currState, inter;

	if (currentResult == nullptr)
	{
		al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
		currentResult = al_create_bitmap(WINDOW_WIDTH, WINDOW_HEIGHT);
	}
	al_set_target_bitmap(currentResult);
	al_clear_to_color(al_map_rgb(255, 255, 255));
	currState = startState;
	for (it = commands.begin();it != commands.end();it++)
	{
		switch (charToCmd.at(*it))
		{
		case forwardDraw:
			inter = currState;
			forward(currState);
			al_draw_line(inter.x, inter.y, currState.x, currState.y, TREECOL,
				thickness);
			break;

		case forwardNoDraw:
			forward(currState);
			break;

		case rightTurn:
			rotate(currState, true);
			break;

		case leftTurn:
			rotate(currState, false);
			break;

		case stackCurr:
			comeBackTo.push(currState);
			break;

		case popCurr:
			currState = comeBackTo.top();
			comeBackTo.pop();
			break;
		case nothing:
			break;
		default:
			throw "hey, that's not an accepted command";
			break;
		}
	}
}
