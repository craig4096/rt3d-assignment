#include "SkyBox.h"
#include <string>
#include "TextureLoader.h"
#include <GL/freeglut.h>
using namespace std;

SkyBox::SkyBox(void)
{
}

SkyBox::~SkyBox(void)
{
}


void SkyBox::Load(ifstream& ifs)
{
	unsigned int width, height;
	for(unsigned int i = 0; i < 6; ++i)
	{
		string tex;
		ifs >> tex;
		textures[i] = TextureLoader::LoadTexture(tex, true, &width, &height);
	}

	// calculate how much half a pixel is
	// (changed this to represent 2 pixels padding as some artifacts were still showing at 0.5 pixel for some reason)
	hp = (2.0f / (float)width);
}


void SkyBox::Draw(const Vector3& camPos)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glPushMatrix();
	glTranslatef(camPos.x, camPos.y, camPos.z);
	glScalef(500.0f, 500.0f, 500.0f);

	// half a pixel size
	float beg = 0.0f + hp;
	float end = 1.0f - hp;

	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glBegin(GL_QUADS);
			// draw the top side (y axis)
			glTexCoord2f(end, beg);	glVertex3f(1, 1, -1);
			glTexCoord2f(end, end);	glVertex3f(1, 1, 1);
			glTexCoord2f(beg, end);	glVertex3f(-1, 1, 1);
			glTexCoord2f(beg, beg);	glVertex3f(-1, 1, -1);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glBegin(GL_QUADS);
			// bottom side
			glTexCoord2f(beg, end);	glVertex3f(-1, -1, -1);
			glTexCoord2f(beg, beg);	glVertex3f(-1, -1, 1);
			glTexCoord2f(end, beg);	glVertex3f(1, -1, 1);
			glTexCoord2f(end, end);	glVertex3f(1, -1, -1);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glBegin(GL_QUADS);
			// front side (z axis)
			glTexCoord2f(end, beg);	glVertex3f(-1, -1, 1);
			glTexCoord2f(end, end);	glVertex3f(-1, 1, 1);
			glTexCoord2f(beg, end);	glVertex3f(1, 1, 1);
			glTexCoord2f(beg, beg);	glVertex3f(1, -1, 1);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textures[3]);
	glBegin(GL_QUADS);
			// back side
			glTexCoord2f(end, beg);	glVertex3f(1, -1, -1);
			glTexCoord2f(end, end);	glVertex3f(1, 1, -1);
			glTexCoord2f(beg, end);	glVertex3f(-1, 1, -1);
			glTexCoord2f(beg, beg);	glVertex3f(-1, -1, -1);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textures[4]);
	glBegin(GL_QUADS);
			// left side (x axis)
			glTexCoord2f(beg, beg);	glVertex3f(1, -1, -1);
			glTexCoord2f(end, beg);	glVertex3f(1, -1, 1);
			glTexCoord2f(end, end);	glVertex3f(1, 1, 1);
			glTexCoord2f(beg, end);	glVertex3f(1, 1, -1);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textures[5]);
	glBegin(GL_QUADS);
			// right side
			glTexCoord2f(end, end);	glVertex3f(-1, 1, -1);
			glTexCoord2f(beg, end);	glVertex3f(-1, 1, 1);
			glTexCoord2f(beg, beg);	glVertex3f(-1, -1, 1);
			glTexCoord2f(end, beg);	glVertex3f(-1, -1, -1);
	glEnd();

	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}
