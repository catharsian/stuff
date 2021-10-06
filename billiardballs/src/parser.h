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
	static int ballQuan;
	static int maxFPS;
	static float speedModifyer;
};

