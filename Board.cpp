#include "Board.h"
#include "ResourceManager.h"
#include <iostream>

Board::Board(){
}

Board::~Board(){

}

void Board::init(int boardStart, int squareSize, bool verbose){
	m_boardStart = boardStart;
	m_squareSize = squareSize;
	m_lightSquare = ResourceManager::getTexture("textures/lightwood2.png");
	m_darkSquare = ResourceManager::getTexture("textures/darkwood.png");
	m_moveTexture = ResourceManager::getTexture("textures/dot.png");
	m_verbose = verbose;
}


void Board::draw(SpriteBatch& spriteBatch, int turnColour){
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	Colour colour;
	colour.r = 255;
	colour.g = 255;
	colour.b = 255;
	colour.a = 255;
	
	int lnum, dnum;
	glm::vec4 posAndSize;
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j += 2){
			if (i % 2 == 0){
				lnum = j+1;
				dnum = j;
			}
			else {
				lnum = j;
				dnum = j+1;
			}
			posAndSize = glm::vec4(m_boardStart + lnum*m_squareSize, m_boardStart + i*m_squareSize, m_squareSize, m_squareSize);
			spriteBatch.draw(posAndSize, uv, m_lightSquare.id, colour, 1.0f);
			posAndSize = glm::vec4(m_boardStart + dnum*m_squareSize, m_boardStart + i*m_squareSize, m_squareSize, m_squareSize);
			spriteBatch.draw(posAndSize, uv, m_darkSquare.id, colour, 1.0f);
		}	
	}
	Piece* piece;
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			if (m_pieces[i][j] != nullptr){
				piece = m_pieces[i][j];
				piece -> draw(spriteBatch, turnColour);
				if (piece -> isClickedOn()){
					int size = 30;
					int xloc, yloc;
					colour.a = 180;
					for (unsigned int k = 0; k < piece -> moves().size(); k++){
						if (turnColour == 0){
							xloc = (m_squareSize - size)/2 + m_boardStart + piece->moves()[k].x * m_squareSize; 
							yloc = (m_squareSize - size)/2 + m_boardStart + piece->moves()[k].y * m_squareSize;
						}
						else {
							xloc = (m_squareSize - size)/2 + m_boardStart + (7 - piece->moves()[k].x) * m_squareSize; 
							yloc = (m_squareSize - size)/2 + m_boardStart + (7 - piece->moves()[k].y) * m_squareSize;
						}
						posAndSize = glm::vec4(xloc, yloc, size, size);
						spriteBatch.draw(posAndSize, uv, m_moveTexture.id, colour, 3.0f);
					}
				}
			}
		}
	}
}

glm::vec2 Board::getTile(glm::vec2 mouseCoords, int turnColour){
	if ((abs(mouseCoords.x) > 4*m_squareSize) || (abs(mouseCoords.y) > 4*m_squareSize)){
		//std::cout <<"\noffboard\n"<< std::endl;
		return glm::vec2(-1,-1);
	}
	glm::vec2 tile;
	if (mouseCoords.x < 0) {
		tile.x = 3 + (int)(mouseCoords.x/m_squareSize);
	}
	else {
		tile.x = 4 + (int)(mouseCoords.x/m_squareSize);
	}

	if (mouseCoords.y < 0) {
		tile.y = 3 + (int)(mouseCoords.y/m_squareSize);
	}
	else {
		tile.y = 4 + (int)(mouseCoords.y/m_squareSize);
	}
	//std::cout<< tile.x <<" "<< tile.y<<std::endl;
	if (turnColour == 0) return tile;
	else return glm::vec2(7,7) - tile;
}

glm::vec2 Board::tileToPos(glm::vec2 tile){
	return glm::vec2(m_boardStart+tile.x*m_squareSize, m_boardStart+tile.y*m_squareSize);
}

Piece* Board::isOccupied(glm::vec2 tile){
	if (tile.x < 0 || tile.y < 0 || tile.x > 7 || tile.y > 7) return nullptr;
	else return m_pieces[(int)tile.x][(int)tile.y];
}

Piece* Board::isOccupied(int x, int y){
	if (x < 0 || y < 0 || x > 7 || y > 7) return nullptr;
	else return m_pieces[x][y];
}

