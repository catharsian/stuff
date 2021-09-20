#include "human.h"
#define RANDOMIZE


int parser::SCREENWIDTH;
int parser::SCREENHEIGHT;
int parser::rad;
int parser::menQuan;
int parser::infChance;
float parser::infPulse;
float parser::infRad;
int parser::maxFPS;
bool parser::expMovement;
int parser::dying;
float parser::speedMultiplier;
int parser::quarantineZone;
std::vector<quarantine*> human::zones;
float parser::tradeCenter;
float parser::tradeCenterDuration;
quarantine* humCon::tradeZone;
quarantine* humCon::healthyZone;
quarantine* humCon::generalZone;


namespace randomizer {
	static unsigned long x = 123456789 * time(0), y = 362436069 / time(0), z = 521288629;

	unsigned long gen(void) {          //period 2^96-1
		unsigned long t;
		x ^= x << 16;
		x ^= x >> 5;
		x ^= x << 1;

		t = x;
		x = y;
		y = z;
		z = t ^ x ^ y;

		return z;
	}
}

void human::expGo()
{
	if (shitVecCount > 0)
	{
		--shitVecCount;
		pos += perpVec * parser::speedMultiplier;
		if (shitVecCount == 0)
		{
			dir = dest - pos;
			dir.normalize();
		}
	}
	else
	{
		pos += dir * parser::speedMultiplier;
	}
	if ((pos - dest).norm() < 2.0f)
	{
		change();
	}
}

void human::change()
{
	/*if (belongs == nullptr) {
		
		auto rerandX = []() {return randomizer::gen() % (parser::SCREENWIDTH - parser::rad * 2 + parser::rad); };
		auto rerandY = []() {return randomizer::gen() % (parser::SCREENHEIGHT - parser::rad * 2 + parser::rad); };
		int genX = rerandX();

		int genY = rerandY();

		for (auto &zone : zones)
		{
			while (!((genX < zone->getX() || genX > zone->getXW()) && (genY < zone->getY() || genY > zone->getYH()))) {
				genX = rerandX();
				genY = rerandY();
			}
		}

		dest = Eigen::Vector2f(genX, genY);


	}*/
	//else
	
	int genX = randomizer::gen() % (belongs->getW()) + belongs->getX();

	int genY = randomizer::gen() % (belongs->getH()) + belongs->getY();
		
	dest = Eigen::Vector2f(genX, genY);
	
	dir = dest - pos;
	dir.normalize();

	//std::cout << dest.x() << ' ' << dest.y() << std::endl;
	//std::cout << (pos - dest).x() << ' ' << (pos - dest).y() << std::endl;
}

void human::change(float x, float y)
{
	dest = Eigen::Vector2f(x,y);

	dir = dest - pos;
	dir.normalize();
}


human::human()
{

	st = state::healthy;
	pos = Eigen::Vector2f(randomizer::gen() % (parser::SCREENWIDTH - parser::rad * 2 + parser::rad), randomizer::gen() % (parser::SCREENHEIGHT - parser::rad * 2 + parser::rad));
	coughTimer = 0;
	coughRad = 0;
	shitVecCount = 0;
	toLive = -1;
	belongs = nullptr;
	quarTimer = -1;
	if (parser::tradeCenter)
	{
		toTrade = randomizer::gen() % int(parser::tradeCenter * parser::maxFPS);
		inTrade = -1;
	}

	//change();
}

void human::drawZones()
{

	for (auto& zone : zones)
	{
		al_draw_rectangle(zone->getX(), zone->getY(), zone->getXW(), zone->getYH(), al_map_rgb(220,220,220), 5);
	}
}


void human::registerQuarantine(quarantine* zone)
{
	zones.push_back(zone);
}

void human::changeQuarantine(quarantine* zone)
{
	belongs = zone;
}


