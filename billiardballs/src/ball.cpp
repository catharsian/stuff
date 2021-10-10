#include "ball.h"
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

int parser::SCREENHEIGHT;
int parser::SCREENWIDTH;
int parser::maxFPS;
int parser::ballQuan;
float parser::speedModifyer;
ALLEGRO_FONT* ball::fon;
Time gameTime;

double theta(Eigen::Vector2f vec)
{
	return atan2(vec.x(), vec.y());
}

Eigen::Vector2f ball::getPos() const
{
	return pos;
}


Eigen::Vector2f ball::getDir() const
{
	return dir;
}

int ball::getRad() const
{
	return rad;
}

ball::ball()
{
	const int fuck = int((randomizer::gen() % 15));
	rad = fuck + 7;
	pos = Eigen::Vector2f(randomizer::gen() % (parser::SCREENWIDTH - 100) + 50, randomizer::gen()%(parser::SCREENHEIGHT-100)+50);
	col = al_map_rgb(randomizer::gen()%256, randomizer::gen() % 256, randomizer::gen() % 256);
	dir = Eigen::Vector2f(0, 0);
	accel = Eigen::Vector2f(0,0);
	held = false;
	afterHeld = false;
	bumped = false;
}

void ball::drawMe() const
{
	if (pos.y() > parser::SCREENHEIGHT - rad) {
		al_draw_filled_circle(pos.x(), parser::SCREENHEIGHT - rad, rad, col);
	}
	else
	{
		al_draw_filled_circle(pos.x(), pos.y(), rad, col);
	}
	al_draw_text(fon, al_map_rgb(0, 0, 0), pos.x() - 7, pos.y() - 6, 0, std::to_string(rad).c_str());
}

void ball::go()
{
	const float forY = pos.y();
	const float forX = pos.x();
	if (forY > parser::SCREENHEIGHT - rad)
	{
		pos << pos.x(), rad;
	}
	else if (forY < rad)
	{
		pos << pos.x(), parser::SCREENHEIGHT - rad;
	}
	if (forX > parser::SCREENWIDTH - rad)
	{
		pos << parser::SCREENWIDTH - rad, pos.y();
		dir = Eigen::Vector2f(dir.x() * -1 + 1, dir.y());
		accel *= -1;

	}
	else if (forX < rad)
	{
		pos << rad, pos.y();
		dir = Eigen::Vector2f(dir.x() * -1 - 1, dir.y());
		accel *= -1;
	}
	pos += dir * parser::speedModifyer * gameTime.delta;
	if (dir.dot(accel) > 0)
		dir -= accel * parser::speedModifyer * gameTime.delta;
	else
	{
		accel = Eigen::Vector2f(0, 0);
		dir = Eigen::Vector2f(0, 0);
		afterHeld = false;
	}
}



void ball::regeneratePos(ball& first, ball& second)
{
	first.pos = Eigen::Vector2f(randomizer::gen() % parser::SCREENWIDTH, randomizer::gen() % parser::SCREENHEIGHT);
	second.pos = Eigen::Vector2f(randomizer::gen() % parser::SCREENWIDTH, randomizer::gen() % parser::SCREENHEIGHT);
	if ((first.getPos() - second.getPos()).norm() < first.getRad() + second.getRad()) regeneratePos(first, second);
}

void ball::bump(ball& first, ball& second)
{
	const Eigen::Vector2f firstdir = first.dir;
	const Eigen::Vector2f seconddir = second.dir;
	double myTheta =  theta(Eigen::Vector2f(firstdir - seconddir));
	double secTheta = myTheta * -1;
	const Eigen::Rotation2Df t(myTheta);
	const Eigen::Rotation2Df c(myTheta);
	const Eigen::Vector2f firstrot  = t.toRotationMatrix() * firstdir;
	const Eigen::Vector2f secondrot = t.toRotationMatrix() * seconddir;;
	const Eigen::Vector2f firstlast = ((first.rad - second.rad) * firstrot + 2 * second.rad * secondrot) / (first.rad + second.rad);
	const Eigen::Vector2f f1 = t.toRotationMatrix() * -1 * Eigen::Vector2f(firstlast.x(), firstlast.y());
	const Eigen::Vector2f secondlast = ((second.rad - first.rad) * secondrot + 2 * first.rad * firstrot) / (first.rad + second.rad);
	const Eigen::Vector2f f2 = t.toRotationMatrix() * -1 * Eigen::Vector2f(secondlast.x(), secondlast.y());
	first .changeDir(Eigen::Vector2f(f1.x(), f1.y() * -1));
	second.changeDir(Eigen::Vector2f(f2.x(), f2.y() * -1));

}

void ball::moveUs(ball& first, ball& second)
{
	const auto diff  =  Eigen::Vector2f(first.getPos() - second.getPos()) ;
	const float most =  first.rad		 + second.rad - diff.norm() + 1;
	first.pos		+= diff.normalized() * most;
	second.pos		-= diff.normalized() * most;
}

void ball::setPos(int x, int y)
{
	pos = Eigen::Vector2f(x, y);
}
void ball::changeDir(const Eigen::Vector2f newDir)
{
	dir = newDir;
	accel = newDir * 0.02;
	afterHeld = true;
}










ballCon::ballCon()
{
	cont = new ball[parser::ballQuan];
	bool doAgain;
	again:
	for (int i = 0; i < parser::ballQuan; ++i)
	{
		for (int j = 0; j < parser::ballQuan; ++j)
		{
			if (i != j) {
				const auto diff = Eigen::Vector2f(cont[i].getPos() - cont[j].getPos());
				if (diff.norm() < (cont[i].getRad() + cont[j].getRad()))
				{
					ball::moveUs(cont[i], cont[j]);
					
					goto again;
				}
			}
		}
	}
	whoIsHeld = nullptr;

}

void ballCon::drawAll()
{
	for (int i = 0; i < parser::ballQuan; ++i)
	{
		cont[i].drawMe();
	}
}

void ballCon::oneCheck(const int i)
{
	for (int j = 0; j < parser::ballQuan; ++j)
	{
		const ball& first = cont[i];
		const ball& second = cont[j];
		if ((first.getPos() - second.getPos()).norm() < (second.getRad() + first.getRad()))
		{
			ball::moveUs(const_cast<ball&>(first), const_cast<ball&>(second));
			const_cast<ball&>(first).go();
		}
		else
		{
			const_cast<ball&>(first).go();
		}
	}
}


void ballCon::checkAll()
{


	for (int i = 0; i < parser::ballQuan; ++i)
	{

		for (int j = 0; j < parser::ballQuan; ++j)
		{
			if (i != j) {
				const ball& first = cont[i];
				const ball& second = cont[j];
					if ((first.getPos() - second.getPos()).norm() < (second.getRad() + first.getRad()))
					{
						ball::moveUs(const_cast<ball&>(first), const_cast<ball&>(second));
							ball::bump(const_cast<ball&>(first), const_cast<ball&>(second));
							const_cast<ball&>(first).bumped = true;
							const_cast<ball&>(second).bumped = true;
					}
			const_cast<ball&>(first).go();
			}
		}
	}
}


void ballCon::resetBumps()
{
	for (int i = 0; i < parser::ballQuan; ++i)
	{
		cont[i].bumped = false;
	}
}
void ballCon::findOne(int mX, int mY)
{
	for (int i = 0; i < parser::ballQuan; ++i)
	{
		if ((cont[i].getPos() - Eigen::Vector2f(mX, mY)).norm() < cont[i].getRad()) {
			whoIsHeld = &cont[i];
			return;
		}
	}
}





















ballCon::~ballCon()
{
	delete[] cont;
}