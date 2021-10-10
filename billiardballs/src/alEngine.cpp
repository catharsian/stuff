#include "alEngine.h"


void baseEngine::initialize()
{
	parser::initialize();
	parser::parse("./conf.cfg");
	if (!al_init())
	{
		std::cerr << "FUCK";
		return;
	}
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_keyboard();
	al_install_mouse();
}

baseEngine::baseEngine()
{
	redraw = true;
}
baseEngine::~baseEngine()
{
	al_shutdown_font_addon		();
	al_shutdown_primitives_addon();
	al_shutdown_ttf_addon		();
	al_destroy_font				(mainFont);
	al_destroy_timer			(tim);
	al_destroy_timer			(sec);
	al_destroy_event_queue		(que);
	al_destroy_display			(disp);
	al_uninstall_keyboard		();
	al_uninstall_mouse			();
	
}