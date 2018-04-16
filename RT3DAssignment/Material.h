#pragma once
#include "Color.h"

class Material
{
public:
	Material(void);
	Material(std::ifstream& ifs);
	~Material(void);

	void Set();

private:

	Color ambient;
	Color diffuse;
	Color specular;
	Color emissive;
	float shininess; 
};