void Board::calculateMoves(Piece* piece, int turnColour){
	std::vector<glm::vec2> moves;
	glm::vec2 pos = piece -> boardPos();
	glm::vec2 move;
	Piece* otherPiece;
	if (turnColour != -1){

	}
	else
	{
	switch (piece->type()){
                case 0: //pawns
		{
        		int otherColour=1;
			int direction = 1;
			int fifthRow = 4;
        		if (piece->colour() == 1) {
				otherColour -= 1;
				direction = -1;
				fifthRow = 3;
			}
			//first move
			move = pos+glm::vec2(0,2*direction);   
			if (piece->timesMoved() == 0 && isOnBoard(move)){
	                                moves.push_back(move);
       	                }
	
			//regular move
			move = pos+glm::vec2(0,direction);   
			if (isOnBoard(move)){
                        	moves.push_back(move);
			}	 
			//taking on the right
			move = pos+glm::vec2(1,direction);   
			if (isOnBoard(move)){
				moves.push_back(move);
	 		}
			//taking on the left
			move = pos+glm::vec2(-1,direction);   
			if (isOnBoard(move)){
                	       	moves.push_back(move);
			}
			//en passent on the right
			move = pos+glm::vec2(1,direction);   
			if ((isOccupied(move)==nullptr) && (isOccupied(pos+glm::vec2(1,0))!= nullptr) && isOnBoard(move)){
				otherPiece = m_pieces[(int)pos.x+1][(int)pos.y];
				if (otherPiece -> type() == 0 && otherPiece -> colour() == otherColour && otherPiece -> boardPos().y == fifthRow && otherPiece -> timesMoved() == 1){
					moves.push_back(move);
				}
			}
	
			//en passent on the left
			move = pos+glm::vec2(-1,direction);   
			if ((isOccupied(move)==nullptr) && (isOccupied(pos+glm::vec2(-1,0))!= nullptr) && isOnBoard(move)){
				otherPiece = m_pieces[(int)pos.x-1][(int)pos.y];
				if (otherPiece -> type() == 0 && otherPiece -> colour() == otherColour && otherPiece -> boardPos().y == fifthRow && otherPiece -> timesMoved() == 1){
					moves.push_back(move);
				}
			}
			break;
		}
		case 1: //rooks
		{
			bool blocked;
			blocked = false;
			for (int i = 1 ; i < 8; i++){
				move = pos+glm::vec2(0,i);
				if (!blocked){
					if (isOccupied(move) == nullptr && isOnBoard(move)) moves.push_back(move);
					else if (isOnBoard(move)){
						moves.push_back(move);	
						blocked = true;
					}
					else {
						blocked = true;
					}
				}
			}
			blocked = false;
			for (int i = 1 ; i < 8; i++){
				move = pos+glm::vec2(0,-i);
				if (!blocked){
					if (isOccupied(move) == nullptr && isOnBoard(move)) moves.push_back(move);
					else if (isOnBoard(move)){
						moves.push_back(move);	
						blocked = true;
					}
					else {
						blocked = true;
					}
				}
			}
			blocked = false;
			for (int i = 1 ; i < 8; i++){
				move = pos+glm::vec2(i,0);
				if (!blocked){
					if (isOccupied(move) == nullptr && isOnBoard(move)) moves.push_back(move);
					else if (isOnBoard(move)){
						moves.push_back(move);	
						blocked = true;
					}
					else {
						blocked = true;
					}
				}
			}
			blocked = false;
			for (int i = 1 ; i < 8; i++){
				move = pos+glm::vec2(-i,0);
				if (!blocked){
					if (isOccupied(move) == nullptr && isOnBoard(move)) moves.push_back(move);
					else if (isOnBoard(move)){
						moves.push_back(move);	
						blocked = true;
					}
					else {
						blocked = true;
					}
				}
			}
			break;
		}
		case 2: //knights
		{
			for (int i = -2; i < 3; i++){
				for (int j = -2; j < 3; j++){
					if (!(abs(i) == abs(j) && abs(i) == 2)){
						if ((i != 0 && j != 0) && (abs(i) == 2 || abs(j) == 2)){
							move = pos + glm::vec2(i,j);
							if (isOnBoard(move)){
								moves.push_back(move);
							}
						}
					}
				}
			}

			break;
		}
		case 3: //bishops
		{
			bool blocked;
			blocked = false;
			for (int i = 1 ; i < 8; i++){
				move = pos+glm::vec2(i,i);
				if (!blocked){
					if (isOccupied(move) == nullptr && isOnBoard(move)) moves.push_back(move);
					else if (isOnBoard(move)){
						moves.push_back(move);	
						blocked = true;
					}
					else {
						blocked = true;
					}
				
				}
			}
			blocked = false;
			for (int i = 1 ; i < 8; i++){
				move = pos+glm::vec2(i,-i);
				if (!blocked){
					if (isOccupied(move) == nullptr && isOnBoard(move)) moves.push_back(move);
					else if (isOnBoard(move)){
						moves.push_back(move);	
						blocked = true;
					}
					else {
						blocked = true;
					}
				
				}
			}
			blocked = false;
			for (int i = 1 ; i < 8; i++){
				move = pos+glm::vec2(-i,i);
				if (!blocked){
					if (isOccupied(move) == nullptr && isOnBoard(move)) moves.push_back(move);
					else if (isOnBoard(move)){
						moves.push_back(move);	
						blocked = true;
					}
					else {
						blocked = true;
					}
				
				}
			}
			blocked = false;
			for (int i = 1 ; i < 8; i++){
				move = pos+glm::vec2(-i,-i);
				if (!blocked){
					if (isOccupied(move) == nullptr && isOnBoard(move)) moves.push_back(move);
					else if (isOnBoard(move)){
						moves.push_back(move);	
						blocked = true;
					}
					else {
						blocked = true;
					}
				
				}
			}
			break;
		}
		case 4: //queens
		{
			bool blocked;
			blocked = false;
			for (int i = 1 ; i < 8; i++){
				move = pos+glm::vec2(0,i);
				if (!blocked){
					if (isOccupied(move) == nullptr && isOnBoard(move)) moves.push_back(move);
					else if (isOnBoard(move)){
						moves.push_back(move);	
						blocked = true;
					}
					else {
						blocked = true;
					}
				}
			}
			blocked = false;
			for (int i = 1 ; i < 8; i++){
				move = pos+glm::vec2(0,-i);
				if (!blocked){
					if (isOccupied(move) == nullptr && isOnBoard(move)) moves.push_back(move);
					else if (isOnBoard(move)){
						moves.push_back(move);	
						blocked = true;
					}
					else {
						blocked = true;
					}
				}
			}
			blocked = false;
			for (int i = 1 ; i < 8; i++){
				move = pos+glm::vec2(i,0);
				if (!blocked){
					if (isOccupied(move) == nullptr && isOnBoard(move)) moves.push_back(move);
					else if (isOnBoard(move)){
						moves.push_back(move);	
						blocked = true;
					}
					else {
						blocked = true;
					}
				}
			}
			blocked = false;
			for (int i = 1 ; i < 8; i++){
				move = pos+glm::vec2(-i,0);
				if (!blocked){
					if (isOccupied(move) == nullptr && isOnBoard(move)) moves.push_back(move);
					else if (isOnBoard(move)){
						moves.push_back(move);	
						blocked = true;
					}
					else {
						blocked = true;
					}
				}
			}
			blocked = false;
			for (int i = 1 ; i < 8; i++){
				move = pos+glm::vec2(i,i);
				if (!blocked){
					if (isOccupied(move) == nullptr && isOnBoard(move)) moves.push_back(move);
					else if (isOnBoard(move)){
						moves.push_back(move);	
						blocked = true;
					}
					else {
						blocked = true;
					}
				
				}
			}
			blocked = false;
			for (int i = 1 ; i < 8; i++){
				move = pos+glm::vec2(i,-i);
				if (!blocked){
					if (isOccupied(move) == nullptr && isOnBoard(move)) moves.push_back(move);
					else if (isOnBoard(move)){
						moves.push_back(move);	
						blocked = true;
					}
					else {
						blocked = true;
					}
				
				}
			}
			blocked = false;
			for (int i = 1 ; i < 8; i++){
				move = pos+glm::vec2(-i,i);
				if (!blocked){
					if (isOccupied(move) == nullptr && isOnBoard(move)) moves.push_back(move);
					else if (isOnBoard(move)){
						moves.push_back(move);	
						blocked = true;
					}
					else {
						blocked = true;
					}
				
				}
			}
			blocked = false;
			for (int i = 1 ; i < 8; i++){
				move = pos+glm::vec2(-i,-i);
				if (!blocked){
					if (isOccupied(move) == nullptr && isOnBoard(move)) moves.push_back(move);
					else if (isOnBoard(move)){
						moves.push_back(move);	
						blocked = true;
					}
					else {
						blocked = true;
					}
				
				}
			}
			break;
		}
		case 5: //kings
		{
			for (int i = -1; i < 2; i++) {
				for (int j = -1; j < 2; j++){
					if (!(i == 0 && j == 0)){
						move = pos + glm::vec2(i,j);
						if (isOnBoard(move)){
							moves.push_back(move);
						}
					}	
				}
			}
			if (piece->timesMoved() == 0){
				bool blocked;
				//castling left
				blocked = false;
				//check if there is a piece in the corner
				otherPiece = m_pieces[0][(int)piece->boardPos().y];
				if (otherPiece != nullptr){
					//if its a rook that hasn't been moved
					if (otherPiece->type() == 1 && otherPiece->timesMoved() == 0){
						for (int i = piece->boardPos().x-1; i > 0; i--){
							if (m_pieces[i][(int)piece->boardPos().y] != nullptr) blocked = true;
						}
						if (blocked == false){
							moves.push_back(piece->boardPos() + glm::vec2(-2,0));
						}
					}
				}
				//castling right
				blocked = false;
				//check if there is a piece in the corner
				otherPiece = m_pieces[7][(int)piece->boardPos().y];
				if (otherPiece != nullptr){
					//if its a rook that hasn't been moved
					if (otherPiece->type() == 1 && otherPiece->timesMoved() == 0){
						for (int i = piece->boardPos().x+1; i < 7; i++){
							if (m_pieces[i][(int)piece->boardPos().y] != nullptr) blocked = true;
						}
						if (blocked == false){
							moves.push_back(piece->boardPos() + glm::vec2(2,0));
						}
					}
				}
			}

			break;
		}
	}
	}
	piece -> setMoves(moves);
}

