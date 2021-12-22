#include "TextureCache.h"
#include "ImageLoader.h"
#include <iostream>

TextureCache::TextureCache(){

}

TextureCache::~TextureCache(){

	
}

GLTexture TextureCache::getTexture(std::string texturePath){
	//lookup the texture and see if it's in the map
	auto mit = m_textureMap.find(texturePath);

	if (mit == m_textureMap.end()){
		//load the texture
		GLTexture newTexture = ImageLoader::loadPNG(texturePath);

		//insert it into the map
		m_textureMap.insert(make_pair(texturePath, newTexture));
		//std::cout <<"Loaded new Texture!\n";
		return newTexture;
	}
	//std::cout <<"Used Cached Texture!\n";
	return mit->second;
}
