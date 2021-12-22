#pragma once
#include <SpriteBatch.h>
#include <string>
#include <GLTexture.h>
#include <glm/glm.hpp>

class Background
{
	public:
		Background();
		~Background();
		
		void init(glm::vec4 region, std::string texturePath);
		void draw(SpriteBatch& spriteBatch);

		void setDepth(float depth) { m_depth = depth; }
	private:
		float m_depth;
		glm::vec4 m_region;
		GLTexture m_texture;		
};
