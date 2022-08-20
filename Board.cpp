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

void Board::calculateMoves(Piece* piece){
    std::vector<glm::vec2> moves;
	glm::vec2 pos = piece -> boardPos();
	glm::vec2 move;
    int otherColour=1;
    int direction = 1;
    if (piece->colour() == 1) {
        otherColour = 0;
        direction = -1;
    }

	switch (piece->type()){
        case 0: //pawns
		{

            //first move
			move = pos+glm::vec2(0,2*direction);   
			if (piece->timesMoved() == 0 && isOnBoard(move) && isOccupied(move)==nullptr){
                moves.push_back(move);
            }	
			//regular move
			move = pos+glm::vec2(0,direction);   
			if (isOnBoard(move) && isOccupied(move)==nullptr){
                moves.push_back(move);
			}	 
			//taking on the right
			move = pos+glm::vec2(1,direction);   
			if (isOnBoard(move) && isOccupied(move) != nullptr){
                if (isOccupied(move)->colour() == otherColour) moves.push_back(move);
	 		}
			//taking on the left
			move = pos+glm::vec2(-1,direction);   
			if (isOnBoard(move)&& isOccupied(move) != nullptr){
                if (isOccupied(move)->colour() == otherColour) moves.push_back(move);
			}
		    
            //en passant on the left
            int fifthrank = 4 - piece->colour();
            Piece* sidepawn = isOccupied(pos+glm::vec2(-1,0));
            move = pos+glm::vec2(-1,direction);
            if (sidepawn != nullptr){
                if (sidepawn->type() == 0 && sidepawn->timesMoved() == 1 
                       && (int)sidepawn->boardPos().y == fifthrank && isOccupied(move) == nullptr){  
                    moves.push_back(move);
                }
            }
		    //en passant on the right
            sidepawn = isOccupied(pos+glm::vec2(1,0));
            move = pos+glm::vec2(1,direction);
            if (sidepawn != nullptr){
                if (sidepawn->type() == 0 && sidepawn->timesMoved() == 1 
                       && (int)sidepawn->boardPos().y == fifthrank && isOccupied(move) == nullptr){  
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
					else if (isOnBoard(move) && isOccupied(move)->colour() == otherColour){
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
					else if (isOnBoard(move) && isOccupied(move)->colour() == otherColour){
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
					else if (isOnBoard(move) && isOccupied(move)->colour() == otherColour){
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
					else if (isOnBoard(move) && isOccupied(move)->colour() == otherColour){
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
                                if (isOccupied(move) == nullptr) moves.push_back(move);
                                else if (isOccupied(move)->colour() == otherColour) moves.push_back(move);
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
					else if (isOnBoard(move) && isOccupied(move)->colour() == otherColour){
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
					else if (isOnBoard(move) && isOccupied(move)->colour() == otherColour){
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
					else if (isOnBoard(move) && isOccupied(move)->colour() == otherColour){
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
					else if (isOnBoard(move) && isOccupied(move)->colour() == otherColour){
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
					else if (isOnBoard(move) && isOccupied(move)->colour() == otherColour){
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
					else if (isOnBoard(move) && isOccupied(move)->colour() == otherColour){
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
					else if (isOnBoard(move) && isOccupied(move)->colour() == otherColour){
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
					else if (isOnBoard(move) && isOccupied(move)->colour() == otherColour){
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
					else if (isOnBoard(move) && isOccupied(move)->colour() == otherColour){
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
					else if (isOnBoard(move) && isOccupied(move)->colour() == otherColour){
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
					else if (isOnBoard(move) && isOccupied(move)->colour() == otherColour){
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
					else if (isOnBoard(move) && isOccupied(move)->colour() == otherColour){
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
		case 5: //kings NEED TO CHECK IF WHEN KING TAKES PIECE ITS DEFENDED AND IF ATTACKING PIECES BLOCK CASTLING
        //ALSO CHECK IF KING IS IN CHECK AND CALCULATE MOVES APPROPRIATELY
        {
            //Regular Moves
			for (int i = -1; i < 2; i++) {
				for (int j = -1; j < 2; j++){
					if (!(i == 0 && j == 0)){
						move = pos + glm::vec2(i,j);
						if (isOnBoard(move)){
                            if (isOccupied(move) == nullptr) moves.push_back(move);
                            else if (isOccupied(move)->colour() == otherColour) moves.push_back(move);
						}
					}	
				}
			}
            //Castling
			if (piece->timesMoved() == 0){
				bool blocked;
                Piece* rook;
				//castling left
				blocked = false;
				//check if there is a piece in the corner
				rook = m_pieces[0][(int)pos.y];
				if (rook != nullptr){
					//if its a rook that hasn't been moved
					if (rook->type() == 1 && rook->timesMoved() == 0){
                        //Check if pieces are between rook and king
						for (int i = pos.x-1; i > 0; i--){
							if (isOccupied(i,(int)pos.y) != nullptr) blocked = true;
						}
						if (blocked == false){
							moves.push_back(pos + glm::vec2(-2,0));
						}
					}
				}
				//castling right
				blocked = false;
				//check if there is a piece in the corner
				rook = m_pieces[7][(int)pos.y];
				if (rook != nullptr){
					//if its a rook that hasn't been moved
					if (rook->type() == 1 && rook->timesMoved() == 0){
						for (int i = pos.x+1; i < 7; i++){
							if (isOccupied(i,(int)pos.y) != nullptr) blocked = true;
						}
						if (blocked == false){
							moves.push_back(pos + glm::vec2(2,0));
						}
					}
				}
			}
            break;
		}
	}
    //std::cout<<"Calculating moves for piece type "<<piece->type()<<" at "<<(int) pos[0]<<", "<<(int) pos[1]<<std::endl;
    //std::cout<<"All moves:"<<std::endl;
    //for (unsigned int i = 0; i < moves.size();i++){
    //    std::cout<<"Move "<<i<<": "<< moves[i][0]<<", "<<moves[i][1]<<std::endl; 
    //}
    //std::cout<<std::endl;
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

void Board::resetPiece(Piece* piece){
	piece->setPos(piece->tileToPos(piece->boardPos()));
}

bool Board::isOnBoard(int x, int y){
	if (x < 0 || x > 7 || y < 0 || y > 7) return false;
	else return true;
}

bool Board::isOnBoard(glm::vec2 pos){
	return isOnBoard(pos.x, pos.y);
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

