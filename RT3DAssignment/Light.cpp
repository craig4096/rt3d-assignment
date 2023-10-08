#include "Light.h"


Light::Light(void)
{
}

Light::Light(std::ifstream& ifs)
{
	position = Vector3(ifs);
	ambient = Color(ifs);
	diffuse = Color(ifs);
	specular = Color(ifs);
	ifs >> shininess;
	ifs >> constantAttenuation;
	ifs >> linearAttenuation;
	ifs >> quadraticAttenuation;
}

Light::~Light(void)
{
}


void Light::Set(GLenum light)
{
	float pos[4] = { position.x, position.y, position.z, 1.0 };
	glLightfv(light, GL_POSITION, pos);
	glLightfv(light, GL_DIFFUSE, (float*)&diffuse);
	glLightfv(light, GL_AMBIENT, (float*)&ambient);
	glLightfv(light, GL_SPECULAR, (float*)&specular);
	//glLightf(light, GL_SHININESS, shininess);
	glLightf(light, GL_CONSTANT_ATTENUATION, constantAttenuation);
	glLightf(light, GL_LINEAR_ATTENUATION, linearAttenuation);
	glLightf(light, GL_QUADRATIC_ATTENUATION, quadraticAttenuation);
}
