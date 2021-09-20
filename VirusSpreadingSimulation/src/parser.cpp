#include "parser.h"


parser::parser()
{
	parse("./conf.cfg");
}

parser::parser(const std::string filepath)
{
	parse(filepath);
}
void parser::setIf(bool& var, const std::string to_find, std::string& line, std::ifstream& file)
{
	if (line == to_find)
	{
		file >> line;
		if (line == "=")
		{
			file >> line;
			std::transform(line.begin(), line.end(), line.begin(), std::tolower);
			if (line == "false")
				var = false;
			else
				if (line == "true")
					var = true;
				else
					std::cerr << "WRONG BOOLEAN VALUE\n ONLY \"true\" AND \"false\" ACCEPTED!";
		}
		else
		{
			std::cerr << "CONFIGURATION FILE SYNTAX ERROR";
		}
	}
}
void parser::setIf(int& var, const std::string to_find, std::string &line, std::ifstream &file)
{
	if (line == to_find)
	{
		file >> line;
		if (line == "=")
		{
			file >> line;
			var = stoi(line);
			//std::cout << arr[0] << '=' << line << std::endl;
		}
		else
		{
			std::cerr << "CONFIGURATION FILE SYNTAX ERROR";
		}
	}
}
void parser::setIf(float& var, const std::string to_find, std::string& line, std::ifstream& file)
{
	if (line == to_find)
	{
		file >> line;
		if (line == "=")
		{
			file >> line;
			var = stof(line);
			//std::cout << arr[0] << '=' << line << std::endl;
		}
	}
}


void parser::parse(const std::string filepath)
{
	std::ifstream file;
	file.open(filepath);
	if (file.fail())
	{
		std::cerr << "FAILED TO OPEN CONFIGURATION FILE!";
	}
	std::string line;
	while (!file.eof())
	{
		file >> line;
		std::transform(line.begin(), line.end(), line.begin(), std::tolower);
		setIf(SCREENWIDTH, "screenwidth", line, file);
		setIf(SCREENHEIGHT, "screenheight", line, file);
		setIf(rad, "rad", line, file);
		setIf(menQuan, "menquan", line, file);
		setIf(infPulse, "infpulse", line, file);
		setIf(infChance, "infchance", line, file);
		setIf(infRad, "infrad", line, file);
		setIf(maxFPS, "maxfps", line, file);
		setIf(expMovement, "expmovement", line, file);
		setIf(dying, "dying", line, file);
		setIf(speedMultiplier, "speedmultiplier", line, file);
		setIf(quarantineZone, "quarantinezone", line, file);
		setIf(tradeCenter, "tradecenter", line, file);
		setIf(tradeCenterDuration, "tradecenterduration", line, file);

	}
	file.close();
}

void parser::initialize()
{
	SCREENHEIGHT = 800;
	SCREENWIDTH = 1200;
	rad = 15;
	menQuan = 60;
	infPulse = 1.0f;
	infChance = 10;
	infRad = 50;
	maxFPS = 60;
	expMovement = false;
	speedMultiplier = 2.0f;
	quarantineZone = 0;
	tradeCenter = 3.0f;
	tradeCenterDuration = 2.0f;
}