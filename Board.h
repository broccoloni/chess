#pragma once
#include <glm/glm.hpp>
#include <SpriteBatch.h>
#include <GLTexture.h>
#include "Piece.h"
#include "Square.h"
#include <vector>

class Board
{
	public:
		Board();
		~Board();

		void init(int boardStart, int squareSize, bool verbose, bool showdisplay);
		void draw(SpriteBatch& spriteBatch, int boardOrientation);
		glm::vec2 getTile(glm::vec2 mouseCoords, int boardOrientation);
		Piece* isOccupied(glm::vec2 tile);
		Piece* isOccupied(int x, int y);
		void updateBoard();
        void resetAttackers();
        void calculateMoves(Piece* piece, int attackers[2][8][8], bool setPieceMoves = true);
		void calculateAllMoves();
        int calculateNextTurnMoves(int nextTurnColour);
        bool doesBlockCheck(Piece* king, Piece* piece, glm::vec2 move);
        void pruneMovesForChecksAndPins(Piece* king);
        int sumMoves(int turnColour);
        int isAttacked(int attackingColour, int x, int y);
        int isAttacked(int attackingColour, glm::vec2 pos);
		glm::vec2 tileToPos(glm::vec2 tile);
		void setPiece(int x, int y, Piece* piece) { m_pieces[x][y] = piece; }
		void setPiece(glm::vec2 pos, Piece* piece) { m_pieces[(int)pos.x][(int)pos.y] = piece; }
		void movePiece(Piece* piece, glm::vec2 boardPos);
		void movePiece(Piece* piece, int x, int y);
		void tempMovePiece(Piece* piece, int x, int y);
		void tempMovePiece(Piece* piece, glm::vec2 pos);
        void tempUnmovePiece(Piece* piece, int movex, int movey);
        void tempUnmovePiece(Piece* piece, glm::vec2 move);
        void resetPiece(Piece* piece);
        void resetJustMoved(int turnColour);
        void printBoard();
        void printPlayerState();
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
        int m_attackers[2][8][8]; //each player x 8 x 8 board attackers[0][:][:] are locations player 0 is attacking
        std::vector<Piece *> m_takenPieces;
		bool m_verbose;		
        bool m_showdisplay;
};
