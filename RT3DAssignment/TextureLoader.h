#pragma once
#include <string>
#include <map>

class TextureLoader
{
public:
	TextureLoader(void);
	~TextureLoader(void);


	// loads a texture from the specified bmp file
	static unsigned int LoadTexture(const std::string& filename, bool buildMipMaps, unsigned int* width = 0, unsigned int* height = 0);

	static void ClearCache();

private:

	typedef std::map<std::string, unsigned int> TextureCache;
	static TextureCache textureCache;
};
