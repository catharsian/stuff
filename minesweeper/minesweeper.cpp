#include "minesweeper.h"
#include <stack>
#include <cstdlib>
#include <list>
#include <ctime>
tileAtlas atl;
bool lost = false;
#define noteq(x, y) (j * width + r1 != x + (y) * width)
#define curx gg.top()->getX()
#define cury gg.top()->getY()
#define CHECKPRESS(i, j) if (i >= 0 && j >= 0 && i < width && j < height ) \
{\
	if (!((observe[i + j * width]).isPressed) && !(observe[i+j * width].isMine)) \
		{\
			pressCell(i,j); \
		}\
}
#define CHECK(i, j) if (INDEX(i, j) != -1) {observe[INDEX(i, j)].advance();}
bool redraw = true;
void Cell::advance()
{
	minesNear += 1;
}
int Cell::getX() const 
{
	return x;
}
int Cell::getY() const
{
	return y;
}
void Cell::makeMined()
{
	isMine = true;
}

Cell::Cell(bool mine, int x1, int y1)
{
	isMine = mine;
	x = x1;
	y = y1;
	dx = x * Cell::aCell;
	dy = y * Cell::aCell;

}
Cell::Cell() {	}

void Cell::NormalConstructor(bool mine, int x1, int y1)
{
	isMine = mine;
	x = x1;
	y = y1;
	dx = x * Cell::aCell;
	dy = (y + 2) * Cell::aCell;
	isMarked = false;
	isPressed = false;
	minesNear = 0;
}
void Cell::press()
{
	if (!isPressed && !lost)
	{
		if (!isMarked)
		{

			isPressed = true;
			if (isMine)
			{
				lost = true;
				redraw = true;
			}
		}
	}
}
bool Cell::mark()
{
	if (!isPressed)
	{
		isMarked = !isMarked;
		DrawMe();
		if (isMine) { return true; }
		else { return false; }
	}
	return false;
}
void Cell::DrawMe()
{
	
	if (!lost) 
	{
		if (!isPressed && !isMarked)
		{
			atl.DrawTile(0, 0, dx, dy);
			return;
		}
		else if (!isPressed && isMarked)
		{
			atl.DrawTile(15, 0, dx, dy);
			return;
		}
		if (isPressed)
		{
			atl.DrawTile(1 + minesNear, 0, dx, dy);
			return;
		}
	}
	else
	{
		if (isMine && !isMarked)
		{
			atl.DrawTile(10, 0, dx, dy);
			return;
		}
		if (isMine && isMarked)
		{
			atl.DrawTile(15, 0, dx, dy);
			return;
		}
		if (!isMine && !isMarked)
		{
			atl.DrawTile(1 + minesNear, 0, dx, dy);
			return;
		}
		else if (!isMine && isMarked)
		{
			atl.DrawTile(11, 0, dx, dy);
			return;
		}
	}
}
void Cell::discard()
{
	isPressed = false;
	isMine = false;
	isMarked = false;
	minesNear = 0;

}
void Cell::operator++(int) {
	minesNear+=1;
}
void Cell::operator--(int)
{
	minesNear+=1;
}
Cell& CellObserver::operator[](const unsigned n) const
{
	return observe[n];
}
int CellObserver::INDEX(int const x, int const y) const
{
	if (x >= 0 && y >= 0 && x < width && y < height)
		return x + (y * width);
	else return -1;
}
CellObserver::CellObserver(int x, int y, int mines)
{
	if (x > 0 && y > 0 && mines > 0)
	{
		std::srand(std::time(0));
		width = x; height = y;
		max = width * height;
		marks = mines;
		observe = new Cell[x * y];
		for (int i = 0; i < x; ++i)
		{
			for (int j = 0; j < y; ++j)
			{
				observe[j * width + i].NormalConstructor(false, i, j);
			}
		}
		won = false;
	}
	else
	{
		puts("ERROR: INVALID WIDTH AND HEIGHT NUMBER!\n");
	}
}

