#pragma once
#include "alEngine.h"

struct mouseSpline
{
	Eigen::Vector2f* ps;
	unsigned char n;
	unsigned char max;
	mouseSpline()
	{}
	mouseSpline(unsigned char _n)
	{
		ps = new Eigen::Vector2f[_n];
		n = _n;
		max = 0;
	}
	void Create(unsigned char _n)
	{
		ps = new Eigen::Vector2f[_n];
		n = _n;
		max = 0;
	}
	void append(const Eigen::Vector2f newP)
	{
		for (int i = 0; i < n - 1; ++i)
		{
			ps[i + 1] = ps[i];
		}
		ps[0] = newP;
		if(max < n - 1)
			++max;
	}
	Eigen::Vector2f getLast() const
	{
		return ps[max];
	}
	Eigen::Vector2f getFirst() const
	{
		return ps[0];
	}
	Eigen::Vector2f getDiff()
	{
		return Eigen::Vector2f(getFirst() - getLast());
	}
	Eigen::Vector2f operator[](const unsigned int i)
	{
		return ps[i];
	}
	~mouseSpline()
	{
		delete[] ps;
	}

};


class endEngine : public baseEngine
{
private:
	ballCon con;
	int mX, mY;
	bool mouseHeld;
	mouseSpline poses;
public:
	void  Start() override;
	bool Update() override;

	endEngine();
	~endEngine();
};



