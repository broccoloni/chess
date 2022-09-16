#include "BoardStates.h"

BoardStates::BoardStates(){

}

BoardStates::~BoardStates(){

}

void BoardStates::addState(BoardState boardState){

}

bool BoardStates::isUnique(BoardState boardState){

}

BoardState BoardStates::getBoardState(Piece* pieces[8][8]){
    BoardState boardState;
    Piece* piece;
    for (unsigned int i = 0; i < 8; i++){
        for (unsigned int j = 0; j < 8; j++){
            piece = pieces[i][j];

            if (piece == nullptr){
                boardState.board[i][j] = 0;
            }
            else{
                if (piece->colour() == 0){
                    boardState.board[i][j] = piece->type()+1;
                }
                else{
                    boardState.board[i][j] = -1*(piece->type()+1);
                }
                if (piece->type() == 0){

                }
            }
        }
    }

}

int 