void human::go()
{
	pos += dir * parser::speedMultiplier;

	if ((pos - dest).norm() < 5.0f)
	{
		change();
	}
	if (parser::tradeCenter != 0.0f && parser::tradeCenterDuration != 0.0f)
	{
		if (toTrade == 0)
		{
			inTrade = parser::maxFPS * parser::tradeCenterDuration;

			belongs = humCon::tradeZone;
			quarantinize();
			toTrade = -1;
		}
		else if (toTrade != -1)
		{
			--toTrade;
		}
		if (inTrade > 0)
		{
			--inTrade;
		}
		else if (inTrade != -1)
		{
			toTrade = parser::maxFPS * parser::tradeCenter;
			belongs = humCon::healthyZone;
			quarantinize();
			inTrade = -1;
		}
	}
}

Eigen::Vector2f human::getPos() const 
{
	return pos;
}
Eigen::Vector2f human::getDir() const
{
	return dir;
}
Eigen::Vector2f human::getDest() const
{
	return dest;
}

void human::drawMe() const
{
	al_draw_circle(pos.x(), pos.y(), parser::rad + 1, al_map_rgb(0, 0, 0), 1);
	switch (st){
	case state::healthy :
		al_draw_filled_circle(pos.x(), pos.y(), parser::rad, al_map_rgb(0,230,0));
		break;
	case state::infected:
		if (coughRad != 0) {
			const int mult = 200 / parser::infRad;
			al_draw_circle(pos.x(), pos.y(), parser::infRad - coughRad, al_map_rgba(220, 220, 0, (parser::infRad - coughRad) * mult), 4);
		}
		al_draw_filled_circle(pos.x(), pos.y(), parser::rad, al_map_rgb(190, 0, 0));
		break;
	case state::dead:
		//al_draw_circle(pos.x(), pos.y(), coughRad, al_map_rgb(50, 50, 50), 4);

		al_draw_circle(pos.x(), pos.y(), parser::rad + 1, al_map_rgb(70, 0, 0), 1);

		al_draw_filled_circle(pos.x(), pos.y(), parser::rad, al_map_rgb(40, 40, 40));
		break;
	}	
}

void human::quarantinize()
{
	if (belongs != nullptr)
	{
		//pos = Eigen::Vector2f(belongs->getX() + belongs->getW() / 2, belongs->getY() + belongs->getH()/2);
		//change();
		teleport(belongs->getX() + belongs->getW() / 2, belongs->getY() + belongs->getH() / 2);
	}
}

void human::teleport(int x, int y)
{
	pos = Eigen::Vector2f(x, y);
	change();
}



int intAbs(int& num)
{
	if (num < 0)
		return -num;
	else 
		return num;
}












humCon::humCon()
{
	cont = new human[parser::menQuan];
	human& first = cont[0];
	first.st = state::infected;

	inf.push_back(&first);

	first.coughTimer = parser::infPulse * parser::maxFPS;

	curThing = 0;

	first.toLive = parser::dying * parser::maxFPS;
	generalZone = nullptr;
	if (parser::quarantineZone)
	{
		generalZone = new quarantine(parser::SCREENWIDTH - 400, parser::SCREENHEIGHT - 400, 400, 400);
		healthyZone = new quarantine(5, 5, parser::SCREENWIDTH - 405, parser::SCREENHEIGHT - 5);
		human::registerQuarantine(generalZone);
		first.quarTimer = parser::quarantineZone * parser::maxFPS;
	}
	else
	{
		healthyZone = new quarantine(5,5,parser::SCREENWIDTH - 5, parser::SCREENHEIGHT - 5);
		//human::registerQuarantine(healthyZone);
	}
	for (int i = 0; i < parser::menQuan; ++i)
	{
		cont[i].changeQuarantine(healthyZone);
		cont[i].teleport(randomizer::gen() % healthyZone->getW() + healthyZone->getX(), randomizer::gen() % healthyZone->getH() + healthyZone->getY());
		human::registerQuarantine(healthyZone);
		//cont[i].change();
	}
	if (parser::tradeCenter && parser::tradeCenterDuration)
	{
		tradeZone = new quarantine(parser::SCREENWIDTH - 400, 5, 400 , 400);
		human::registerQuarantine(tradeZone);
	}
}

void humCon::drawAll() const
{
	for (int i = 0; i < parser::menQuan; ++i)
	{
		cont[i].drawMe();
	}
}

