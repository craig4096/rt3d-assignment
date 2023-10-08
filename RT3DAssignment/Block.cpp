#include "Block.h"
#include <GL/freeglut.h>
#include "TextureLoader.h"
#include <string>
using namespace std;

Block::Block(void)
{
}

Block::Block(ifstream& ifs)
{
	position = Vector3(ifs);
	size = Vector3(ifs);
	textureRepeat = Vector3(ifs);

	std::string textureName;
	ifs >> textureName;

	this->texture = TextureLoader::LoadTexture(textureName.c_str(), true);

	material = Material(ifs);

	CreateDisplayList();
}

Block::~Block(void)
{
}

void Block::Draw()
{
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
	glScalef(size.x, size.y, size.z);

	material.Set();

	glCallList(displayList);

	glPopMatrix();
}


void Block::CreateDisplayList()
{
	displayList = glGenLists(1);
	glNewList(displayList, GL_COMPILE);

	float texX = size.x / textureRepeat.x;
	float texY = size.y / textureRepeat.y;
	float texZ = size.z / textureRepeat.z;

	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
		// draw the top side (y axis)
		glNormal3f(0, 1, 0);
		glTexCoord2f(0.0, texZ);
		glVertex3f(-1, 1, 1);

		glNormal3f(0, 1, 0);
		glTexCoord2f(texX, texZ);
		glVertex3f(1, 1, 1);

		glNormal3f(0, 1, 0);
		glTexCoord2f(texX, 0.0);
		glVertex3f(1, 1, -1);

		glNormal3f(0, 1, 0);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-1, 1, -1);

		// draw the bottom side
		glNormal3f(0, -1, 0);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-1, -1, -1);

		glNormal3f(0, -1, 0);
		glTexCoord2f(texX, 0.0);
		glVertex3f(1, -1, -1);

		glNormal3f(0, -1, 0);
		glTexCoord2f(texX, texZ);
		glVertex3f(1, -1, 1);

		glNormal3f(0, -1, 0);
		glTexCoord2f(0.0, texZ);
		glVertex3f(-1, -1, 1);

		// draw the left side (x axis)
		glNormal3f(1, 0, 0);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(1, -1, -1);

		glNormal3f(1, 0, 0);
		glTexCoord2f(texY, 0.0);
		glVertex3f(1, 1, -1);

		glNormal3f(1, 0, 0);
		glTexCoord2f(texY, texZ);
		glVertex3f(1, 1, 1);

		glNormal3f(1, 0, 0);
		glTexCoord2f(0.0, texZ);
		glVertex3f(1, -1, 1);

		// draw the right side
		glNormal3f(-1, 0, 0);
		glTexCoord2f(0.0, texZ);
		glVertex3f(-1, -1, 1);

		glNormal3f(-1, 0, 0);
		glTexCoord2f(texY, texZ);
		glVertex3f(-1, 1, 1);

		glNormal3f(-1, 0, 0);
		glTexCoord2f(texY, 0.0);
		glVertex3f(-1, 1, -1);

		glNormal3f(-1, 0, 0);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-1, -1, -1);

		// draw the front side (z axis)
		glNormal3f(0, 0, 1);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-1, -1, 1);

		glNormal3f(0, 0, 1);
		glTexCoord2f(texX, 0.0);
		glVertex3f(1, -1, 1);

		glNormal3f(0, 0, 1);
		glTexCoord2f(texX, texY);
		glVertex3f(1, 1, 1);

		glNormal3f(0, 0, 1);
		glTexCoord2f(0.0, texY);
		glVertex3f(-1, 1, 1);


		// draw the back side
		glNormal3f(0, 0, -1);
		glTexCoord2f(0.0, texY);
		glVertex3f(-1, 1, -1);

		glNormal3f(0, 0, -1);
		glTexCoord2f(texX, texY);
		glVertex3f(1, 1, -1);

		glNormal3f(0, 0, -1);
		glTexCoord2f(texX, 0.0);
		glVertex3f(1, -1, -1);

		glNormal3f(0, 0, -1);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-1, -1, -1);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, -1);

	glEndList();

}


BoundingBox3d Block::GetBoundingBox() const
{
	BoundingBox3d box(position - size);
	box.max = box.min + (size*2.0);
	return box;
}
