#pragma once

class someText
{
public:
	ALLEGRO_USTR* text;
	int x; int y;
	ALLEGRO_COLOR color;
	ALLEGRO_FONT* font;
	someText(const char* _txt, int _x, int _y, ALLEGRO_COLOR col, ALLEGRO_FONT* fnt)
	{
		text = al_ustr_new(_txt);
		x = _x;
		y = _y;
		color = col;
		font = fnt;

	}
	void draw()
	{
		al_draw_ustr(font, color, x, y, 0, text /*text.c_str()*/);
	}
	~someText()
	{
		//al_destroy_font(font);
	}

};

class Image
{
public:
	ALLEGRO_BITMAP* image;
	int x, y;
	void draw()
	{
		al_draw_bitmap(image, x, y, 0);
	}
	Image(ALLEGRO_BITMAP* im, int _x, int _y)
	{
		x = _x;
		y = _y;
		image = im;
	}
};