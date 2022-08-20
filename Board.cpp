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
    resetAttackers();
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

void Board::resetAttackers(){
    for (unsigned int player = 0; player <= 1; player++){
        for (unsigned int i = 0; i < 8; i++){
            for (unsigned int j = 0; j < 8; j++){
                m_attackers[player][i][j] = 0;
            }
        }
    }
}

void Board::calculateMoves(Piece* piece, int *** attackers, bool setPieceMoves = true){
    std::vector<glm::vec2> moves;
	glm::vec2 pos = piece -> boardPos();
	glm::vec2 move;
    int colour = piece->colour();
    int otherColour = 1-colour;
    int direction = 1 - 2*colour;

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
                if (isOccupied(move)->colour() == otherColour){
                    moves.push_back(move);
                }
                attackers[colour][(int)move.x][(int)move.y] += 1;
	 		}
            else if (isOnBoard(move)) attackers[colour][(int)move.x][(int)move.y] += 1;
			//taking on the left
			move = pos+glm::vec2(-1,direction);   
			if (isOnBoard(move)&& isOccupied(move) != nullptr){
                if (isOccupied(move)->colour() == otherColour){
                    moves.push_back(move);
                }
                attackers[colour][(int)move.x][(int)move.y] += 1;
            }        
            else if (isOnBoard(move)) attackers[colour][(int)move.x][(int)move.y] += 1;
            //en passant on the left
            int fifthrank = 4 - piece->colour();
            Piece* sidepawn = isOccupied(pos+glm::vec2(-1,0));
            move = pos+glm::vec2(-1,direction);
            if (sidepawn != nullptr){
                if (sidepawn->type() == 0 && sidepawn->timesMoved() == 1 
                       && (int)sidepawn->boardPos().y == fifthrank && isOccupied(move) == nullptr
                       && sidepawn->wasJustMoved()){  
                    moves.push_back(move);
                }
            }
		    //en passant on the right
            sidepawn = isOccupied(pos+glm::vec2(1,0));
            move = pos+glm::vec2(1,direction);
            if (sidepawn != nullptr){
                if (sidepawn->type() == 0 && sidepawn->timesMoved() == 1 
                       && (int)sidepawn->boardPos().y == fifthrank && isOccupied(move) == nullptr
                       && sidepawn->wasJustMoved()){  
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
                    //empty square
					if (isOccupied(move) == nullptr && isOnBoard(move)){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
                        moves.push_back(move);
                    }
                    //occupied by opponent
					else if (isOnBoard(move) && isOccupied(move)->colour() == otherColour){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
						moves.push_back(move);	
						blocked = true;
                        //if blocked by king, continue calculation until blocked by something else
                        if (isOccupied(move)->type() == 5){
                            for (int j = i; j < 8; j++){
                                move = pos + glm::vec2(0,j);
                                if (isOnBoard(move) && isOccupied(move) == nullptr){
                                    attackers[colour][(int)move.x][(int)move.y] += 1;
                                }
                            }
                        }
					}
                    else if (isOnBoard(move) && isOccupied(move)->colour() == colour){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
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
					if (isOccupied(move) == nullptr && isOnBoard(move)) {
                        attackers[colour][(int)move.x][(int)move.y] += 1;
                        moves.push_back(move);
                    }
					else if (isOnBoard(move) && isOccupied(move)->colour() == otherColour){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
						moves.push_back(move);	
						blocked = true;
                        //if blocked by king, continue calculation until blocked by something else
                        if (isOccupied(move)->type() == 5){
                            for (int j = i; j < 8; j++){
                                move = pos + glm::vec2(0,-j);
                                if (isOnBoard(move) && isOccupied(move) == nullptr){
                                    attackers[colour][(int)move.x][(int)move.y] += 1;
                                }
                            }
                        }
					}
                    else if (isOnBoard(move) && isOccupied(move)->colour() == colour){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
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
					if (isOccupied(move) == nullptr && isOnBoard(move)){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
                        moves.push_back(move);
                    }
					else if (isOnBoard(move) && isOccupied(move)->colour() == otherColour){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
						moves.push_back(move);	
						blocked = true;
                        //if blocked by king, continue calculation until blocked by something else
                        if (isOccupied(move)->type() == 5){
                            for (int j = i; j < 8; j++){
                                move = pos + glm::vec2(j,0);
                                if (isOnBoard(move) && isOccupied(move) == nullptr){
                                    attackers[colour][(int)move.x][(int)move.y] += 1;
                                }
                            }
                        }
					}
                    else if (isOnBoard(move) && isOccupied(move)->colour() == colour){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
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
					if (isOccupied(move) == nullptr && isOnBoard(move)){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
                        moves.push_back(move);
                    }
					else if (isOnBoard(move) && isOccupied(move)->colour() == otherColour){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
						moves.push_back(move);	
						blocked = true;
                        //if blocked by king, continue calculation until blocked by something else
                        if (isOccupied(move)->type() == 5){
                            for (int j = i; j < 8; j++){
                                move = pos + glm::vec2(-j,0);
                                if (isOnBoard(move) && isOccupied(move) == nullptr){
                                    attackers[colour][(int)move.x][(int)move.y] += 1;
                                }
                            }
                        }
					}
                    else if (isOnBoard(move) && isOccupied(move)->colour() == colour){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
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
                                if (isOccupied(move) == nullptr) {
                                    attackers[colour][(int)move.x][(int)move.y] += 1;
                                    moves.push_back(move);
                                }
                                else if (isOccupied(move)->colour() == otherColour){
                                    attackers[colour][(int)move.x][(int)move.y] += 1;
                                    moves.push_back(move);
                                }
                                else if (isOccupied(move)->colour() == colour){
                                    attackers[colour][(int)move.x][(int)move.y] += 1;
                                }
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
					if (isOccupied(move) == nullptr && isOnBoard(move)) {
                        attackers[colour][(int)move.x][(int)move.y] += 1;
                        moves.push_back(move);
                    }
                    else if (isOnBoard(move) && isOccupied(move)->colour() == otherColour){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
						moves.push_back(move);	
						blocked = true;
                        //if blocked by king, continue calculation until blocked by something else
                        if (isOccupied(move)->type() == 5){
                            for (int j = i; j < 8; j++){
                                move = pos + glm::vec2(j,j);
                                if (isOnBoard(move) && isOccupied(move) == nullptr){
                                    attackers[colour][(int)move.x][(int)move.y] += 1;
                                }
                            }
                        }
					}
                    else if (isOnBoard(move) && isOccupied(move)->colour() == colour){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
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
					if (isOccupied(move) == nullptr && isOnBoard(move)) {
                        attackers[colour][(int)move.x][(int)move.y] += 1;
                        moves.push_back(move);
                    }
					else if (isOnBoard(move) && isOccupied(move)->colour() == otherColour){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
						moves.push_back(move);	
						blocked = true;
                        //if blocked by king, continue calculation until blocked by something else
                        if (isOccupied(move)->type() == 5){
                            for (int j = i; j < 8; j++){
                                move = pos + glm::vec2(j,-j);
                                if (isOnBoard(move) && isOccupied(move) == nullptr){
                                    attackers[colour][(int)move.x][(int)move.y] += 1;
                                }
                            }
                        }
					}
                    else if (isOnBoard(move) && isOccupied(move)->colour() == colour){ 
                        attackers[colour][(int)move.x][(int)move.y] += 1;
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
					if (isOccupied(move) == nullptr && isOnBoard(move)){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
                        moves.push_back(move);
                    }
					else if (isOnBoard(move) && isOccupied(move)->colour() == otherColour){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
						moves.push_back(move);	
						blocked = true;
                        //if blocked by king, continue calculation until blocked by something else
                        if (isOccupied(move)->type() == 5){
                            for (int j = i; j < 8; j++){
                                move = pos + glm::vec2(-j,j);
                                if (isOnBoard(move) && isOccupied(move) == nullptr){
                                    attackers[colour][(int)move.x][(int)move.y] += 1;
                                }
                            }
                        }
					}
                    else if (isOnBoard(move) && isOccupied(move)->colour() == colour){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
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
					if (isOccupied(move) == nullptr && isOnBoard(move)){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
                        moves.push_back(move);
                    }
					else if (isOnBoard(move) && isOccupied(move)->colour() == otherColour){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
						moves.push_back(move);	
						blocked = true;
                        //if blocked by king, continue calculation until blocked by something else
                        if (isOccupied(move)->type() == 5){
                            for (int j = i; j < 8; j++){
                                move = pos + glm::vec2(-j,-j);
                                if (isOnBoard(move) && isOccupied(move) == nullptr){
                                    attackers[colour][(int)move.x][(int)move.y] += 1;
                                }
                            }
                        }
					}
                    else if (isOnBoard(move) && isOccupied(move)->colour() == colour){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
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
					if (isOccupied(move) == nullptr && isOnBoard(move)){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
                        moves.push_back(move);
                    }
					else if (isOnBoard(move) && isOccupied(move)->colour() == otherColour){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
						moves.push_back(move);	
						blocked = true;
                        //if blocked by king, continue calculation until blocked by something else
                        if (isOccupied(move)->type() == 5){
                            for (int j = i; j < 8; j++){
                                move = pos + glm::vec2(0,j);
                                if (isOnBoard(move) && isOccupied(move) == nullptr){
                                    attackers[colour][(int)move.x][(int)move.y] += 1;
                                }
                            }
                        }
					}
                    else if (isOnBoard(move) && isOccupied(move)->colour() == colour){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
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
					if (isOccupied(move) == nullptr && isOnBoard(move)){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
                        moves.push_back(move);
                    }
					else if (isOnBoard(move) && isOccupied(move)->colour() == otherColour){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
						moves.push_back(move);	
						blocked = true;
                        //if blocked by king, continue calculation until blocked by something else
                        if (isOccupied(move)->type() == 5){
                            for (int j = i; j < 8; j++){
                                move = pos + glm::vec2(0,-j);
                                if (isOnBoard(move) && isOccupied(move) == nullptr){
                                    attackers[colour][(int)move.x][(int)move.y] += 1;
                                }
                            }
                        }
					}
                    else if (isOnBoard(move) && isOccupied(move)->colour() == colour){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
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
					if (isOccupied(move) == nullptr && isOnBoard(move)){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
                        moves.push_back(move);
                    }
					else if (isOnBoard(move) && isOccupied(move)->colour() == otherColour){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
						moves.push_back(move);	
						blocked = true;
                        //if blocked by king, continue calculation until blocked by something else
                        if (isOccupied(move)->type() == 5){
                            for (int j = i; j < 8; j++){
                                move = pos + glm::vec2(j,0);
                                if (isOnBoard(move) && isOccupied(move) == nullptr){
                                    attackers[colour][(int)move.x][(int)move.y] += 1;
                                }
                            }
                        }
					}
                    else if (isOnBoard(move) && isOccupied(move)->colour() == colour){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
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
					if (isOccupied(move) == nullptr && isOnBoard(move)){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
                        moves.push_back(move);
                    }
                    else if (isOnBoard(move) && isOccupied(move)->colour() == otherColour){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
						moves.push_back(move);	
						blocked = true;
                        //if blocked by king, continue calculation until blocked by something else
                        if (isOccupied(move)->type() == 5){
                            for (int j = i; j < 8; j++){
                                move = pos + glm::vec2(-j,0);
                                if (isOnBoard(move) && isOccupied(move) == nullptr){
                                    attackers[colour][(int)move.x][(int)move.y] += 1;
                                }
                            }
                        }
					}
                    else if (isOnBoard(move) && isOccupied(move)->colour() == colour){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
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
					if (isOccupied(move) == nullptr && isOnBoard(move)){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
                        moves.push_back(move);
                    }
                    else if (isOnBoard(move) && isOccupied(move)->colour() == otherColour){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
						moves.push_back(move);	
						blocked = true;
                        //if blocked by king, continue calculation until blocked by something else
                        if (isOccupied(move)->type() == 5){
                            for (int j = i; j < 8; j++){
                                move = pos + glm::vec2(j,j);
                                if (isOnBoard(move) && isOccupied(move) == nullptr){
                                    attackers[colour][(int)move.x][(int)move.y] += 1;
                                }
                            }
                        }
					}
                    else if (isOnBoard(move) && isOccupied(move)->colour() == colour){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
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
					if (isOccupied(move) == nullptr && isOnBoard(move)){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
                        moves.push_back(move);
                    }
                    else if (isOnBoard(move) && isOccupied(move)->colour() == otherColour){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
						moves.push_back(move);	
						blocked = true;
                        //if blocked by king, continue calculation until blocked by something else
                        if (isOccupied(move)->type() == 5){
                            for (int j = i; j < 8; j++){
                                move = pos + glm::vec2(j,-j);
                                if (isOnBoard(move) && isOccupied(move) == nullptr){
                                    attackers[colour][(int)move.x][(int)move.y] += 1;
                                }
                            }
                        }
					}
                    else if (isOnBoard(move) && isOccupied(move)->colour() == colour){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
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
					if (isOccupied(move) == nullptr && isOnBoard(move)){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
                        moves.push_back(move);
                    }
                    else if (isOnBoard(move) && isOccupied(move)->colour() == otherColour){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
						moves.push_back(move);	
						blocked = true;
                        //if blocked by king, continue calculation until blocked by something else
                        if (isOccupied(move)->type() == 5){
                            for (int j = i; j < 8; j++){
                                move = pos + glm::vec2(-j,j);
                                if (isOnBoard(move) && isOccupied(move) == nullptr){
                                    attackers[colour][(int)move.x][(int)move.y] += 1;
                                }
                            }
                        }
					}
                    else if (isOnBoard(move) && isOccupied(move)->colour() == colour){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
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
					if (isOccupied(move) == nullptr && isOnBoard(move)){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
                        moves.push_back(move);
                    }
                    else if (isOnBoard(move) && isOccupied(move)->colour() == otherColour){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
						moves.push_back(move);	
						blocked = true;
                        //if blocked by king, continue calculation until blocked by something else
                        if (isOccupied(move)->type() == 5){
                            for (int j = i; j < 8; j++){
                                move = pos + glm::vec2(-j,-j);
                                if (isOnBoard(move) && isOccupied(move) == nullptr){
                                    attackers[colour][(int)move.x][(int)move.y] += 1;
                                }
                            }
                        }
					}
                    else if (isOnBoard(move) && isOccupied(move)->colour() == colour){
                        attackers[colour][(int)move.x][(int)move.y] += 1;
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
                            //square empty and not attacked
                            //std::cout<<"KING MOVE: "<<move.x<<", "<<move.y<<" attackers: "<<isAttacked(otherColour,move)<<std::endl;
                            if (isOccupied(move) == nullptr && isAttacked(otherColour,move) == 0){
                                attackers[colour][(int)move.x][(int)move.y] += 1;
                                moves.push_back(move);
                            }
                            //square empty and attacked
                            else if (isOccupied(move) == nullptr){
                                attackers[colour][(int)move.x][(int)move.y] += 1;
                            }
                            //square has opponents piece undefended
                            else if (isOccupied(move)->colour() == otherColour && isAttacked(otherColour,move) == 0){
                                attackers[colour][(int)move.x][(int)move.y] += 1;
                                moves.push_back(move);
                            }
                            //square has oppenents piece defended
                            else if (isOccupied(move)->colour() == otherColour){
                                attackers[colour][(int)move.x][(int)move.y] += 1;
                            }
                            //square has players piece 
                            else if (isOccupied(move)->colour() == colour){
                                attackers[colour][(int)move.x][(int)move.y] += 1;
                            }
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
                        //Check if pieces are between rook and king or if attacked
						for (int i = pos.x-1; i > 0; i--){
							if (isOccupied(i,(int)pos.y) != nullptr || isAttacked(otherColour,i,(int)pos.y)) blocked = true;
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
							if (isOccupied(i,(int)pos.y) != nullptr || isAttacked(otherColour,i,(int)pos.y)) blocked = true;
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
    if (setPieceMoves) piece -> setMoves(moves);
}

void Board::pruneMovesForCheck(Piece* king){
    Piece* piece;
    int numkingattackers = isKingInCheck(king);
    //No attackers, no pruning
    if (numkingattackers == 0) return;

    //One attacker, have to find king moves and blocks
    else if (numkingattackers == 1){
        for (unsigned int i = 0; i < 8; i++){
            for (unsigned int j = 0; j < 8; j++){
                piece = isOccupied(i,j);
                if (piece != nullptr){
                    if (piece->colour() == king->colour() && piece->type() != 5){
                        for (auto move:piece->moves){
                            if !doesBlockCheck(king,piece,move) //FINISH THIS
                        }
    }

    //Two+ attackers, king has to move
    else{
        std::vector<glm::vec2> moves;
        for (unsigned int i = 0; i < 8; i++){
            for (unsigned int j = 0; j < 8; j++){
                piece = isOccupied(i,j);
                if (piece != nullptr){
                    if (piece->colour() == king->colour() && piece->type() != 5){
                        piece->setMoves(moves);
                    }
                }
            }
        }
    }
}

bool Board::doesBlockCheck(Piece* king, Piece* piece, glm::vec2 move){
    glm::vec2 origpos = piece->boardPos();
    tempMovePiece(piece,move);
    Piece* opppiece;
    int oppcolour = 1 - piece->colour();
    int *** attackers[2][8][8] = {}; //initialized to 0
    for (unsigned int i = 0; i < 8; i++){
        for (unsigned int j = 0; j < 8; j++){
            opppiece = isOccupied(i,j);
            if (opppiece != nullptr){
                if (opppiece -> colour() == oppcolour)
                    calculateMoves(opppiece,attackers,setPieceMoves = false);
            }    
        }
    }
    tempMovePiece(piece,origpos);
    int numattackers = attackers[oppcolour][(int)king->boardPos().x][(int)king->boardPos().y]
    if (numattackers == 0) return true;
    return false;

int Board::isAttacked(int colour, int x, int y){
    return m_attackers[colour][x][y];
}

int Board::isAttacked(int colour, glm::vec2 pos){
    return isAttacked(colour, (int)pos.x, (int)pos.y);
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
	setPiece(piece->boardPos(), nullptr);
	setPiece(x,y, piece);
    piece->move(x,y);

    //For next players turn
    calculateAllMoves();
    resetJustMoved(turnColour);
    
    //find opponents king
    Piece* oppking;
    for (unsigned int i = 0; i < 8; i++){
        for (unsigned int j = 0; j < 8; j++){
            if (isOccupied(i,j) != nullptr){
                if (isOccupied(i,j)->type() == 5 && isOccupied(i,j)->colour() != turnColour){
                    oppking = isOccupied(i,j);
                }
            }
        }
    }
    //Prune moves to get out of check if in check
    pruneMovesForCheck(oppking);
}

void Board::tempMovePiece(Piece* piece, int x, int y){
	setPiece(piece->boardPos(), nullptr);
	setPiece(x,y, piece);
}

void Board::tempMovePiece(Piece* piece, glm::vec2 pos){
	tempMovePiece(piece, (int)pos.x, (int)pos.y);
}

void Board::calculateAllMoves(){
    resetAttackers();
    //Calculate all moves other than kings
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			if (m_pieces[i][j] != nullptr){
				if (m_pieces[i][j]->type() < 5){
                    calculateMoves(m_pieces[i][j],m_attackers);
                }
			}
		}
	}
    //Calculate king moves
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			if (m_pieces[i][j] != nullptr){
				if (m_pieces[i][j]->type() == 5){
                    calculateMoves(m_pieces[i][j],m_attackers);
                }
			}
		}
	}
    std::cout<<"Attackers:"<<std::endl;
    for (unsigned int player = 0; player <= 1; player++){
        std::cout<<"player "<<player<<std::endl;
        for (unsigned int i = 0; i < 8; i++){
            for (unsigned int j = 0; j < 8; j++){
                std::cout << m_attackers[player][i][j]<< " ";
            }
            std::cout<<std::endl;
        }
        std::cout<<std::endl;
    }

}

void Board::resetPiece(Piece* piece){
	piece->setPos(piece->tileToPos(piece->boardPos()));
}

void Board::resetJustMoved(int turnColour){
    Piece* piece;
    for (unsigned int i = 0; i < 8; i++){
        for (unsigned int j = 0; j < 8; j++){
            piece = isOccupied(i,j);
            if (piece != nullptr){
                if (piece->colour() != turnColour && piece->wasJustMoved()) piece->wasNotJustMoved();
            }
        }
    }
}

bool Board::isOnBoard(int x, int y){
	if (x < 0 || x > 7 || y < 0 || y > 7) return false;
	else return true;
}

bool Board::isOnBoard(glm::vec2 pos){
	return isOnBoard(pos.x, pos.y);
}

int Board::isKingInCheck(Piece* king){
	 return m_attackers[1-king->colour()][(int)king->boardPos().x][(int)king->boardPos().y];
}

