#pragma once
#include "vector.h"
#include "INCLUDES.h"
#define RANDOMIZE
enum class state : unsigned char {
	healthy,
	infected,
	dead
};

class human
{
private:
	Eigen::Vector2f pos;
	Eigen::Vector2f dir;
	Eigen::Vector2f dest;
	quarantine* belongs;
	static std::vector<quarantine*> zones;
public:
	Eigen::Vector2f perpVec;
	int shitVecCount;
	int coughTimer;
	int coughRad;
	int quarTimer;
	int toTrade;
	int inTrade;
	state st;
	human();
	void go();
	void change();
	void change(float x, float y);
	void drawMe() const;
	void expGo();
	Eigen::Vector2f getPos() const; 
	Eigen::Vector2f getDir() const;
	Eigen::Vector2f getDest() const;
	int toLive;
	static void registerQuarantine(quarantine* zone);
	void teleport(int x, int y);
	void changeQuarantine(quarantine* zone);
	void quarantinize();
	static void drawZones();
};

class humCon
{
private:
	std::vector<someText> text;
	human* cont;
	std::vector<human*> inf;
	int curThing;
	void expPhysicsLoop();
	std::vector<Image> imgs;
public:
	void loadImg(ALLEGRO_BITMAP* img, int x, int y);
	void loadImg(ALLEGRO_BITMAP* img, quarantine* zone);
	static quarantine* generalZone;
	static quarantine* healthyZone;
	static quarantine* tradeZone;
	void loadText(const char* txt, int x, int y, ALLEGRO_COLOR col, ALLEGRO_FONT* fnt);
	void drawTexts();
	humCon();
	void drawAll() const;
	void infect(human &man);
	void die(human* man);
	void checkAll();
	void physics();
	void expPhysics();
	void coughAll();
	void drawImgs();
	~humCon();

};