bool CellObserver::safeGet(const int n) const
{
	if (n >= 0 && n < width * height)
	{
		return true;
	}
	else
		return false;
}
Cell& CellObserver::getXY(const int x1, const int y1) const
{
	return observe[x1 + (y1 * width)];
}
void CellObserver::pressCell(int const x1, const int y1)
{
	
	if (x1 >= 0 && y1 >= 0 && x1 < width && y1 < height)
	{
		if (observe[x1 + y1 * width].isMine && !observe[x1 + y1 * width].isMarked && !lost)
		{
			lost = true;
			observe[x1 + y1 * width].press();
			observe[x1 + y1 * width].DrawMe();
		}
		else if (!lost)
		{
			if (observe[x1 + y1 * width].minesNear != 0)
			{
				observe[x1 + y1 * width].press();
				observe[x1 + y1 * width].DrawMe();
			}
			else if (observe[x1 + y1 * width].isPressed)
			{

			}
			else
			{
				std::stack<Cell*, std::list<Cell*> > gg;
				gg.push(&observe[x1 + y1 * width]);
				while (!gg.empty())
				{
					gg.top()->press();
					gg.top()->DrawMe();
					if (gg.top()->minesNear == 0)
					{
						if (INDEX(curx + 1, cury) != -1)
						{
							if (!(observe[INDEX(curx + 1, cury)].isPressed) && !(observe[INDEX(curx + 1, cury)].isMine) && !(observe[INDEX(curx + 1, cury)].isMarked))
							{
								gg.push(&observe[INDEX(curx + 1, cury)]);
								continue;
							}
						}
						if (INDEX(curx - 1, cury) != -1)
						{
							if (!(observe[INDEX(curx - 1, cury)].isPressed) && !(observe[INDEX(curx - 1, cury)].isMine) && !(observe[INDEX(curx - 1, cury)].isMarked))
							{
								gg.push(&observe[INDEX(curx - 1, cury)]);
								continue;
							}
						}
						if (INDEX(curx, cury + 1) != -1)
						{
							if (!(observe[INDEX(curx, cury + 1)].isPressed) && !(observe[INDEX(curx, cury + 1)].isMine) && !(observe[INDEX(curx, cury + 1)].isMarked))
							{
								gg.push(&observe[INDEX(curx, cury + 1)]);
								continue;
							}
						}
						if (INDEX(curx, cury - 1) != -1)
						{
							if (!(observe[INDEX(curx, cury - 1)].isPressed) && !(observe[INDEX(curx, cury - 1)].isMine) && !(observe[INDEX(curx, cury - 1)].isMarked))
							{
								gg.push(&observe[INDEX(curx, cury - 1)]);

								continue;
							}
						}
						if (INDEX(curx - 1, cury - 1) != -1)
						{
							if (!(observe[INDEX(curx - 1, cury - 1)].isPressed) && !(observe[INDEX(curx - 1, cury - 1)].isMine) && !(observe[INDEX(curx - 1, cury - 1)].isMarked))
							{
								gg.push(&observe[INDEX(curx - 1, cury - 1)]);
								continue;
							}
						}
						if (INDEX(curx - 1, cury + 1) != -1)
						{
							if (!(observe[INDEX(curx - 1, cury + 1)].isPressed) && !(observe[INDEX(curx - 1, cury + 1)].isMine) && !(observe[INDEX(curx - 1, cury + 1)].isMarked))
							{
								gg.push(&observe[INDEX(curx - 1, cury + 1)]);
								continue;
							}
						}
						if (INDEX(curx + 1, cury + 1) != -1)
						{
							if (!(observe[INDEX(curx + 1, cury + 1)].isPressed) && !(observe[INDEX(curx + 1, cury + 1)].isMine) && !(observe[INDEX(curx + 1, cury + 1)].isMarked))
							{
								gg.push(&observe[INDEX(curx + 1, cury + 1)]);
								continue;
							}
						}
						if (INDEX(curx + 1, cury - 1) != -1)
						{
							if (!(observe[INDEX(curx + 1, cury - 1)].isPressed) && !(observe[INDEX(curx + 1, cury - 1)].isMine) && !(observe[INDEX(curx + 1, cury - 1)].isMarked))
							{
								gg.push(&observe[INDEX(curx + 1, cury - 1)]);

								continue;
							}
						}
					}
					gg.pop();
				}
			}
		}
	}
	al_flip_display();
}

void CellObserver::DrawEverything()
{
	for (int i = 0; i < width * height; i++)
	{
		observe[i].DrawMe();
	}
}
void CellObserver::Degenerate()
{
	for (int i = 0; i < width * height; i++)
	{
		observe[i].discard();
	}
}

void CellObserver::tryMark(int x, int y)
{
	if (x >= 0 && y >= 0 && x < width && y < height)
	{
		if (observe[x + y * width].mark())
		{
			if (observe[x + y * width].isMarked)
			{
				marks--;
			}
			else
			{
				marks++;
			}
		}
		else
		{
			if (observe[x + y * width].isMarked)
			{
				marks++;
			}
			else
			{
				marks--;
			}
		}
		if (marks == 0)
		{
			won = true;
			lost = true;
			redraw = true;
		}
	}
	al_flip_display();
}
CellObserver::~CellObserver()
{
	delete[] observe;
}

void CellObserver::generateMines(const int x, const int y)
{
	int mines = marks;
	int r, r1;
	while (mines > 0)
	{
		for (int j = 0; j < height && mines > 0; ++j)
		{
			r = std::rand() % (width / 2);
			for (int i = 0; i < r; i++)
			{
				r1 = rand() % width;
			lol:
				if (INDEX(r1, j) != -1)
				{
					if (observe[j * width + r1].isMine || 
					   !(noteq(x - 1, y + 1) && noteq(x, y + 1) && noteq(x + 1, y + 1) &&
						noteq(x - 1, y) && noteq(x, y) && noteq(x + 1, y) && noteq(x - 1, y - 1) &&
						noteq(x, y - 1) && noteq(x + 1, y - 1))) 
					{
						r1++;
						goto lol;
					}
					else
					{
						observe[j * width + r1].makeMined();
						mines--;
					}
				}
			}
		}
	}
}

void CellObserver::remakeNears()
{
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			if (observe[j * width + i].isMine)
			{
				CHECK(i - 1, j - 1);
				CHECK(i, j - 1);
				CHECK(i + 1, j - 1);
				CHECK(i - 1, j);
				CHECK(i + 1, j);
				CHECK(i - 1, j + 1);
				CHECK(i, j + 1);
				CHECK(i + 1, j + 1);
				observe[j * width + i].minesNear = 0;
			}
		}
	}
}

