#include "Background.h"
#include <ResourceManager.h>

Background::Background(){
	
}

Background::~Background(){

}

void Background::init(glm::vec4 region, std::string filePath){
	m_region = region;
	m_texture = ResourceManager::getTexture(filePath.c_str());
}

void Background::draw(SpriteBatch& spriteBatch){
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	Colour colour;
	colour.r = 255;
	colour.g = 255;
	colour.b = 255;
	colour.a = 255;
	spriteBatch.draw(m_region, uv, m_texture.id, colour, m_depth);
}
