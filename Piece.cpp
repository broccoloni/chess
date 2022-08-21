#include "Piece.h"
#include <ResourceManager.h>
#include <iostream>

Piece::Piece(int boardStart, int squareSize, glm::vec2 boardPos, int pieceColour, int pieceType, std::string texturePath){
	m_beingHeld = false;
	m_clickedOn = false;
    m_justMoved = false;
    m_boardStart = boardStart;
	m_squareSize = squareSize;
	m_pieceColour = pieceColour;
	m_pieceType = pieceType;
	m_timesMoved = 0;
	m_boardPos = boardPos;
	m_pos = tileToPos(boardPos);
	m_texture = ResourceManager::getTexture(texturePath.c_str());
}

Piece::~Piece(){

}

void Piece::draw(SpriteBatch& spriteBatch, int turnColour){
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	Colour colour;
	colour.r = 255;
	colour.g = 255;
	colour.b = 255;
	colour.a = 255;
	glm::vec4 posAndSize;

	if (turnColour == 0){
		posAndSize = glm::vec4(m_pos.x, m_pos.y, m_squareSize, m_squareSize);
	}
	else {
		glm::vec2 pos = tileToPos(glm::vec2(7,7) - m_boardPos);
		posAndSize = glm::vec4(pos.x, pos.y, m_squareSize, m_squareSize);
	}
	spriteBatch.draw(posAndSize, uv, m_texture.id, colour, 2.0f);
}

glm::vec2 Piece::tileToPos(glm::vec2 tile){
        return glm::vec2(m_boardStart+tile.x*m_squareSize, m_boardStart+tile.y*m_squareSize);
}

void Piece::move(int x, int y){
    m_justMoved = true;
    m_timesMoved += 1;
    setBoardPos(x,y);
    setPos(tileToPos(m_boardPos));
    clickOff();
}

bool Piece::isAMove(int x, int y){
	for (unsigned int i = 0; i < m_moves.size(); i++){
		if (m_moves[i].x == x && m_moves[i].y == y){
			return true;
		}
	}
	return false;
}

bool Piece::isAMove(glm::vec2 pos){
	return isAMove((int)pos.x, (int)pos.y);
}


