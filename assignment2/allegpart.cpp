#include "head.h"

using namespace std;

ALLEGRO_DISPLAY *currentDisplay = nullptr;

void initAlleg(int flags, int w, int h, const char* window_name)
{
	if (!al_init())
		throw "Could not initialize Allegro 5";
	if (!al_init_image_addon())
		throw "Image addon init error";
	if (!al_init_primitives_addon())
		throw "Primitives init error";
	if (!al_install_keyboard())
		throw "Keyboard init error";
	if (!al_install_mouse())
		throw "Mouse init error";

	if (w != 0 && h != 0)
	{
		if (w < MINDISPW)
			w = MINDISPW;
		al_set_new_display_flags(flags); //ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE, or ALLEGRO_FULLSCREEN_WINDOW
		currentDisplay = al_create_display(w, h);
		if (!currentDisplay)
			throw "Could not create display";
		al_set_window_title(currentDisplay, window_name);
	}
}

void closeAlleg()
{
	al_destroy_display(currentDisplay);
}

ALLEGRO_BITMAP *showResult(map<int, map<int, Tile>>& sol, int ignoreLvl)
{
	ALLEGRO_BITMAP *rep;

	al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
	rep = al_create_bitmap(SIDE, SIDE);

	al_set_target_bitmap(rep);
	al_lock_bitmap(rep, ALLEGRO_PIXEL_FORMAT_ANY, 0);

	for (const auto& elemx : sol)
	{
		for (const auto& elemy : elemx.second)
		{
			al_put_pixel(elemx.first, elemy.first, elemy.second.getcol());
		}
	}
	al_unlock_bitmap(rep);
	if (ignoreLvl < 2)
	{
		al_set_target_backbuffer(currentDisplay);
		al_draw_bitmap(rep, 0, 0, 0);
		al_flip_display();
	}
	return (rep);
}

void pauseUntilEsc()
{
	ALLEGRO_EVENT_QUEUE *eventsKeyboard;
	ALLEGRO_EVENT ret_event;

	eventsKeyboard = al_create_event_queue();
	al_register_event_source(eventsKeyboard, al_get_keyboard_event_source());
	
	while(true)
	{
		al_wait_for_event(eventsKeyboard, &ret_event);
		if (ret_event.type == ALLEGRO_EVENT_KEY_DOWN &&
			ret_event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			break;
	}
}

void gettitle(string& dest, int seed)
{
	stringstream ss;

	ss << "st:" << STARTATTEN << " dif:" << DIFFATEN << " sm:" << SMOOTHNB
	<< " c:" << SIDE << " v:" << VALNB << " seed:" << seed;
	dest = ss.str();
}

void saveToF(ALLEGRO_BITMAP *what, int seed, int num)
{
	stringstream ss;
	string str;

	gettitle(str, seed);
	ss << RESULTDIR << str << " " << num << ".png";
	str = ss.str();
	al_save_bitmap(str.c_str(), what);
}
