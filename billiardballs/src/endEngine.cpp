#include "endEngine.h"





static void get_fps(int* average, int* minmax)
{
	int i;
	int prev = parser::maxFPS - 1;
	double min_dt = 1;
	double max_dt = 1 / 1000000.0;
	double av = 0;
	double d;
	for (i = 0; i < parser::maxFPS; i++) {
		if (i != 0) {
				double dt = 0;
			if (dt < min_dt)
				min_dt = dt;
			if (dt > max_dt)
				max_dt = dt;
			av += dt;
		}
	prev = i;	
	}
	av /= (parser::maxFPS - 1);
	* average = ceil(1 / av);
	d = 1 / min_dt - 1 / max_dt;
	* minmax = floor(d / 2);
}

endEngine::endEngine()
{
	//timerShit = 0;
	mouseHeld = false;
	mX = 0;
	mY = 0;
	poses.Create(5);
}
//Time gameTime;

void endEngine::Start()
{

	disp = al_create_display(parser::SCREENWIDTH, parser::SCREENHEIGHT);
	tim = al_create_timer(float(1) / float(parser::maxFPS));
	sec = al_create_timer(1);
	que = al_create_event_queue();
	al_register_event_source(que, al_get_display_event_source(disp));
	al_register_event_source(que, al_get_timer_event_source(tim));
	//al_register_event_source(que, al_get_mouse_event_source());
	//al_register_event_source(que, al_get_keyboard_event_source());
	al_start_timer(tim);
	al_start_timer(sec);
	mainFont = al_load_font("./font/font.ttf", -14, 0);
	largerFont = al_load_font("./font/font.ttf", -30, 0);
	ball::fon = mainFont;
	
	again:
	gameTime.running = al_get_time();
	if (Update()) {
		gameTime.delta = al_get_time() - gameTime.running;
		goto again;
	}
	
	delete this;
}

bool endEngine::Update()
{
	al_wait_for_event(que, &eve);
	al_get_keyboard_state(&st);
	al_get_mouse_state(&mSt);
	if (eve.type == ALLEGRO_EVENT_DISPLAY_CLOSE) return false;
	if (al_key_down(&st, ALLEGRO_KEY_SPACE))
	{
		return true;
	}
	if (mSt.buttons & 1 && !mouseHeld)
	{
		mouseHeld = true;

		con.findOne(mSt.x, mSt.y);
		
	}
	else
	if (!(mSt.buttons & 1) && mouseHeld)
	{
		mouseHeld = false;
		if (con.whoIsHeld != nullptr)
			con.whoIsHeld->changeDir(poses.getDiff());
		
		con.whoIsHeld = nullptr;
	}



	con.checkAll();
	if (con.whoIsHeld != nullptr)
	{
		poses.append(Eigen::Vector2f(mSt.x,mSt.y));
		
		con.whoIsHeld->setPos(mSt.x,mSt.y);
	}


	con.drawAll();

	al_draw_text(largerFont, al_map_rgb(170, 100, 0), 0, 0, 0, (std::string("FPS: ") + std::to_string(static_cast<int>(1.0 / gameTime.delta + 0.5))).c_str());
	al_flip_display();
	al_clear_to_color(al_map_rgb(255, 255, 255));
	return true;
}

endEngine::~endEngine()
{
}