#include "TextureLoader.h"
#include "glbmp.h"
#include <GL/freeglut.h>
#include "Game.h"
#include <map>
using namespace std;

TextureLoader::TextureCache TextureLoader::textureCache;

TextureLoader::TextureLoader(void)
{
}

TextureLoader::~TextureLoader(void)
{
}


unsigned int TextureLoader::LoadTexture(const std::string& filename, bool buildMipMaps, unsigned int* width, unsigned int* height)
{
	TextureCache::iterator i = textureCache.find(filename);

	if(i == textureCache.end())
	{
		// texture not found, need to load it
		glbmp_t bmp;
		if(!glbmp_LoadBitmap((Game::textureDirectory + filename).c_str(), 0, &bmp))
		{
			return -1;
		}

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);


		if(buildMipMaps)
		{
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.width, bmp.height, GL_RGB, GL_UNSIGNED_BYTE, bmp.rgb_data);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, 3, bmp.width, bmp.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp.rgb_data);
		}

		// set the mag and min filters for rendering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, buildMipMaps ? GL_NEAREST_MIPMAP_LINEAR : GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// set parameters for texture repeat
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		if(width) *width = bmp.width;
		if(height) *height = bmp.height;

		glbmp_FreeBitmap(&bmp);

		// add texture to cache
		textureCache[filename] = texture;

		return texture;
	}
	else
	{
		// cache hit
		return i->second;
	}
}


void TextureLoader::ClearCache()
{
	for(TextureCache::iterator i = textureCache.begin(); i != textureCache.end(); ++i)
	{
		glDeleteTextures(1, &i->second);
	}
	textureCache.clear();
}
