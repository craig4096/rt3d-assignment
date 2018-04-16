#include "Material.h"
#include <GL/freeglut.h>


Material::Material(void)
{
}

Material::Material(std::ifstream& ifs)
{
	ambient = Color(ifs);
	diffuse = Color(ifs);
	specular = Color(ifs);
	emissive = Color(ifs);
	ifs >> shininess;
}

Material::~Material(void)
{
}


void Material::Set()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, (float*)&ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, (float*)&diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, (float*)&specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, (float*)&emissive);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}
