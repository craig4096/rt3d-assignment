#pragma once
#include "Vector3.h"
#include "Color.h"
#include <fstream>
#include <GL/freeglut.h>

class Light
{
public:
	Light(void);
	Light(std::ifstream& ifs);
	~Light(void);

	void Set(GLenum light);

private:

	Vector3 position;

	Color ambient;
	Color diffuse;
	Color specular;
	float shininess;

	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
};
