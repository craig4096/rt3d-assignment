#pragma once
#include <fstream>

class Color
{
public:
	Color(void);
	Color(std::ifstream& ifs);
	~Color(void);


	float red, green, blue, alpha;
};
