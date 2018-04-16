#include "Color.h"

Color::Color(void)
{
}

Color::Color(std::ifstream& ifs)
{
	ifs >> red;
	ifs >> green;	
	ifs >> blue;
	alpha = 1.0;
}

Color::~Color(void)
{
}
