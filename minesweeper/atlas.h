#pragma once
#include "externalLibraries.h"
#include <vector>
#include <utility>


template <typename T>
struct vec2
{
	T x, y;
};










class BitmapClass // for idk what
{
protected:
	int fullW, fullH;
	ALLEGRO_BITMAP* bitmap;

public:
	
	BitmapClass() { }
	BitmapClass(int w, int h) { bitmap = al_create_bitmap(w, h); fullW = w; fullH = h; }
	void CreateCustom(int w, int h); // DO NOT USE IN DERIVED!!!
	bool isDef() const;
	int getWidth() const;
	int getHeight() const;
	void resetWidth();
	void resetHeight();
	void resetBoth(); // does what reset width and height together do
	void DrawBitmap(float x, float y, int flags) const;
	void DrawColoredBitmap(float x, float y, ALLEGRO_COLOR color, int flags) const;
};


class BitmapImage : public BitmapClass // for single image textures
{
public:
	void load_image(const char* filename);
	void load_image(const char* filename, int flags);
	BitmapImage() { }
	BitmapImage(const char* filename) { load_image(filename); }
	BitmapImage(const char* filename, int flags) { load_image(filename, flags); }
};

class tileAtlas : public BitmapImage // for atlas with known width and height of a tile
{
protected:
	int w, h;

public:
	void DrawTile(float x, float y, float dx, float dy) const;
	void DrawTile(float x, float y, float dx, float dy, int flags) const;
	void DrawColoredTile(float x, float y, float dx, float dy, ALLEGRO_COLOR color) const;
	void DrawColoredTile(float x, float y, float dx, float dy, ALLEGRO_COLOR color, int flags) const;
	tileAtlas() {   }
	tileAtlas(int width, int height); // need width and height of each tile
	tileAtlas(int width, int height, const char *filename);
	tileAtlas(int width, int height, const char* filename, int flags);
	void setNewWH(int widht, int height);
	int getTileWidth() const;
	int getTileHeight() const;
};
