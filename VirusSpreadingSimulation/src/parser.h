#pragma once
#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>
class parser
{
private:
	static void setIf(int& var, const std::string to_find, std::string& line, std::ifstream& file);
	static void setIf(float& var, const std::string to_find, std::string& line, std::ifstream& file);
	static void setIf(bool& var, const std::string to_find, std::string& line, std::ifstream& file);
public:
	parser();
	static void parse(const std::string filepath);
	static void initialize();
	parser(const std::string filepath);
	static int SCREENWIDTH;
	static int SCREENHEIGHT;
	static int rad;
	static int menQuan;
	static int infChance;
	static float infPulse;
	static float infRad;
	static int maxFPS;
	static bool expMovement;
	static int dying;
	static float speedMultiplier;
	static int quarantineZone;
	static float tradeCenter;
	static float tradeCenterDuration;
};

