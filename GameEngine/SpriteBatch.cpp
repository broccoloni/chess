#include "SpriteBatch.h"
#include <algorithm>

SpriteBatch::SpriteBatch() :
m_vbo(0),
m_vao(0)
{

}

SpriteBatch::~SpriteBatch(){

}

void SpriteBatch::init(){
	createVertexArray();
}

void SpriteBatch::begin(GlyphSortType sortType /*GlyphSortType::TEXTURE*/){
	m_sortType = sortType;
	m_renderBatches.clear();
	for (unsigned int i = 0; i < m_glyphs.size(); i++){
		delete m_glyphs[i];
	}
	m_glyphs.clear();
}

void SpriteBatch::end(){
	sortGlyphs();
	createRenderBatches();
}

void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, const Colour& colour, float depth){
	Glyph* newGlyph = new Glyph;

	newGlyph->texture = texture;
	newGlyph->depth = depth;

	newGlyph->topLeft.colour = colour;
	newGlyph->topLeft.setPosition( destRect.x, destRect.y + destRect.w);
	newGlyph->topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

	newGlyph->bottomLeft.colour = colour;
	newGlyph->bottomLeft.setPosition( destRect.x, destRect.y);
	newGlyph->bottomLeft.setUV(uvRect.x, uvRect.y);
	
	newGlyph->topRight.colour = colour;
	newGlyph->topRight.setPosition( destRect.x + destRect.z, destRect.y + destRect.w);
	newGlyph->topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
	
	newGlyph->bottomRight.colour = colour;
	newGlyph->bottomRight.setPosition( destRect.x + destRect.z, destRect.y);
	newGlyph->bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

	m_glyphs.push_back(newGlyph);
}

void SpriteBatch::renderBatch(){
	glBindVertexArray(m_vao);

	for (unsigned int i = 0; i < m_renderBatches.size(); i++){
		glBindTexture(GL_TEXTURE_2D, m_renderBatches[i].texture);
		glDrawArrays(GL_TRIANGLES, m_renderBatches[i].offset, m_renderBatches[i].numVertices);
	}

	glBindVertexArray(0);
}

void SpriteBatch::createRenderBatches(){
	std::vector <Vertex> vertices;
	vertices.resize(m_glyphs.size() * 6);

	if (m_glyphs.empty()){
		return;
	}
	int offset = 0;
	int cv = 0; //current vertex
	m_renderBatches.emplace_back(offset, 6, m_glyphs[0]->texture);
	vertices[cv++] = m_glyphs[0]->topLeft;
	vertices[cv++] = m_glyphs[0]->bottomLeft;
	vertices[cv++] = m_glyphs[0]->bottomRight;
	vertices[cv++] = m_glyphs[0]->bottomRight;
	vertices[cv++] = m_glyphs[0]->topRight;
	vertices[cv++] = m_glyphs[0]->topLeft;
	offset += 6;	

	for (unsigned int cg = 1; cg < m_glyphs.size(); cg++){ //cg = current glyph
		if (m_glyphs[cg] -> texture != m_glyphs[cg - 1] -> texture){
			m_renderBatches.emplace_back(offset, 6, m_glyphs[cg]->texture);
		}
		else {
			m_renderBatches.back().numVertices += 6;
		}
		vertices[cv++] = m_glyphs[cg]->topLeft;
		vertices[cv++] = m_glyphs[cg]->bottomLeft;
		vertices[cv++] = m_glyphs[cg]->bottomRight;
		vertices[cv++] = m_glyphs[cg]->bottomRight;
		vertices[cv++] = m_glyphs[cg]->topRight;
		vertices[cv++] = m_glyphs[cg]->topLeft;
		offset += 6;
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	//orphan the buffer
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
	//upload the data
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void SpriteBatch::createVertexArray(){
	if (m_vao == 0){
		glGenVertexArrays(1, &m_vao);
	}
	glBindVertexArray(m_vao);
	if (m_vbo == 0){
		glGenBuffers(1, &m_vbo);
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        //This is the position attribute pointer
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        //This is the colour attribute pointer
        glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, colour));
        //This is the UV attribute pointer
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glBindVertexArray(0);

}

void SpriteBatch::sortGlyphs(){
	switch (m_sortType){
		case GlyphSortType::NONE:
			break;

		case GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(m_glyphs.begin(), m_glyphs.end(), compareBackToFront);
			break;
		
		case GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(m_glyphs.begin(), m_glyphs.end(), compareFrontToBack);
			break;

		case GlyphSortType::TEXTURE:
			std::stable_sort(m_glyphs.begin(), m_glyphs.end(), compareTexture);
			break;
	}
}

bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b){
	return (a-> depth < b->depth);
}

bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b){
	return (a-> depth > b->depth);
}

bool SpriteBatch::compareTexture(Glyph* a, Glyph* b){
	return (a->texture < b->texture);
}



