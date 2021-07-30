#include "atlas.h"
#include "minesweeper.h"
#include "externalLibraries.h"
#include <future>





int INDEX(int x, int y, int width)
{
	return x + (y * width);
}


float pythagoras(float x, float y)
{
	float z = x * x + y * y;
	z = std::sqrtf(z);
	return z;
}

bool faceButton(int x, int y, int screenwidth)
{
	if (y > 0x10 && y < 0x30 && x > (screenwidth / 2) && x < ((screenwidth / 2) + 0x20))
	{
		return true;
	}
	else
	{
		return false;
	}
}



int main()
{
	al_init();
	al_install_mouse();
	al_install_keyboard();
	al_init_font_addon();
	al_init_primitives_addon();
	al_init_image_addon();
	al_init_ttf_addon();
	bool done = false; // close program
	ALLEGRO_EVENT event1;
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	ALLEGRO_DISPLAY* menu = al_create_display(400,300);
	al_register_event_source(queue, al_get_mouse_event_source());
	bool held = false;
	ALLEGRO_MOUSE_STATE mouse;
	ALLEGRO_FONT* font1 = al_load_ttf_font("./fonts/OpenSans.ttf",20,0);
	al_clear_to_color(al_map_rgb(255,255,255));
	al_draw_rectangle(0,0,400,100,al_map_rgb(0,255,0),5);
	al_draw_rectangle(0, 100, 400, 200, al_map_rgb(255, 255, 0), 5);
	al_draw_rectangle(0, 200, 400, 300, al_map_rgb(255, 0, 0), 5);
	al_draw_text(font1,al_map_rgb(0,0,0),100,43,0,"Easy(15x15, 80 mines)");
	al_draw_text(font1, al_map_rgb(0,0,0), 100, 143, 0, "Normal(30x25, 200 mines)");
	al_draw_text(font1, al_map_rgb(0,0,0), 100, 243, 0, "Hard(50x25, 500 mines)");
	al_flip_display();
	int x, y, m;
	while (1)
	{
		al_wait_for_event(queue, &event1);
		al_get_mouse_state(&mouse);
		if ((mouse.buttons & 1) && !held) {
			held = true;
		}
		if (!(mouse.buttons & 1) && held)
		{
			if (event1.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
				return 0;
			if (mouse.y > 0 && mouse.y < 100)
			{
				x = 15; y = 15; m = 80;
				break;
			}
			if (mouse.y > 100 && mouse.y < 200)
			{
				x = 30; y = 25; m = 200;
				break;
			}
			if (mouse.y > 200 && mouse.y < 300)
			{
				x = 50; y = 25; m = 500;
				break;
			}
		}
	}
	held = false;
	al_destroy_display(menu);

	done = false;

	atl.setNewWH(32, 32);
	atl.load_image("./textures/minesweeper.png");
	CellObserver cells(x, y, m);
	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0); // 30 fps lock
	if (!timer)
	{
		printf("couldn't initialize timer\n");
		return 1;
	}

	ALLEGRO_DISPLAY* disp = al_create_display(x * Cell::aCell, y * Cell::aCell + Cell::aCell * 2);

	ALLEGRO_FONT* font = al_create_builtin_font();

	al_register_event_source(queue, al_get_display_event_source(disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));


	 // if true -> redraw the screen(to increase the performance as there's no need to draw it always)
	bool shet = true;
	ALLEGRO_KEYBOARD_STATE key1;
	bool first = true;
	bool right = false;
	//al_start_timer(timer);
	while (1)
	{
		al_wait_for_event(queue, &event1);
		al_get_mouse_state(&mouse);
		if (((mouse.buttons & 1) || (mouse.buttons & 2)) && !held) {
			held = true;
			if (mouse.buttons & 2) right = true;
		}
		if ((!(mouse.buttons & 1) && !(mouse.buttons & 2)) && held)
		{
			held = false;
			if (faceButton(mouse.x, mouse.y, al_get_display_width(disp)))
			{
				std::thread gg([&cells]() { cells.Degenerate(); });
				al_draw_text(font1,al_map_rgb(255,0,0),0,0,0,"RELOADING...");
				lost = false;
				shet = true;
				first = true;
				al_flip_display();
				gg.join();
				goto shit;
			}
			int x1 = mouse.x / Cell::aCell; // get x in cells
			int y1 = mouse.y / Cell::aCell - 2; // get y in cells
			//std::cout << "x = " << x1 << "; y = " << y << "; INDEX = " << INDEX(x1, y, x) << ';' << std::endl; // DEBUG
			//if (cells.safeGet(INDEX(x1,y,x)))
			//{
			if (first)
			{
				if (cells.INDEX(x1, y1) != -1)
				{
					cells.generateMines(x1, y1);
					cells.remakeNears();
					cells.pressCell(x1,y1);
					first = false;
				}
				goto shit;
			}
			if (right)
			{
				cells.tryMark(x1, y1);
			}
			else
			{
				cells.pressCell(x1, y1);
			}
			right = false;
			//redraw = true;
				
			//}
			
		}
		switch (event1.type)
		{
		case ALLEGRO_EVENT_TIMER:
			// game logic
			//i++;
			//redraw = true;
			break;

		case ALLEGRO_EVENT_KEY_DOWN:
			break;
		case ALLEGRO_EVENT_KEY_UP:
			break;
		
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			done = true;
			break;
		}
		if (done) break;
		if (lost && shet)
		{
			redraw = true;
			shet = false;
		}
		if (redraw /* && al_is_event_queue_empty(queue)*/)
		{
			shit:
			al_clear_to_color(al_map_rgb(255, 255, 255));
			atl.DrawTile(12 + int(lost) + int(cells.won),0,al_get_display_width(disp) / 2, Cell::aCell / 2);

			cells.DrawEverything();
			al_flip_display();
			redraw = false;
		}
	}
	al_destroy_font(font);
	al_destroy_display(disp);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);
	return 0;
}