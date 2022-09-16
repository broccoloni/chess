#pragma once
#include <SpriteBatch.h>
#include <string>
#include <GLTexture.h>
#include <glm/glm.hpp>
#include <ResourceManager.h>
#include <iostream>

class Square
{
	public:
		Square();
		~Square();
		
		void init(glm::vec4 region, std::string texturePath, std::string clickedTexturePath = "");
        void click();
        bool isUnder(glm::vec2 mousepos);
        void draw(SpriteBatch& spriteBatch);
		void setDepth(float depth) { m_depth = depth; }
        void setPos(glm::vec2 pos) {m_region = glm::vec4(pos.x,pos.y,m_region[2],m_region[3]); }
        void setTexture(std::string texturePath){m_texture = ResourceManager::getTexture(texturePath.c_str()); } 
        void printDepth(){ std::cout<<m_depth<<std::endl; }

	private:
		float m_depth;
		glm::vec4 m_region; //minx, miny, width,height
		GLTexture m_texture;
        GLTexture m_clickedTexture;        
        bool m_hasClickedTexture;
        bool m_clicked;
};