bool Board::isDefended(int x, int y, int colour){
	std::vector<glm::vec2> moves;
	Piece* tile = m_pieces[x][y];
	int otherColour = 1;
	if (colour == 1) {
		otherColour = 0;
	}
	Piece* defender;
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			defender = m_pieces[i][j];
			if (defender != nullptr && !(x == i && y == j)){
				//if possible defender is of the other colour of input piece
				if (defender -> colour() == otherColour){
					//if the other piece is a king
					if (defender -> type() == 5){
						//note can't use isValidMove if other piece
						//is a king since you'd get an infinite loop
						//if tile is empty
						if (tile == nullptr){
							if (defender->isAMove(x,y)){
								return true;
							}
						}
						//if the tile has a piece on it and is opposite colour to the input piece
						else if (defender -> colour() == tile -> colour()){
							if (defender->isAMove(x,y)) {
								return true;
							}
						}
					}
					else if (defender -> type() == 0){
						//make sure the pawn is only defending if it attacks sideways
						if (defender->isAMove(x,y) && defender->boardPos().x != x){
							return true;
						}
					}
					else if (defender -> isAMove(x,y)){
					       	return true;
					}
				}
			}
		}
	}
	return false;
}

void Board::movePiece(Piece* piece, glm::vec2 boardPos, int turnColour){
	movePiece(piece, (int)boardPos.x, (int)boardPos.y, turnColour);
}

