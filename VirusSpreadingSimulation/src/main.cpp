
#include "human.h"









int main()
{
	parser::initialize();
	parser::parse("./conf.cfg");
	al_init();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_keyboard();
	al_init_image_addon();

	ALLEGRO_DISPLAY* disp = al_create_display(parser::SCREENWIDTH, parser::SCREENHEIGHT);
	ALLEGRO_TIMER* tim = al_create_timer(1.0 / parser::maxFPS);
	ALLEGRO_EVENT_QUEUE* que = al_create_event_queue();
	al_register_event_source(que, al_get_timer_event_source(tim));
	humCon game;
	if(parser::tradeCenter && parser::tradeCenterDuration)
		game.loadImg(al_load_bitmap("./imgs/marketSign.png"), humCon::tradeZone);
	if (parser::quarantineZone)
		game.loadImg(al_load_bitmap("./imgs/quarantine.png"), humCon::generalZone);
	std::string line;
	std::ifstream file;
	file.open("./font/txt.utf");
	file >> line;
	ALLEGRO_FONT* font = al_load_ttf_font("./font/font.ttf", 20, 0);
	game.loadText( line.c_str() , parser::SCREENWIDTH - 395, parser::SCREENHEIGHT - 375, 
		al_map_rgb(255,255,255), font);
	std::string sec;

	file >> line;
	sec = line;
	file >> line;
	sec += ' ' + line;
	game.loadText(sec.c_str(), parser::SCREENWIDTH - 395, 25, al_map_rgb(255, 255, 255), font);


	file.close();
	ALLEGRO_KEYBOARD_STATE st ;

	ALLEGRO_EVENT eve;
	//al_register_event_source(que, al_get_keyboard_event_source());
	al_register_event_source(que, al_get_display_event_source(disp));
	al_start_timer(tim);
	while (true)
	{
		al_wait_for_event(que, &eve);
		al_get_keyboard_state(&st);
		if (al_key_down(&st, ALLEGRO_KEY_SPACE))
			goto skip;
		switch (eve.type)
		{
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			goto done;
			break;
		default:
			break;
		}
		if (parser::expMovement)
		{
			game.expPhysics();
		}
		else {
			game.physics();
		}
		game.checkAll();

		human::drawZones();
		game.drawImgs();
		game.drawTexts();
		game.drawAll();
		al_flip_display();
		al_clear_to_color(al_map_rgb(21, 21, 21));

	skip:
		{}
	}

done:
	{}
	al_shutdown_font_addon();
	al_shutdown_primitives_addon();
	al_shutdown_ttf_addon();
	al_destroy_display(disp);
	al_destroy_event_queue(que);
	al_destroy_timer(tim);
	al_destroy_font(font);

	return 0;
}