void humCon::infect(human &man)
{
	man.st = state::infected;
	inf.push_back(&man);
	man.coughTimer = parser::infPulse * parser::maxFPS;
	if (parser::dying != 0)
	{
		man.toLive = parser::dying * parser::maxFPS;
	}
	if (parser::quarantineZone)
	{
		man.quarTimer = parser::quarantineZone * parser::maxFPS;
	}
}

void humCon::die(human* man)
{
	man->st = state::dead;
	man->coughTimer = 0;
	man->toLive = -1;
}


void humCon::checkAll()
{
	int count = 0;
	for (auto man : inf)
	{
		if (man->coughTimer == 0)
		{
			man->coughTimer = parser::infPulse * parser::maxFPS;
			man->coughRad = parser::infRad;
		}
		if (man->coughRad > 0)
		{

			for (int i = 0; i < parser::menQuan; ++i)
			{
				if (cont[i].st != state::infected && cont[i].st != state::dead )
				{
					if ((man->getPos() - cont[i].getPos()).norm() < parser::infRad - man->coughRad)
					{
						if (randomizer::gen() % parser::infChance == 0)
						{
							infect(cont[i]);
						}
					}
				}
			}
		}
		man->coughTimer-=1;
		man->toLive -= 1;
		if (man->coughRad > 0) man->coughRad-=1;
		if (man->toLive == 0)
		{
			die(man);
			auto ptr = inf.begin();
			ptr += count;
			inf.erase(ptr);
		}
		if (parser::quarantineZone)
		{
			if (man->quarTimer > 0)
			{
				man->quarTimer -= 1;
			}
			else
			{
				if (man->quarTimer != -1) {
					man->changeQuarantine(generalZone);
					man->quarantinize();
					man->quarTimer = -1;
					man->toTrade = -1;
					man->inTrade = -1;
				}


			}
		}
		++count;
	}
}

void humCon::physics()
{
	for (int i = 0; i < parser::menQuan; ++i)
	{
		cont[i].go();
	}
}

void humCon::expPhysicsLoop()
{
	for (int i = 0; i < parser::menQuan; ++i)
	{
		if (i != curThing) {
			const Eigen::Vector2f firstPos = cont[i].getPos();
			const Eigen::Vector2f firstDir = cont[i].getDir();
			const Eigen::Vector2f curPos = cont[curThing].getPos();
			const Eigen::Vector2f curDir = cont[curThing].getDir();
			if ((firstPos + firstDir * parser::speedMultiplier - curPos + curDir * parser::speedMultiplier).norm() < parser::rad + 3.0f)
			{
				cont[i].perpVec = Eigen::Vector2f(firstDir.y(), firstDir.x());
				cont[i].shitVecCount = parser::rad;
				cont[curThing].perpVec = Eigen::Vector2f(curDir.y() * -1, curDir.x() * -1);
				cont[curThing].shitVecCount = parser::rad;
			}
		}
	}
	++curThing;
	if (curThing == parser::menQuan)
		curThing = 0;
}






void humCon::expPhysics()
{
	for (int i = 0; i < 10; ++i)
		expPhysicsLoop();
	for (int i = 0; i < parser::menQuan; ++i)
	{
		cont[i].expGo();
	}
}



void humCon::coughAll()
{
	for (auto man : inf)
	{

	}
}


void humCon::loadText(const char* txt, int x, int y, ALLEGRO_COLOR col, ALLEGRO_FONT* fnt)
{

	someText newTXT(txt, x, y, col, fnt);

	text.push_back(newTXT);

}

void humCon::drawTexts()
{
	for (auto& txt : text)
	{
		txt.draw();
	}
}



humCon::~humCon()
{
	delete generalZone;
	delete healthyZone;
	delete tradeZone;
}

void humCon::loadImg(ALLEGRO_BITMAP* img, int x, int y)
{
	Image image(img, x, y);
	imgs.push_back(image);
}

void humCon::drawImgs()
{
	for (auto& img : imgs)
	{
		img.draw();
	}
}
void humCon::loadImg(ALLEGRO_BITMAP* img, quarantine* zone)
{
	Image image(img, zone->getX() + (zone->getW() / 2) - 100, zone->getY() + (zone->getH() / 2) - 100);
	imgs.push_back(image);
}