void Board::movePiece(Piece* piece, int x, int y, int turnColour){
	if (piece -> type() == 5){	
		//castling left
		if (x == (int)piece->boardPos().x - 2){
			movePiece(m_pieces[0][y], x+1, y, turnColour);
		}
		//castling right
		else if (x == (int)piece->boardPos().x + 2){
			movePiece(m_pieces[7][y], x-1, y, turnColour);
		}
	}
	piece->madeMove();
	setPiece(piece->boardPos(), nullptr);
	piece->setBoardPos(x,y);
	setPiece(x,y, piece);
	piece->setPos(piece->tileToPos(piece->boardPos()));
	piece->clickOff();
	calculateAllMoves();	
	validateAllMoves(turnColour);
}

void Board::tempMovePiece(Piece* piece, int x, int y){
	setPiece(piece->boardPos(), nullptr);
	setPiece(x,y, piece);
}

void Board::tempMovePiece(Piece* piece, glm::vec2 pos){
	tempMovePiece(piece, (int)pos.x, (int)pos.y);
}

void Board::calculateAllMoves(){
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			if (m_pieces[i][j] != nullptr){
				calculateMoves(m_pieces[i][j]);
			}
		}
	}
}

void Board::validateAllMoves(int turnColour){
	if (m_verbose) std::cout <<"***\tNEW VALIDATION\t***"<<std::endl;
	std::vector<glm::vec2> validMoves;
	//Note: must validate king moves first so that other moves
	//such as pawn attacking moves don't get deleted
	int numMoves = 0;
	Piece* king;
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			if (m_pieces[i][j] != nullptr){
				if (m_pieces[i][j] -> type() == 5){
					if (m_pieces[i][j]->colour() == turnColour) king = m_pieces[i][j];
					for (unsigned int k = 0; k < m_pieces[i][j]->moves().size(); k++){
						if (isValidMove(m_pieces[i][j]->moves()[k], m_pieces[i][j], nullptr)){
							validMoves.push_back(m_pieces[i][j]->moves()[k]);
							numMoves += 1;
						}
					}
					m_pieces[i][j]->setMoves(validMoves);
				}
			}
			validMoves.clear();
		}
	}
	//can't re-validate king moves or it'll do the same thing
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			if (m_pieces[i][j] != nullptr){
				if (m_pieces[i][j] -> type() != 5){
					for (unsigned int k = 0; k < m_pieces[i][j]->moves().size(); k++){
						if (isValidMove(m_pieces[i][j]->moves()[k], m_pieces[i][j], king)){
							validMoves.push_back(m_pieces[i][j]->moves()[k]);
							numMoves += 1; 
						}
					}
					m_pieces[i][j]->setMoves(validMoves);
				}
			}
			validMoves.clear();
		}	
	}
	if (m_verbose){
		std::cout<<"***\tALL VALID MOVES\t***"<<std::endl;
		for (int i = 0; i < 8; i++){
			for (int j = 0; j < 8; j++){
				if (m_pieces[i][j] != nullptr){
					for (unsigned int k = 0; k < m_pieces[i][j]->moves().size(); k++){
						std::cout<<"Piece Location: "<<i<<" "<<j<<" Move: "<< m_pieces[i][j]->moves()[k].x<<" "<<m_pieces[i][j]->moves()[k].y<<std::endl;
					}
				}
			}
		}
		std::cout<< "Total Number of Valid Moves: "<<numMoves<<std::endl;
	}
	//return numMoves;
}

