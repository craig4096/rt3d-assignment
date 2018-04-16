#pragma once

#include "Vector3.h"
#include <vector>
#include <map>

class MD2Model
{
public:

	static MD2Model* Load(const std::string& filename);

	static void ClearCache();

	// sets the current frame of animation
	void SetFrameNumber(float frameNo);

	// returns the number of frames of animation
	unsigned int GetFrameCount() const;

	// draws the model (does not bind any textures) this is left up
	// to the caller
	void Draw();

private:

	MD2Model(const char* filename);
	~MD2Model(void);

	// structures for loading data from file
	struct Header
	{
		int magicNumber;
		int version;
		int skinwidth;
		int skinheight;
		int framesize;
		int num_skins;
		int num_xyz;
		int num_st;
		int num_tris;
		int num_glcmds;
		int num_frames;
		int ofs_skins;
		int ofs_st;
		int ofs_tris;
		int ofs_frames;
		int ofs_glcmds;
		int ofs_end;
	};

	struct FrameData
	{
		unsigned char v[3];
		unsigned char lightnormalindex;
	};

	struct Frame
	{
		float scale[3];
		float translate[3];
		char name[16];
	};


	struct Triangle
	{
		short vertexindex[3];
		short textureindex[3];
	};

	struct TexCoord
	{
		float s, t;
	};

	struct Skin
	{
		char name[64];
	};

	struct Vertex
	{
		Vector3 pos;
		unsigned int normalIndex;
	};


	// number of positions for each frame
	int numPositionsPerFrame;
	int frameCount;

	// the current frame the animation is set to
	float frameNo;


	std::vector<Triangle> triangles;
	std::vector<Vertex> vertices;
	std::vector<TexCoord> texCoords;
	std::vector<Skin> skins;


	typedef std::map<std::string, MD2Model*> ModelCache;
	static ModelCache modelCache;
};
