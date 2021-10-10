#pragma once
#include "ball.h"

class baseEngine
{
protected:
	ALLEGRO_DISPLAY*	   disp;
	ALLEGRO_TIMER*		   tim;
	ALLEGRO_TIMER*		   sec;
	ALLEGRO_EVENT_QUEUE*   que;
	ALLEGRO_FONT*		   mainFont;
	ALLEGRO_MOUSE*		   mouse;
	ALLEGRO_KEYBOARD_STATE st;
	ALLEGRO_EVENT		   eve;
	ALLEGRO_FONT*		   largerFont;
	ALLEGRO_MOUSE_STATE    mSt;
	bool redraw;
public: 
	baseEngine();
	~baseEngine();

	static void initialize();


	virtual void Start () = 0;
	virtual bool Update() = 0;
	

};

