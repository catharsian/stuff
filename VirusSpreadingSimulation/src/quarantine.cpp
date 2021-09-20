#include "quarantine.h"

int quarantine::getX() const
{
	return x;
}
int quarantine::getY() const
{
	return y;
}
int quarantine::getXW() const
{
	return x + w;
}
int quarantine::getYH() const
{
	return y + h;
}
int quarantine::getW() const
{
	return w;
}
int quarantine::getH() const
{
	return h;
}

quarantine::quarantine(int _x, int _y, int _w, int _h)
{
	x = _x;
	y = _y;
	w = _w;
	h = _h;
}