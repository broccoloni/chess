#pragma once
#include <glm/glm.hpp>
#include <GLTexture.h>
#include <SpriteBatch.h>
#include <string>
#include <vector>

class Piece
{
	public:
		Piece(int boardStart, int squareSize, glm::vec2 boardPos, int pieceColour, int pieceType, std::string texturePath);	
		~Piece();
			
		int m_timesMoved;

		void draw(SpriteBatch& spriteBatch, int turnColour);

		void setPos(int x, int y) { m_pos = glm::vec2(x,y); }
		void setPos(glm::vec2 pos) { m_pos = pos; }
		void setBoardPos(int x, int y) { m_boardPos = glm::vec2(x,y); }
		void setBoardPos(glm::vec2 boardPos) { m_boardPos = boardPos; }

		glm::vec2 boardPos() { return m_boardPos; }
		glm::vec2 pos() { return m_pos; }

		int type() { return m_pieceType; }
		int colour() { return m_pieceColour; }
		int timesMoved() { return m_timesMoved; }

		glm::vec2 tileToPos(glm::vec2 tile);

		bool isBeingHeld() { return m_beingHeld; }
		bool isClickedOn() { return m_clickedOn; }
        bool wasJustMoved() { return m_justMoved; }        
		void hold() { m_beingHeld = true; }
		void drop() { m_beingHeld = false; }
		void clickOn() { m_clickedOn = true; }
		void clickOff() { m_clickedOn = false; }
        void click() { m_clickedOn = !m_clickedOn; }
        void wasNotJustMoved() { m_justMoved = false; }
        void setMoves(std::vector<glm::vec2> moves) { m_moves = moves; }
		void move(int x, int y);
		std::vector<glm::vec2> moves() { return m_moves; }
		
		bool isAMove(int x, int y);
		bool isAMove(glm::vec2 pos);

	private:
		bool m_beingHeld;
		bool m_clickedOn;
        bool m_justMoved;		
		int m_pieceType;
		int m_pieceColour;
		int m_boardStart;
		int m_squareSize;
		glm::vec2 m_pos;
		glm::vec2 m_boardPos;
		GLTexture m_texture;
		std::vector<glm::vec2> m_moves;		
		
};
