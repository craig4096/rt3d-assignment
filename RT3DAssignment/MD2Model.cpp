#include "MD2Model.h"
#include <exception>
#include <fstream>
#include <GL/freeglut.h>
#include <iostream>
#include "Game.h"
using namespace std;


float anorms[162][3] = {
{ -0.525731f,  0.000000f,  0.850651f },
{ -0.442863f,  0.238856f,  0.864188f },
{ -0.295242f,  0.000000f,  0.955423f },
{ -0.309017f,  0.500000f,  0.809017f },
{ -0.162460f,  0.262866f,  0.951056f },
{  0.000000f,  0.000000f,  1.000000f },
{  0.000000f,  0.850651f,  0.525731f },
{ -0.147621f,  0.716567f,  0.681718f },
{  0.147621f,  0.716567f,  0.681718f },
{  0.000000f,  0.525731f,  0.850651f },
{  0.309017f,  0.500000f,  0.809017f },
{  0.525731f,  0.000000f,  0.850651f },
{  0.295242f,  0.000000f,  0.955423f },
{  0.442863f,  0.238856f,  0.864188f },
{  0.162460f,  0.262866f,  0.951056f },
{ -0.681718f,  0.147621f,  0.716567f },
{ -0.809017f,  0.309017f,  0.500000f },
{ -0.587785f,  0.425325f,  0.688191f },
{ -0.850651f,  0.525731f,  0.000000f },
{ -0.864188f,  0.442863f,  0.238856f },
{ -0.716567f,  0.681718f,  0.147621f },
{ -0.688191f,  0.587785f,  0.425325f },
{ -0.500000f,  0.809017f,  0.309017f },
{ -0.238856f,  0.864188f,  0.442863f },
{ -0.425325f,  0.688191f,  0.587785f },
{ -0.716567f,  0.681718f, -0.147621f },
{ -0.500000f,  0.809017f, -0.309017f },
{ -0.525731f,  0.850651f,  0.000000f },
{  0.000000f,  0.850651f, -0.525731f },
{ -0.238856f,  0.864188f, -0.442863f },
{  0.000000f,  0.955423f, -0.295242f },
{ -0.262866f,  0.951056f, -0.162460f },
{  0.000000f,  1.000000f,  0.000000f },
{  0.000000f,  0.955423f,  0.295242f },
{ -0.262866f,  0.951056f,  0.162460f },
{  0.238856f,  0.864188f,  0.442863f },
{  0.262866f,  0.951056f,  0.162460f },
{  0.500000f,  0.809017f,  0.309017f },
{  0.238856f,  0.864188f, -0.442863f },
{  0.262866f,  0.951056f, -0.162460f },
{  0.500000f,  0.809017f, -0.309017f },
{  0.850651f,  0.525731f,  0.000000f },
{  0.716567f,  0.681718f,  0.147621f },
{  0.716567f,  0.681718f, -0.147621f },
{  0.525731f,  0.850651f,  0.000000f },
{  0.425325f,  0.688191f,  0.587785f },
{  0.864188f,  0.442863f,  0.238856f },
{  0.688191f,  0.587785f,  0.425325f },
{  0.809017f,  0.309017f,  0.500000f },
{  0.681718f,  0.147621f,  0.716567f },
{  0.587785f,  0.425325f,  0.688191f },
{  0.955423f,  0.295242f,  0.000000f },
{  1.000000f,  0.000000f,  0.000000f },
{  0.951056f,  0.162460f,  0.262866f },
{  0.850651f, -0.525731f,  0.000000f },
{  0.955423f, -0.295242f,  0.000000f },
{  0.864188f, -0.442863f,  0.238856f },
{  0.951056f, -0.162460f,  0.262866f },
{  0.809017f, -0.309017f,  0.500000f },
{  0.681718f, -0.147621f,  0.716567f },
{  0.850651f,  0.000000f,  0.525731f },
{  0.864188f,  0.442863f, -0.238856f },
{  0.809017f,  0.309017f, -0.500000f },
{  0.951056f,  0.162460f, -0.262866f },
{  0.525731f,  0.000000f, -0.850651f },
{  0.681718f,  0.147621f, -0.716567f },
{  0.681718f, -0.147621f, -0.716567f },
{  0.850651f,  0.000000f, -0.525731f },
{  0.809017f, -0.309017f, -0.500000f },
{  0.864188f, -0.442863f, -0.238856f },
{  0.951056f, -0.162460f, -0.262866f },
{  0.147621f,  0.716567f, -0.681718f },
{  0.309017f,  0.500000f, -0.809017f },
{  0.425325f,  0.688191f, -0.587785f },
{  0.442863f,  0.238856f, -0.864188f },
{  0.587785f,  0.425325f, -0.688191f },
{  0.688191f,  0.587785f, -0.425325f },
{ -0.147621f,  0.716567f, -0.681718f },
{ -0.309017f,  0.500000f, -0.809017f },
{  0.000000f,  0.525731f, -0.850651f },
{ -0.525731f,  0.000000f, -0.850651f },
{ -0.442863f,  0.238856f, -0.864188f },
{ -0.295242f,  0.000000f, -0.955423f },
{ -0.162460f,  0.262866f, -0.951056f },
{  0.000000f,  0.000000f, -1.000000f },
{  0.295242f,  0.000000f, -0.955423f },
{  0.162460f,  0.262866f, -0.951056f },
{ -0.442863f, -0.238856f, -0.864188f },
{ -0.309017f, -0.500000f, -0.809017f },
{ -0.162460f, -0.262866f, -0.951056f },
{  0.000000f, -0.850651f, -0.525731f },
{ -0.147621f, -0.716567f, -0.681718f },
{  0.147621f, -0.716567f, -0.681718f },
{  0.000000f, -0.525731f, -0.850651f },
{  0.309017f, -0.500000f, -0.809017f },
{  0.442863f, -0.238856f, -0.864188f },
{  0.162460f, -0.262866f, -0.951056f },
{  0.238856f, -0.864188f, -0.442863f },
{  0.500000f, -0.809017f, -0.309017f },
{  0.425325f, -0.688191f, -0.587785f },
{  0.716567f, -0.681718f, -0.147621f },
{  0.688191f, -0.587785f, -0.425325f },
{  0.587785f, -0.425325f, -0.688191f },
{  0.000000f, -0.955423f, -0.295242f },
{  0.000000f, -1.000000f,  0.000000f },
{  0.262866f, -0.951056f, -0.162460f },
{  0.000000f, -0.850651f,  0.525731f },
{  0.000000f, -0.955423f,  0.295242f },
{  0.238856f, -0.864188f,  0.442863f },
{  0.262866f, -0.951056f,  0.162460f },
{  0.500000f, -0.809017f,  0.309017f },
{  0.716567f, -0.681718f,  0.147621f },
{  0.525731f, -0.850651f,  0.000000f },
{ -0.238856f, -0.864188f, -0.442863f },
{ -0.500000f, -0.809017f, -0.309017f },
{ -0.262866f, -0.951056f, -0.162460f },
{ -0.850651f, -0.525731f,  0.000000f },
{ -0.716567f, -0.681718f, -0.147621f },
{ -0.716567f, -0.681718f,  0.147621f },
{ -0.525731f, -0.850651f,  0.000000f },
{ -0.500000f, -0.809017f,  0.309017f },
{ -0.238856f, -0.864188f,  0.442863f },
{ -0.262866f, -0.951056f,  0.162460f },
{ -0.864188f, -0.442863f,  0.238856f },
{ -0.809017f, -0.309017f,  0.500000f },
{ -0.688191f, -0.587785f,  0.425325f },
{ -0.681718f, -0.147621f,  0.716567f },
{ -0.442863f, -0.238856f,  0.864188f },
{ -0.587785f, -0.425325f,  0.688191f },
{ -0.309017f, -0.500000f,  0.809017f },
{ -0.147621f, -0.716567f,  0.681718f },
{ -0.425325f, -0.688191f,  0.587785f },
{ -0.162460f, -0.262866f,  0.951056f },
{  0.442863f, -0.238856f,  0.864188f },
{  0.162460f, -0.262866f,  0.951056f },
{  0.309017f, -0.500000f,  0.809017f },
{  0.147621f, -0.716567f,  0.681718f },
{  0.000000f, -0.525731f,  0.850651f },
{  0.425325f, -0.688191f,  0.587785f },
{  0.587785f, -0.425325f,  0.688191f },
{  0.688191f, -0.587785f,  0.425325f },
{ -0.955423f,  0.295242f,  0.000000f },
{ -0.951056f,  0.162460f,  0.262866f },
{ -1.000000f,  0.000000f,  0.000000f },
{ -0.850651f,  0.000000f,  0.525731f },
{ -0.955423f, -0.295242f,  0.000000f },
{ -0.951056f, -0.162460f,  0.262866f },
{ -0.864188f,  0.442863f, -0.238856f },
{ -0.951056f,  0.162460f, -0.262866f },
{ -0.809017f,  0.309017f, -0.500000f },
{ -0.864188f, -0.442863f, -0.238856f },
{ -0.951056f, -0.162460f, -0.262866f },
{ -0.809017f, -0.309017f, -0.500000f },
{ -0.681718f,  0.147621f, -0.716567f },
{ -0.681718f, -0.147621f, -0.716567f },
{ -0.850651f,  0.000000f, -0.525731f },
{ -0.688191f,  0.587785f, -0.425325f },
{ -0.587785f,  0.425325f, -0.688191f },
{ -0.425325f,  0.688191f, -0.587785f },
{ -0.425325f, -0.688191f, -0.587785f },
{ -0.587785f, -0.425325f, -0.688191f },
{ -0.688191f, -0.587785f, -0.425325f }
};


