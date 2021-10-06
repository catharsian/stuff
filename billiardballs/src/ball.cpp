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
int ball::counter;
ALLEGRO_FONT* ball::fon;



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
	rad = randomizer::gen() % 15 + 7;
	pos = Eigen::Vector2f(randomizer::gen() % (parser::SCREENWIDTH - 100) + 50, randomizer::gen()%(parser::SCREENHEIGHT-100)+50);
	col = al_map_rgb(randomizer::gen()%256, randomizer::gen() % 256, randomizer::gen() % 256);
	dir = Eigen::Vector2f(0, 0);
	accel = Eigen::Vector2f(0, 1);
	n = counter;
	++counter;
	held = false;
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
	al_draw_text(fon, al_map_rgb(0, 0, 0), pos.x() - 7, pos.y() - 6, 0, std::to_string(n).c_str());
}

void ball::go()
{
	const float forY = pos.y() + dir.y() * parser::speedModifyer;
	const float forX = pos.x() + dir.x() * parser::speedModifyer;
	bool shit = true;
	if (forY > parser::SCREENHEIGHT - rad)
	{
		dir = Eigen::Vector2f(dir.x(), dir.y() * -1 + 1);
		//shit = false;
	}
	else if (forY < rad)
	{
		dir = Eigen::Vector2f(dir.x(), dir.y() * -1 - 1);
		//shit = false;
	}
	if (forX > parser::SCREENWIDTH - rad)
	{
		dir = Eigen::Vector2f(dir.x() * -1 + 1, dir.y());
		//shit = false;
	}
	else if (forX < rad)
	{
		dir = Eigen::Vector2f(dir.x() * -1 - 1, dir.y());
		//shit = false;
	}
	if (shit)
	{
		//dir += accel;
		pos += dir;
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
	//const Eigen::Vector2f firstdir = first.dir;
	//const Eigen::Vector2f seconddir = second.dir;
	first.dir = first.dir - ((first.pos - second.pos) * first.dir.norm());
	second.dir = second.dir -((first.pos - second.pos) * second.dir.norm());
}

void ball::moveUs(ball& first, ball& second)
{
	const auto diff = Eigen::Vector2f(first.getPos() - second.getPos());
	float most = first.rad + second.rad - diff.norm();
	first.pos+= diff.normalized() * most;
	second.pos -= diff.normalized() * most;
}





std::vector<float> ballCon::OverlappsX;
std::vector<float> ballCon::OverlappsY;



ballCon::ballCon()
{
	ball::counter = 1;
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
					std::cout << "Overlapped Ns = " << cont[i].n << " and " << cont[j].n << "; they're coords are 1x = " << cont[i].getPos().x() << ", 1y = " << cont[i].getPos().y() << "\n2x = "
						<< cont[j].getPos().x() << ", 2y = " << cont[j].getPos().y() << std::endl;
					OverlappsX.push_back(cont[i].getPos().x());
					OverlappsX.push_back(cont[j].getPos().x());
					OverlappsY.push_back(cont[i].getPos().y());
					OverlappsY.push_back(cont[j].getPos().y());
					ball::moveUs(cont[i], cont[j]);
					std::cout << "And now we are - 1x = " << cont[i].getPos().x() << ", 1y = " << cont[i].getPos().y() << "\n2x = "
						<< cont[j].getPos().x() << ", 2y = " << cont[j].getPos().y() << std::endl;
					goto again;
				}
			}
		}
	}

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
		if ((first.getPos() + first.getDir() - second.getPos()).norm() < (second.getRad() + first.getRad()))
		{
			ball::bump(const_cast<ball&>(first), const_cast<ball&>(second));
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
				if ((first.getPos() + first.getDir() - second.getPos()).norm() < (second.getRad() + first.getRad()))
				{
					ball::bump(const_cast<ball&>(first), const_cast<ball&>(second));
					const_cast<ball&>(first).go();
				}
				else
				{
					const_cast<ball&>(first).go();
				}
			}
		}
	}
}























ballCon::~ballCon()
{
	delete[] cont;
}