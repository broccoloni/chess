#include "Square.h"
#include <ResourceManager.h>
#include <iostream>
Square::Square(){
    m_clicked = false;	
    m_hasClickedTexture = false;
    m_region = glm::vec4(0,0,0,0);
    m_depth = 0.0;
}

Square::~Square(){

}

void Square::init(glm::vec4 region, std::string texturePath, std::string clickedTexturePath){
	m_region = region;
	m_texture = ResourceManager::getTexture(texturePath.c_str());
    if (clickedTexturePath != ""){
        m_hasClickedTexture = true;
        m_clickedTexture = ResourceManager::getTexture(clickedTexturePath.c_str());
    }
}

void Square::click(){
    m_clicked = !m_clicked;
}

bool Square::isUnder(glm::vec2 mousepos){
    //std::cout<<"("<<mousepos.x<<", "<<mousepos.y<<")\t("<<m_region[0]<<"-"<<m_region[0]+m_region[2]<<","<<m_region[1]<<"-"<<m_region[1]+m_region[3]<<")"<<std::endl;
    if ((int)mousepos.x < (int)m_region[0]) {
        return false; 
    }
    if ((int)mousepos.x > (int)m_region[0]+(int)m_region[2]) {
        return false;
    }
    if ((int)mousepos.y < (int)m_region[1]) {
        return false;
    }
    if ((int)mousepos.y > (int)m_region[1]+(int)m_region[3]) {
        return false;
    }
    return true;
}

void Square::draw(SpriteBatch& spriteBatch){
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	Colour colour;
	colour.r = 255;
	colour.g = 255;
	colour.b = 255;
	colour.a = 255;
    if (!m_clicked){
	    spriteBatch.draw(m_region, uv, m_texture.id, colour, m_depth);
    }
    else{
	    spriteBatch.draw(m_region, uv, m_clickedTexture.id, colour, m_depth);
    }
}