MD2Model::ModelCache MD2Model::modelCache;



MD2Model* MD2Model::Load(const std::string& filename)
{
	MD2Model* model;
	ModelCache::iterator i = modelCache.find(filename);
	if(i == modelCache.end())
	{
		model = new MD2Model(filename.c_str());
		modelCache[filename] = model;
	}
	else
	{
		model = i->second;
	}
	return model;
}

void MD2Model::ClearCache()
{
	// delete all models
	for(ModelCache::iterator i = modelCache.begin(); i != modelCache.end(); ++i)
	{
		delete i->second;
	}
	modelCache.clear();
}

MD2Model::MD2Model(const char* filename)
{
	ifstream ifs((Game::modelDirectory + filename).c_str(), ios::binary);
	if(!ifs.is_open())
	{
throw (Game::modelDirectory + filename + ":   could not open").c_str();
		//throw "could not open md2 file to read";
	}

	// read in the header
	Header header;
	ifs.read((char*)&header, sizeof(header));

	static int MAGIC_NUMBER = (int('2') << 24 | int('P') << 16 | int('D') << 8 | int('I'));

	if(header.magicNumber != MAGIC_NUMBER)
	{
		throw "could not load md2 file: magic number does not match";
	}


	// load in the texture coordinates first
	texCoords.resize(header.num_st);
	ifs.seekg(header.ofs_st);
	for(int i = 0; i < header.num_st; ++i)
	{
		struct Tex { short s, t; } st;

		ifs.read((char*)&st, sizeof(Tex));

		// convert the texture coordinate to normalized float(0.0 - 1.0)
		texCoords[i].s = st.s / (float)header.skinwidth;
		texCoords[i].t = st.t / (float)header.skinheight;
	}


	// read in the triangle array
	triangles.resize(header.num_tris);
	ifs.seekg(header.ofs_tris);
	ifs.read((char*)&triangles[0], sizeof(Triangle) * header.num_tris);

	// cache the num_xyz value
	this->numPositionsPerFrame = header.num_xyz;
	this->frameCount = header.num_frames;

	// read in the frames
	vertices.resize(header.num_frames * header.num_xyz);
	ifs.seekg(header.ofs_frames);
	int curPos = 0;

	for(int i = 0; i < header.num_frames; ++i)
	{
		Frame frame;
		ifs.read((char*)&frame, sizeof(Frame));

//std::cout << frame.name << "  " << i << endl;

		// read in all the frame data
		for(int j = 0; j < header.num_xyz; ++j)
		{
			FrameData data;
			ifs.read((char*)&data, sizeof(data));

			vertices[curPos].pos.x = ((float)data.v[0] * frame.scale[0]) + frame.translate[0];
			vertices[curPos].pos.z = ((float)data.v[1] * frame.scale[1]) + frame.translate[1];
			vertices[curPos].pos.y = ((float)data.v[2] * frame.scale[2]) + frame.translate[2];

			vertices[curPos].normalIndex = data.lightnormalindex;

			// next frame data
			curPos++;
		}
	}

	// load the skins
	if(header.num_skins != 0)
	{
		skins.resize(header.num_skins);
		ifs.seekg(header.ofs_skins);
		ifs.read((char*)&skins[0], sizeof(Skin) * header.num_skins);
	}


	// close the file
	ifs.close();
}


