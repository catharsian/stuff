#pragma once
#include "includes.h"

struct Time
{
	double delta, running;
	Time() { delta = 0.0; running = 0.0; }
};
extern Time gameTime;

class ball
{
private:
	Eigen::Vector2f pos;
	Eigen::Vector2f dir;
	Eigen::Vector2f accel;
	int rad;
	ALLEGRO_COLOR col;
	bool held;
	bool afterHeld;
public:
	static ALLEGRO_FONT* fon;
	int n;
	bool bumped;
	ball();
	int getRad() const;
	Eigen::Vector2f getPos() const;
	Eigen::Vector2f getDir() const;
	void drawMe() const;
	void go();
	
	void setPos(int x, int y);
	static void moveUs(ball& first, ball& second);
	static void regeneratePos(ball& first, ball& second);
	static void bump(ball& first, ball& second);
	void changeDir(Eigen::Vector2f newDir);
};


class ballCon
{
private:

	ball* cont;


public:
	void findOne(int mX, int mY);
	static std::vector<float> OverlappsX;
	static std::vector<float> OverlappsY;
	ballCon();
	~ballCon();
	void drawAll();
	void checkAll();
	void moveAll();
	void oneCheck(const int i);
	void resetBumps();
	ball* whoIsHeld;
};
