#include "Sprite.h"
#include <GL/glew.h>
#include "Vertex.h"
#include <cstddef>
#include "ResourceManager.h"

Sprite::Sprite(){
	m_vbufferID = 0;
}

Sprite::~Sprite(){
	if (m_vbufferID != 0){
		glDeleteBuffers(1, &m_vbufferID);
	}
}

void Sprite::init(float x, float y, float width, float height, std::string texturePath){
	m_x = x;
	m_y = y;
	m_width = width;
	m_height = height;
	
	m_texture = ResourceManager::getTexture(texturePath);

	if (m_vbufferID == 0){
		glGenBuffers(1, &m_vbufferID);
	}	

	Vertex vertData[6];
	vertData[0].setPosition(x + width, y + height);
	vertData[0].setUV(1.0f, 1.0f);
	vertData[1].setPosition(x, y + height);
	vertData[1].setUV(0.0f, 1.0f);
	vertData[2].setPosition(x, y);
	vertData[2].setUV(0.0f, 0.0f);
	vertData[3].setPosition(x, y);
	vertData[3].setUV(0.0f, 0.0f);
	vertData[4].setPosition(x + width, y);
	vertData[4].setUV(1.0f, 0.0f);
	vertData[5].setPosition(x + width, y + height);
	vertData[5].setUV(1.0f, 1.0f);

	for (int i = 0; i < 6; i++){
		vertData[i].setColour(255, 255, 255, 255);
	}
	//vertData[1].setColour(0, 0, 255, 255);
	//vertData[4].setColour(0, 255, 0, 255);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertData), vertData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sprite::draw(){
	glBindTexture(GL_TEXTURE_2D, m_texture.id);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vbufferID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//This is the position attribute pointer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	//This is the colour attribute pointer
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, colour));
	//This is the UV attribute pointer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
