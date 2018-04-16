#pragma once

#include "Vector3.h"
#include <vector>

class Heightmap
{
public:

	// loads the heightmap from a bitmap image
	// scale determines how far apart the vertices are in the x and z axis
	// height determines how high the heightmap should stretch to
	// tile - how much tiling to apply to the texture
	// texture - the diffuse texture that will cover the terrain
	Heightmap(const char* heightmap, const char* texture, float scale, float height, float tile);
	~Heightmap(void);

	// draws the heightmap
	void Draw();

	// returns the size of the heightmap (x axis)
	float GetXSize() const { return (width-1) * scale; }

	// returns the size of the heightmap (z axis)
	float GetZSize() const { return (height-1) * scale; }

	// this function will return a height value given the specified x and z world coordinate
	float GetHeight(float x, float z) const;

private:

	static Vector3 GetNormal(const Vector3& a, const Vector3& b, const Vector3& c);

	// calculates smooth normals for the heightmap,
	// stores result in 'normals' array
	void CalculateSmoothNormals();

	struct TexCoord
	{
		float u, v;
	};

	// texture coordinate array
	std::vector<TexCoord> texCoords;

	// the vertex array
	std::vector<Vector3> vertices;

	std::vector<Vector3> normals;

	// index buffer
	std::vector<unsigned int> indices;
	unsigned int width, height;


	// half the dimension of the heightmap (xz) in world metric
	float halfWidth, halfHeight;

	float scale;

	// the texture used for texture mapping the terrain
	unsigned int texture;
};
