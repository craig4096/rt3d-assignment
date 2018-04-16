#pragma once
#include "Vector3.h"
#include <fstream>
#include "BoundingBox3d.h"
#include "Material.h"

class Block
{
public:
	Block(void);

	// constructs the block from a text file
	Block(std::ifstream& ifs);

	~Block(void);

	// block class represents a 3 dimensional cuboid which is
	// the basic building blocks for this game, is can be used
	// for collisions and also as a renderable mesh, artists can
	// use blocks to define walls etc.

	// draws this block
	void Draw();

	// returns the bounding box of this block
	BoundingBox3d GetBoundingBox() const;

	// sets the position of this block
	void SetPosition(const Vector3& pos);

private:

	// draws this block to the display list
	void CreateDisplayList();

	unsigned int displayList;

	// the center position of the block
	Vector3 position;

	// the block dimensions (x, y, z respectively)
	Vector3 size;


	Vector3 textureRepeat;

	Material material;


	unsigned int texture;
};