MD2Model::~MD2Model(void)
{
}

void MD2Model::SetFrameNumber(float frameNo)
{
	if(frameNo < frameCount-1 && frameNo >= 0.0)
	{
		this->frameNo = frameNo;
	}
}

unsigned int MD2Model::GetFrameCount() const
{
	return frameCount;
}


void MD2Model::Draw()
{
	// find the two frames the current frame number lies inbetween
	int start = (int)frameNo;
	int end = start + 1;

	// interpolation ratio (between 0 and 1)
	float ratio = frameNo - (float)start;

	// get a pointer to the vertices of both the start and end frames (a and b respectively)
	Vertex* aVertices = &vertices[start * numPositionsPerFrame];
	Vertex* bVertices = &vertices[end * numPositionsPerFrame];

	glBegin(GL_TRIANGLES);
	// now iterate through the triangles and blend the vertices together
	for(unsigned int i = 0; i < triangles.size(); ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			Vertex& a = aVertices[triangles[i].vertexindex[j]];
			Vertex& b = bVertices[triangles[i].vertexindex[j]];

			Vector3 aNorm = anorms[ a.normalIndex ];
			Vector3 bNorm = anorms[ b.normalIndex ];

			// now interpolate the positions together based on 'ratio'
			Vector3 fPos = a.pos + ((b.pos - a.pos) * ratio);
			Vector3 fNorm = aNorm + ((bNorm - aNorm) * ratio);

			// get the texture coordinate
			TexCoord tc = texCoords[triangles[i].textureindex[j]];

			glNormal3f(fNorm.x, fNorm.y, fNorm.z);
			glTexCoord2f(tc.s, tc.t);
			glVertex3f(fPos.x, fPos.y, fPos.z);
		}
	}
	glEnd();
}
