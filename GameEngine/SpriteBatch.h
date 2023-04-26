#pragma once
#include <GL/glew.h>
//#include <GLES2/gl2.h>
//#include <GLES2/gl2ext.h>
#include "Vertex.h"
#include <glm/glm.hpp>
#include <vector>

enum class GlyphSortType{
	NONE, 
	FRONT_TO_BACK,
	BACK_TO_FRONT,
	TEXTURE
};

class RenderBatch 
{
	public:
		RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) : 
			offset(Offset),
			numVertices(NumVertices),
			texture(Texture) {}
		GLuint offset;
		GLuint numVertices;
		GLuint texture;	
};

struct Glyph 
{
	GLuint texture;
	float depth;

	Vertex topLeft;
	Vertex bottomLeft;
	Vertex topRight;
	Vertex bottomRight;
};


class SpriteBatch
{
	public:
		SpriteBatch();
		~SpriteBatch();

		void init();

		void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
		void end();

		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, const Colour& colour, float depth);
		void renderBatch();

	private:
		void createRenderBatches();
		void createVertexArray();
		void sortGlyphs();

		static bool compareFrontToBack(Glyph* a, Glyph*b);
		static bool compareBackToFront(Glyph* a, Glyph*b);
		static bool compareTexture(Glyph* a, Glyph*b);

		GlyphSortType m_sortType;
		GLuint m_vbo;
		GLuint m_vao;
		std::vector<Glyph *> m_glyphs;
		std::vector<RenderBatch> m_renderBatches;
};



