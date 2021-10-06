
#include "ball.h"







int main()
{
	parser::initialize();
	parser::parse("./conf.cfg");
	al_init();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_keyboard();
	al_install_mouse();
	//al_init_image_addon();

	ALLEGRO_DISPLAY* disp = al_create_display(parser::SCREENWIDTH, parser::SCREENHEIGHT);
	ALLEGRO_TIMER* tim = al_create_timer(1.0 / parser::maxFPS);
	ALLEGRO_EVENT_QUEUE* que = al_create_event_queue();
	al_register_event_source(que, al_get_timer_event_source(tim));
	ballCon game;
	ALLEGRO_FONT* font = al_load_ttf_font("./font/font.ttf", 10, 0);
	ball::fon = font;
	ALLEGRO_KEYBOARD_STATE st;
	ALLEGRO_MOUSE* ms;
	unsigned long timerShit = 0;
	ALLEGRO_EVENT eve;
	al_register_event_source(que, al_get_mouse_event_source());
	//al_register_event_source(que, al_get_keyboard_event_source());
	al_register_event_source(que, al_get_display_event_source(disp));
	al_start_timer(tim);
	int mX, mY;

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
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			al_get_mouse_cursor_position(&mX, &mY);

			break;
		default:
			break;
		}

		game.checkAll();


		game.drawAll();

		for (int i = 0; i < ballCon::OverlappsX.size(); i+=2)
		{
			al_draw_circle(ballCon::OverlappsX[i], ballCon::OverlappsY[i],4, al_map_rgb(255, 0, 0), 2);
			al_draw_circle(ballCon::OverlappsX[i+1], ballCon::OverlappsY[i+1], 5, al_map_rgb(0, 255, 0), 2);
		}

		al_draw_text(font, al_map_rgb(255, 255, 0), 0, 0, 0, std::to_string(timerShit).c_str());
		al_flip_display();
		al_clear_to_color(al_map_rgb(255, 255, 255));

	skip:
		{}
		++timerShit;
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