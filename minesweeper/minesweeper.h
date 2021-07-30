#pragma once
#include "atlas.h"
#include <thread>

extern bool redraw;

extern bool lost;
extern tileAtlas atl;


struct Cell
{
	static const int aCell = 0x20;
	int x, y;
	int dx, dy;
	int minesNear;
	bool isMine;
	bool isPressed;
	bool isMarked;
	int getX() const ;
	int getY() const ;
	bool mark();
	void advance();
	void press();
	void operator++(int);
	void operator--(int);
	void makeMined();
	void DrawMe();
	Cell(bool mine, int x1, int y1);
	void discard();
	void NormalConstructor(bool mine, int x1, int y1);
	Cell();

};


class CellObserver
{
protected:
	Cell* observe;
	int max;
	int width, height;
	int marks;

public:
	void tryMark(int x, int y);
	bool won;
	int INDEX(const int x, int const y) const; // convert x and y coordinate on the grid to possible index for array
	CellObserver(int x, int y, int mines);
	Cell& operator[](const unsigned n) const;
	Cell &getXY(const int x, const int y) const ; // possibly get the cell by x and y coordinate
	bool safeGet(const int n) const;
	void DrawEverything();
	void pressCell(const int x, const int y);
	~CellObserver();
	void generateMines(const int x, const int y);
	void remakeNears();
	void Degenerate();
};
