#include "Piece.h"
#include <ResourceManager.h>
#include <iostream>
#include <string>

Piece::Piece(){

}

Piece::Piece(int boardStart, int squareSize, glm::vec2 boardPos, int pieceColour, int pieceType, bool showdisplay){
    m_beingHeld = false;
	m_clickedOn = false;
    m_justMoved = false;
    m_isCaptured = false;
    m_boardStart = boardStart;
	m_squareSize = squareSize;
    m_depth = 2.0f;
	m_pieceColour = pieceColour;
	m_pieceType = pieceType;
	m_timesMoved = 0;
	m_boardPos = boardPos;
	m_pos = tileToPos(boardPos);
    m_origPos = m_pos;
    m_showdisplay = showdisplay;
	if (showdisplay) setTexture();
}

Piece::~Piece(){

}

std::string Piece::texturePath(int pieceType,int pieceColour){
    std::vector<std::string> textures{"textures/Pieces/w_pawn.png",
                                 "textures/Pieces/w_rook.png",
                                 "textures/Pieces/w_knight.png",
                                 "textures/Pieces/w_bishop.png",
                                 "textures/Pieces/w_queen.png",
                                 "textures/Pieces/w_king.png",
                                 "textures/Pieces/b_pawn.png",
                                 "textures/Pieces/b_rook.png",
                                 "textures/Pieces/b_knight.png",
                                 "textures/Pieces/b_bishop.png",
                                 "textures/Pieces/b_queen.png",
                                 "textures/Pieces/b_king.png"};
    return textures[6*pieceColour+pieceType];
}

void Piece::setTexture(){
    m_texture = ResourceManager::getTexture(texturePath(m_pieceType,m_pieceColour).c_str());
}

void Piece::draw(SpriteBatch& spriteBatch, int boardOrientation){
    //CHANGE THIS TO HAVE VARIABLE DEPTH
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	Colour colour;
	colour.r = 255;
	colour.g = 255;
	colour.b = 255;
	colour.a = 255;
	glm::vec4 posAndSize;

    //if piece hasn't been captured - display normally
    if (!m_isCaptured){
        if (boardOrientation == 0){
            posAndSize = glm::vec4(m_pos.x, m_pos.y, m_squareSize, m_squareSize);
        }
        else {
            glm::vec2 pos = tileToPos(glm::vec2(7,7)) - m_pos + glm::vec2(m_boardStart,m_boardStart);
            posAndSize = glm::vec4(pos.x, pos.y, m_squareSize, m_squareSize);
        }
    }
    //if piece is captured - display tiny in it's position on the right
    else{
        int scaleby = 3;
        if (boardOrientation == 0){
            if (m_pieceColour == 0){
                glm::vec2 pos = tileToPos(glm::vec2(9.5,6.3))-glm::vec2(m_origPos.x/scaleby,m_origPos.y/scaleby);
                posAndSize = glm::vec4(pos.x,pos.y,(int)m_squareSize/scaleby,(int)m_squareSize/scaleby);
            }
            else{
                glm::vec2 pos = tileToPos(glm::vec2(9.5,1))-glm::vec2(m_origPos.x/scaleby,m_origPos.y/scaleby);
                posAndSize = glm::vec4(pos.x,pos.y,(int)m_squareSize/scaleby,(int)m_squareSize/scaleby);
            }
            
        }
        else{
            if (m_pieceColour == 0){
                glm::vec2 pos = tileToPos(glm::vec2(9.8,1.3))+glm::vec2(m_origPos.x/scaleby,m_origPos.y/scaleby);
                posAndSize = glm::vec4(pos.x,pos.y,(int)m_squareSize/scaleby,(int)m_squareSize/scaleby);
            }
            else{
                glm::vec2 pos = tileToPos(glm::vec2(9.8,6.6))+glm::vec2(m_origPos.x/scaleby,m_origPos.y/scaleby);
                posAndSize = glm::vec4(pos.x,pos.y,(int)m_squareSize/scaleby,(int)m_squareSize/scaleby);
            }
        }
    }
    spriteBatch.draw(posAndSize, uv, m_texture.id, colour, m_depth);
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

void Piece::promote(int newType){
    m_pieceType = newType;
    if (m_showdisplay) setTexture();
}






