#pragma once
#include <glm/glm.hpp>
#include <SpriteBatch.h>
#include <GLTexture.h>
#include "Piece.h"
#include <vector>

class Board
{
	public:
		Board();
		~Board();

		void init(int boardStart, int squareSize, bool verbose);
		void draw(SpriteBatch& spriteBatch, int turnColour);
		glm::vec2 getTile(glm::vec2 mouseCoords, int turnColour);
		Piece* isOccupied(glm::vec2 tile);
		Piece* isOccupied(int x, int y);
		void updateBoard();
        void resetAttackers();
        void calculateMoves(Piece* piece, int attackers[2][8][8], bool setPieceMoves = true);
		void calculateAllMoves();
        bool doesBlockCheck(Piece* king, Piece* piece, glm::vec2 move);
        void pruneMovesForCheck(Piece* king);
        int isAttacked(int colour, int x, int y);
        int isAttacked(int colour, glm::vec2 pos);
		glm::vec2 tileToPos(glm::vec2 tile);
		void setPiece(int x, int y, Piece* piece) { m_pieces[x][y] = piece; }
		void setPiece(glm::vec2 pos, Piece* piece) { m_pieces[(int)pos.x][(int)pos.y] = piece; }
		Piece* piece(int x, int y) { return m_pieces[x][y]; }
		Piece* piece(glm::vec2 pos) { return m_pieces[(int)pos.x][(int)pos.y]; }
		void movePiece(Piece* piece, glm::vec2 boardPos, int turnColour);
		void movePiece(Piece* piece, int x, int y, int turnColour);
		void tempMovePiece(Piece* piece, int x, int y);
		void tempMovePiece(Piece* piece, glm::vec2 pos);
		void resetPiece(Piece* piece);
        void resetJustMoved(int turnColour);
        bool isOnBoard(glm::vec2 pos);
		bool isOnBoard(int x, int y);
		int isKingInCheck(Piece* king);

	private:
		int m_boardStart;
		int m_squareSize;
		GLTexture m_lightSquare;
		GLTexture m_darkSquare;
		GLTexture m_moveTexture;
		Piece* m_pieces[8][8];
        int m_attackers[2][8][8]; //each player x 8 x 8 board
        std::vector<Piece *> m_takenPieces;
		bool m_verbose;		
};
