#include "Heightmap.h"
#include "glbmp.h"
#include <GL/freeglut.h>
#include <exception>
#define _USE_MATH_DEFINES
#include <math.h>
#include "TextureLoader.h"
#include "Game.h"


Heightmap::Heightmap(const char* filename, const char* textureFile, float scale, float height, float tile)
{
	glbmp_t bmp;
	if(!glbmp_LoadBitmap((Game::textureDirectory + filename).c_str(), 0, &bmp))
	{
		throw "could not load heightmap bmp file";
	}

	this->scale = scale;

	this->width = bmp.width;
	this->height = bmp.height;

	// resize the vertex array to be the same size as the image
	vertices.resize(bmp.width * bmp.height);
	texCoords.resize(bmp.width * bmp.height);

	normals.resize(bmp.width * bmp.height);

	// the pitch of the image
	int pitch = bmp.width * 3;

	this->halfWidth = ((bmp.width-1) * scale) / 2.0;
	this->halfHeight = ((bmp.height-1) * scale) / 2.0;

	// now loop through each pixel in the bitmap and construct
	// a position per pixel
	for(int y = 0; y < bmp.height; ++y)
	{
		for(int x = 0; x < bmp.width; ++x)
		{
			// the destination vertex
			Vector3& dst = vertices[y * bmp.width + x];
			TexCoord& txdst = texCoords[y * bmp.width + x];

			// set the x and y components
			// subtract halfWidth + Height to center the heightmap
			dst.x = (x * scale) - halfWidth;
			dst.z = (y * scale) - halfHeight;

			// set the texture coordinates
			txdst.u = (x / (float)bmp.width) * tile;
			txdst.v = (y / (float)bmp.height) * tile;

			// we can use any component (rgb) to determine the height as the
			// bmp will be grayscale anyway so r=g=b
			unsigned char red = bmp.rgb_data[y * pitch + (x*3)];

			// get the normalized height value between 0 and 1
			float nHeight = red / 255.0;

			// set the height of teh vertex
			dst.y = nHeight * height;
		}
	}

	// now construct the index buffer, between every height value we will
	// need 6 indices, therefore resize the array to...
	indices.resize((bmp.width-1) * (bmp.height-1) * 6);

	// now construct the indices
	unsigned int i = 0;
	for(int z = 0; z < bmp.height-1; ++z)
	{
		for(int x = 0; x < bmp.width-1; ++x)
		{
			int bl = (z+1) * bmp.width + x;		// bottom left
			int br = (z+1) * bmp.width + (x+1);	// bottom right
			int tr = z * bmp.width + (x+1);		// top right
			int tl = z * bmp.width + x;			// top left

			// first tri
			indices[i++] = bl;
			indices[i++] = tr;
			indices[i++] = tl;

			// second tri
			indices[i++] = bl;
			indices[i++] = br;
			indices[i++] = tr;
		}
	}

	glbmp_FreeBitmap(&bmp);

	// load the texture
	texture = TextureLoader::LoadTexture(textureFile, true);

	// calculate the normals
	CalculateSmoothNormals();
}

Heightmap::~Heightmap(void)
{
}



void Heightmap::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glBindTexture(GL_TEXTURE_2D, this->texture);

	//glColor3f(1,1,1);
	//glPolygonMode(GL_FRONT, GL_LINE);

	// set the vertex buffer
	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &texCoords[0]);
	glNormalPointer(GL_FLOAT, 0, &normals[0]);

	// draw the vertices
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, &indices[0]);

	glBindTexture(GL_TEXTURE_2D, -1);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}


float Heightmap::GetHeight(float xWorld, float zWorld) const
{
	// translate the coordinates to positive space
	float x = xWorld + halfWidth;
	float z = zWorld + halfHeight;

	// divide by the scale factor
	x /= scale;
	z /= scale;

	// we now need to round the x and z values to get
	// the index of the bottom left vertex
	int xi = floor(x);
	int zi = floor(z);

	// check for out of bounds
	if(xi < 0 || xi > width-2 || zi < 0 || zi > height-2)
	{
		return 0.0;
	}

	// we can now get the four vertices that surround the
	// specified position
	const Vector3& br = this->vertices[zi * width + xi];			// bottom right
	const Vector3& bl = this->vertices[zi * width + (xi+1)];		// bottom left
	const Vector3& tr = this->vertices[(zi+1) * width + xi];		// top right
	const Vector3& tl = this->vertices[(zi+1) * width + (xi+1)];	// top left

	// considering the four vertices as a box caluculate the
	// coordinates of the world pos relative to this box (normalized)
	float xNorm = (xWorld - bl.x) / (br.x - bl.x);
	float zNorm = (zWorld - br.z) / (tr.z - br.z);


	// 0,1|-----|1,1
	//    | B / |
	//    |  /  |
	//    | /   |
	//    |/  A |
	// 0,0|-----|1,0


	// now determine what triangle will be used
	if((xNorm + (1.0 - zNorm)) > 1.0)
	{
		// use bottom right triangle of quad (A)
		return bl.y + (((br.y - bl.y) * xNorm) + ((tr.y - br.y) * zNorm));
	}
	else // < 1.0
	{
		// use top left triangle of quad (B)
		return bl.y + ((tr.y - tl.y) * xNorm) + ((tl.y - bl.y) * zNorm);
	}
}


Vector3 Heightmap::GetNormal(const Vector3& a, const Vector3& b, const Vector3& c)
{
	return (c - a).CrossProduct(b - a);
}

void Heightmap::CalculateSmoothNormals()
{
	int normalCount = 0;
	Vector3 normals[6];

	for(unsigned int z = 0; z < height; ++z)
	{
		for(unsigned int x = 0; x < width; ++x)
		{
			if(x > 0 && z > 0)
			{
				// bottom left (upper) triangle
				normals[normalCount++] =
					GetNormal(
						vertices[(z-1) * width + (x-1)],	// a
						vertices[z * width + x],			// b
						vertices[z * width + (x-1)]			// c
					);

				// bottom left (lower) triangle
				normals[normalCount++] =
					GetNormal(
						vertices[(z-1) * width + (x-1)],
						vertices[(z-1) * width + x],
						vertices[z * width + x]
					);
			}

			if(x < width-1 && z < height-1)
			{
				// top right (upper) triangle
				normals[normalCount++] =
					GetNormal(
						vertices[z * width + x],
						vertices[(z+1) * width + (x+1)],
						vertices[(z+1) * width + x]
					);

				// top right (lower) triangle
				normals[normalCount++] =
					GetNormal(
						vertices[z * width + x],
						vertices[z * width + (x+1)],
						vertices[(z+1) * width + (x+1)]
					);
			}

			if(x > 0 && z < height-1)
			{
				// top left
				normals[normalCount++] =
					GetNormal(
						vertices[z * width + (x-1)],
						vertices[z * width + x],
						vertices[(z+1) * width + x]
					);
			}

			if(x < width-1 && z > 0)
			{
				// bottom right
				normals[normalCount++] =
					GetNormal(
						vertices[(z-1) * width + x],
						vertices[z * width + (x+1)],
						vertices[z * width + x]
					);
			}


			Vector3 avg;
			// now get the averaged normal vector
			for(unsigned int i = 0; i < normalCount; ++i)
			{
				avg += normals[i];
			}
			avg = avg / (float)normalCount;
			normalCount = 0;

			this->normals[z * width + x] = avg;
		}
	}
}
