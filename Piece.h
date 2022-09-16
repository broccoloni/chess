#pragma once
#include <glm/glm.hpp>
#include <GLTexture.h>
#include <SpriteBatch.h>
#include <string>
#include <vector>

class Piece
{
	public:
        Piece(); // to instantiate and use texturepath function
		Piece(int boardStart, int squareSize, glm::vec2 boardPos, int pieceColour, int pieceType, bool showdisplay);	
		~Piece();
			
		int m_timesMoved;

        std::string texturePath(int pieceType, int pieceColour);
        void setTexture();
        void draw(SpriteBatch& spriteBatch, int boardOrientation);
		void setPos(int x, int y) { m_pos = glm::vec2(x,y); }
		void setPos(glm::vec2 pos) { m_pos = pos; }
		void setBoardPos(int x, int y) { m_boardPos = glm::vec2(x,y); }
		void setBoardPos(glm::vec2 boardPos) { m_boardPos = boardPos; }
		void hold() { m_beingHeld = true; }
		void drop() { m_beingHeld = false; }
		void clickOn() { m_clickedOn = true; }
		void clickOff() { m_clickedOn = false; }
        void click() { m_clickedOn = !m_clickedOn; }
        void wasNotJustMoved() { m_justMoved = false; }
        void setMoves(std::vector<glm::vec2> moves) { m_moves = moves; }
		void move(int x, int y);
        void capture() { m_isCaptured = true; }
        void setDepth(float depth) { m_depth = depth; }
		glm::vec2 boardPos() { return m_boardPos; }
		glm::vec2 pos() { return m_pos; }
		glm::vec2 tileToPos(glm::vec2 tile);
		std::vector<glm::vec2> moves() { return m_moves; }
		int type() { return m_pieceType; }
		int colour() { return m_pieceColour; }
		int timesMoved() { return m_timesMoved; }
		bool isBeingHeld() { return m_beingHeld; }
		bool isClickedOn() { return m_clickedOn; }
        bool wasJustMoved() { return m_justMoved; }        	
		bool isAMove(int x, int y);
		bool isAMove(glm::vec2 pos);
        void promote(int newType);

	private:
		bool m_beingHeld;
		bool m_clickedOn;
        bool m_justMoved;	
        bool m_showdisplay;    
        bool m_isCaptured;
		int m_pieceType;
		int m_pieceColour;
		int m_boardStart;
		int m_squareSize;
        float m_depth;
		glm::vec2 m_pos;
		glm::vec2 m_boardPos;
        glm::vec2 m_origPos;
		GLTexture m_texture;
		std::vector<glm::vec2> m_moves;		
		
};
