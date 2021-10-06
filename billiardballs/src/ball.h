#pragma once
#include "includes.h"



class ball
{
private:
	Eigen::Vector2f pos;
	Eigen::Vector2f dir;
	Eigen::Vector2f accel;
	int rad;
	ALLEGRO_COLOR col;
	bool held;

public:
	static int counter;
	static ALLEGRO_FONT* fon;
	int n;
	ball();
	int getRad() const;
	Eigen::Vector2f getPos() const;
	Eigen::Vector2f getDir() const;
	void drawMe() const;
	void go();
	

	static void moveUs(ball& first, ball& second);
	static void regeneratePos(ball& first, ball& second);
	static void bump(ball& first, ball& second);
};


class ballCon
{
private:

	ball* cont;


public:
	void findOne;
	static std::vector<float> OverlappsX;
	static std::vector<float> OverlappsY;
	ballCon();
	~ballCon();
	void drawAll();
	void checkAll();
	void moveAll();
	void oneCheck(const int i);
	
};
