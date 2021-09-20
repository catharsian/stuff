#pragma once

#include <wchar.h>
#include <string>
class quarantine
{
private:
	int x, y, w, h;

public:
	int getX() const;
	int getY() const;
	int getXW() const;
	int getYH() const;
	int getW() const;
	int getH() const;
	quarantine(int _x, int _y, int _w, int _h);

};



