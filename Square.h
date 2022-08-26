#pragma once
#include <SpriteBatch.h>
#include <string>
#include <GLTexture.h>
#include <glm/glm.hpp>

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

	private:
		float m_depth;
		glm::vec4 m_region; //minx, miny, width,height
		GLTexture m_texture;
        GLTexture m_clickedTexture;        
        bool m_hasClickedTexture;
        bool m_clicked;
};
