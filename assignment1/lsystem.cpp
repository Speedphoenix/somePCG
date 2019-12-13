#include "head.h"

using namespace std;

Lsystem::Lsystem()
	:commands(), axiom(), grammars(), charToCmd(defaultCommands),
	possibleRules(defaultPossibleRules), walklength(20), walkvar(1),
	anglechange(M_PI / 6), anglevar(M_PI / 16), thickness(1.0),
	startState(0, 0, M_PI_2),
	currentResult(nullptr), fitness(0.0)
{

}

Lsystem::~Lsystem()
{
	if (currentResult != nullptr)
		al_destroy_bitmap(currentResult);
}

Lsystem(const Lsystem& other)
	:commands(other.commands), axiom(other.axiom), grammars(other.grammars),
	charToCmd(other.charToCmd), possibleRules(other.possibleRules),
	thickness(other.thickness), walklength(other.walklength),
	walkvar(other.walkvar), anglechange(other.anglechange),
	anglevar(other.anglevar), startState(other.startState),
	currentResult(nullptr), fitness(0)
{
	//you need to calculate fitness and draw again
}

Lsystem& operator=(const Lsystem& other)
{
	if (&other == this)
		return (*this);
	if (currentResult != nullptr)
		al_destroy_bitmap(currentResult);
	:commands = other.commands;
	axiom = other.axiom;
	grammars = other.grammars;
	charToCmd = other.charToCmd;
	possibleRules(other.possibleRules), 
	thickness = other.thickness;
	walklength = other.walklength;
	walkvar = other.walkvar;
	anglechange = other.anglechange;
	anglevar = other.anglevar;
	startState = other.startState;
	fitness = other.fitness;
	currentResult = al_clone_bitmap(other.currentResult);
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

void Lsystem::randomCmds(list<char>& dest, list<char>::iterator where, int length)
{
	int openedCount = 0;
	char newAddition;

	while (length > 0)
	{
		if (length <= openedCount)
		{
			newAddition = POPSTATE;
			openedCount--;
		}
		else if (length > 2 && randOffset(1.0, 0.0) > STACK_CHANCE)
		{
			//if we add opening or closing brackets

			if (length > openedCount + 1
				&& (openedCount <= 0 || randOffset(0.0, 1) < 0.5))
			{
				newAddition = STACKSTATE;
				openedCount++;
			}
			else // if (openedCount > 0)
			{
				newAddition = POPSTATE;
				openedCount--;
			}
		}
		else
			newAddition = possibleRules.at(randOffset(possibleRules.size(), 0));
		dest.insert(where, newAddition);
		length--;
	}
}

void Lsystem::randomize()
{
	int length, addedvals;
	int i;

	//randomize bland values
	startState = TurtleState(0, 0, M_PI_2);
	walklength = randOffset(ST_MAX_WALKLEN, ST_MIN_WALKLEN);
	walkvar = randOffset(ST_MAX_WALKVAR, ST_MIN_WALKVAR);
	anglechange = randOffset(ST_MAX_ANGLE, ST_MIN_ANGLE);
	anglevar = randOffset(ST_MAX_ANGLEVAR, ST_MIN_ANGLEVAR);

	//randomly add other symbols/variables
	i = 1;
	while (randOffset<double>(1.0, 0.0) < ST_NEW_SYMBOL_CHANCE
		&& i < MAX_SYMBOLS)
	{
		i++;
		char newone = randOffset<char>(26, 0);
		newone += 'a';
		if (charToCmd.count(newone) != 0)
		{
			newone += 'A' - 'a';
			if (charToCmd(newone) != 0)
				continue;
		}
		charToCmd[newone] = randOffset(RULEABLE_COMMANDS, 0);
		possibleRules.push_back(newone);
	}

	//randomize axiom length
	length = randOffset(ST_MAX_AXIOMLEN, ST_MIN_AXIOMLEN);
	randomCmds(axiom, axiom.end(), length);

	//randomize number of grammars (keep it under number of symbols)
	//for each grammar randomize its length
}

void makeVary(double varyAmount)
{

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