void Board::resetPiece(Piece* piece){
	piece->setPos(piece->tileToPos(piece->boardPos()));
}

bool Board::isValidMove(glm::vec2 move, Piece* piece, Piece* king){
	return isValidMove(move.x, move.y, piece, king);
}

bool Board::isOnBoard(int x, int y){
	if (x < 0 || x > 7 || y < 0 || y > 7) return false;
	else return true;
}

bool Board::isOnBoard(glm::vec2 pos){
	return isOnBoard(pos.x, pos.y);
}

bool Board::isValidMove(int x, int y, Piece* piece, Piece* king){
	if (m_verbose) std::cout <<"Piece loc: "<<piece->boardPos().x<<" "<<piece->boardPos().y<<"\ttype: "<<piece->type()<<"\tmove: "<<x<<" "<<y<<" ";
	//check if the move is to one with a piece
	if (isOccupied(x,y)){
		//if current piece is a king
		if (piece -> type() == 5){
			//if the other piece is the other colour
			if (m_pieces[x][y]->colour() != piece->colour()){
				//if its defended
				if (isDefended(x,y, piece -> colour())) {
					if (m_verbose) std::cout <<"\tinvalid: "<< false <<" king can't attack defended piece"<<std::endl;
					return false;
				}
				else {
					if (m_verbose) std::cout <<"\tvalid: "<< true <<" king taking undefended piece"<<std::endl;
				}
			}
			//if the other piece is the same colour
			else {
				if (m_verbose) std::cout <<"\tinvalid: "<< false <<" king can't move to where its own piece is"<<std::endl;
				return false;
			}
		}
		//if current piece is a pawn
		else if (piece -> type() == 0){
			//if the pawn if attacking a piece of the other colour at a different x location
			if (x != piece -> boardPos().x && m_pieces[x][y]->colour() != piece->colour()){
				if (m_verbose) std::cout <<"\tvalid: "<< true <<" pawn taking on an angle"<<std::endl;
			}
			else {
				if (m_verbose) std::cout <<"\tinvalid: "<< false <<" pawn can't take in front of itself"<<std::endl;
				return false;	
			}
		}
		//if its not a pawn or king
		//if its attacking a piece of the other colour
		else if (m_pieces[x][y]->colour() != piece->colour()) {
			if (m_verbose) std::cout <<"\tvalid: "<< true <<" not king or pawn taking a piece"<<std::endl;
		}
		else {
			if (m_verbose) std::cout <<"\tinvalid: "<< false <<" not king or pawn can't take piece of own colour"<<std::endl;
			return false;
		}
	}
	//if move is to a tile without a piece
	else {
		//if its a king
		if (piece -> type() == 5){
			//if to a defended square
			if (isDefended(x,y, piece->colour())) {
				if (m_verbose) std::cout <<"\tinvalid: "<< false <<" king can't move to defended tile"<<std::endl;
				return false;
			}
			else {
				if (m_verbose) std::cout <<"\tvalid: "<< true <<" king moving to empty tile"<<std::endl;
			}
		}
		//if its a pawn
		else if (piece -> type() == 0){
			//if the x location is the same as the pawns
			if (x != piece -> boardPos().x) {
				if (m_verbose) std::cout <<"\tinvalid: "<< false <<" pawn can't move sideways to empty tile"<<std::endl;
				return false;
			}
			else {
				if (m_verbose) std::cout <<"\tvalid: "<< true <<" pawn moving to empty tile in front of it"<<std::endl;
			}
		}
		//if its not a pawn or king
		else {
			if (m_verbose) std::cout <<"\tvalid: "<< true <<" not king or pawn moving to empty tile"<<std::endl;
		}
	}
	//temporarily move piece and check if king is in check
	int kingAttackers;
	if (king != nullptr){
		if (piece->colour() == king->colour()){
			tempMovePiece(piece, x,y);
			kingAttackers = isKingInCheck(king);
			tempMovePiece(piece, piece->boardPos());
			if (kingAttackers > 0) {
				if (m_verbose) std::cout << "\t\t\t\t\tinvalid: "<< false<< " Number of King Attackers if move made: "<<kingAttackers<<std::endl;
				return false;
			}
			else{
				if (m_verbose) std::cout << "\t\t\t\t\tvalid: "<< true<< " Number of King Attackers if move made: "<<kingAttackers<<std::endl;
			}
		}
		else {
			if (m_verbose) std::cout <<"\t\t\t\t\tvalid: "<< true<<" Don't need to check checks for other colour piece"<<std::endl;
		}
	}
	else {
		if (m_verbose) std::cout<<"\t\t\t\t\tvalid: "<<true<<" Don't need to check checks for king moves"<<std::endl;
	}
	return true;
}

int Board::isKingInCheck(Piece* king){
	int numAttackers = 0;
	int otherColour = (king -> colour() + 1) % 2;
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			if (m_pieces[i][j] != nullptr){
				if (m_pieces[i][j]->colour() == otherColour){
					for (unsigned int k = 0; k < m_pieces[i][j]->moves().size(); k++){
						if (m_pieces[i][j] -> moves()[k].x == king->boardPos().x && m_pieces[i][j]->moves()[k].y == king->boardPos().y){
							numAttackers += 1;
						}
					}
				}
			}
		}
	}
	return numAttackers;
}

