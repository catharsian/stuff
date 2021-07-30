#include "atlas.h"

void BitmapClass::CreateCustom(int w, int h)// DO NOT USE IN DERIVED CLASSES!!!
{
	bitmap = al_create_bitmap(w, h);
	fullW = w;
	fullH = h;
}
void BitmapClass::resetHeight()
{
	fullH = al_get_bitmap_height(bitmap);
}
void BitmapClass::resetWidth()
{
	fullW = al_get_bitmap_width(bitmap);
}
void BitmapClass::DrawBitmap(float x, float y, int flags) const
{
	al_draw_bitmap(bitmap,x,y,flags);
}
void BitmapClass::resetBoth()
{
	resetWidth();
	resetHeight();
}
void BitmapClass::DrawColoredBitmap(float x, float y, ALLEGRO_COLOR color, int flags) const
{
	al_draw_tinted_bitmap(bitmap,color,x,y,flags);
}
void BitmapImage::load_image(const char* filename)
{
	bitmap = al_load_bitmap(filename);
	if (bitmap == NULL)
	{
		puts("LOADING BITMAP FAILED");
	}
}
void BitmapImage::load_image(const char* filename, int flags)
{
	bitmap = al_load_bitmap_flags(filename, flags);
}
bool BitmapClass::isDef() const
{
	if (bitmap == NULL)
	{
		return false;
	}
	else
	{
		return true;
	}
}
tileAtlas::tileAtlas(int width, int height)
{
	w = width;
	h = height;
}
void tileAtlas::setNewWH(int width, int height)
{
	w = width;
	h = height;
}
int	BitmapClass::getWidth() const
{
	return al_get_bitmap_width(bitmap);
}
int BitmapClass::getHeight() const
{
	return al_get_bitmap_height(bitmap);
}
int tileAtlas::getTileWidth() const
{
	return w;
}
int tileAtlas::getTileHeight() const
{
	return h;
}
tileAtlas::tileAtlas(int width, int height, const char* filename)
{
	w = width;
	h = height;
	load_image(filename);
}
void tileAtlas::DrawTile(float x, float y, float dx, float dy) const
{
	al_draw_bitmap_region(bitmap, x * w , y * h , w, h, dx, dy, 0);
}
void tileAtlas::DrawTile(float x, float y, float dx, float dy, int flags)  const
{
	al_draw_bitmap_region(bitmap, x * w, y * h, w, h, dx, dy, flags);
}
void tileAtlas::DrawColoredTile(float x, float y, float dx, float dy, ALLEGRO_COLOR color) const
{
	al_draw_tinted_bitmap_region(bitmap, color, x * w, y * h, w, h, dx, dy, 0);
}
void tileAtlas::DrawColoredTile(float x, float y, float dx, float dy, ALLEGRO_COLOR color,int flags) const
{
	al_draw_tinted_bitmap_region(bitmap, color, x * w , y * h, w, h, dx, dy, flags);
}