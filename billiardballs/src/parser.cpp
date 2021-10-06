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
void parser::setIf(int& var, const std::string to_find, std::string& line, std::ifstream& file)
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
		setIf(ballQuan, "ballquan", line, file);
		setIf(maxFPS, "maxfps", line, file);
		setIf(speedModifyer, "speedmodifyer", line, file);
	}
	file.close();
}

void parser::initialize()
{
	SCREENHEIGHT = 800;
	SCREENWIDTH = 1200;
	ballQuan = 50;
	maxFPS = 90;
	speedModifyer = 0.2f;
}