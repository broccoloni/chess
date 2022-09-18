#include "BoardStates.h"
#include <iostream>
BoardStates::BoardStates(){

}

BoardStates::~BoardStates(){

}

int BoardStates::addState(Piece* pieces[8][8]){
    BoardState* boardState = getBoardState(pieces);
    int repind = repetitions(boardState);
    m_boardStates.push_back(boardState);
    //unique board state
    if (repind == -1){
        m_boardStateCounts.push_back(1);
        m_uniqueBoardStateInds.push_back(m_boardStates.size()-1);    
        return 1;
    }
    //repeated board state
    else{
        m_boardStateCounts[repind] += 1;
        return m_boardStateCounts[repind];
    }
}

int BoardStates::repetitions(BoardState* boardState){
    //returns index of m_uniqueBoardStateInds where repetition occurs
    //returns -1 if no repetition found
    if (m_boardStates.size() == 0) return -1;
    BoardState* bs;
    bool dif;
    for (unsigned int i = 0; i < m_uniqueBoardStateInds.size(); i++){
        dif = false;
        bs = m_boardStates[m_uniqueBoardStateInds[i]];
        if (boardState->numWPieces != bs->numWPieces) {
            continue;
        }
        if (boardState->numBPieces != bs->numBPieces){
            continue;
        }
        if (boardState->numWPawns != bs->numWPawns){ 
            continue;
        }
        if (boardState->numBPawns != bs->numBPawns){ 
            continue;
        }
        //en passant rights
        for (unsigned int j = 0; j < 16; j++){
            if (boardState->enPassantRights[j] != bs->enPassantRights[j]){
                dif = true;
                break;
            }
        }
        if (dif) continue;
        
        for (unsigned int j = 0; j < 4; j++){
            if (boardState->castlingRights[j] != bs->castlingRights[j]){
                dif = true;
                break;
            }
        }
        if (dif) continue;

        for (unsigned int j = 0; j < 8; j++){
            for (unsigned int k = 0; k < 8; k++){
                if (boardState->board[j][k] != bs->board[j][k]){
                    dif = true;
                    break;
                }
            }
        }
        if (dif) continue;
        
        //if no difference is found between board states then it's a repetition
        return i;
    } 
    //if it gets this far, it's unique
    return -1;
}

BoardState * BoardStates::getBoardState(Piece* pieces[8][8]){
    BoardState* boardState = new BoardState;
    Piece* piece;
    bool wKingMoved = true;
    bool bKingMoved = true;
    boardState->numWPieces = 0;
    boardState->numWPawns = 0;
    boardState->numBPieces = 0;
    boardState->numBPawns = 0;

    for (unsigned int i = 0; i < 16; i++){
        boardState->enPassantRights[i] = false;
    }

    for (unsigned int i = 0; i < 8; i++){
        for (unsigned int j = 0; j < 8; j++){
            piece = pieces[i][j];

            if (piece == nullptr){
                boardState->board[i][j] = 0;
            }
            else{
                //check rook and king locations for castling rights
                //from perspective of white
                //white left rook
                if (i == 0 && j == 0 && piece->type() == 1 && piece->timesMoved() == 0){
                    boardState->castlingRights[0] = true;
                }
                //white right rook
                if (i == 7 && j == 0 && piece->type() == 1 && piece->timesMoved() == 0){
                    boardState->castlingRights[1] = true;
                }
                //white king
                if (i == 4 && j == 0 && piece->type() == 5 && piece->timesMoved() == 0){
                    wKingMoved = false;
                }
                //black left rook
                if (i == 0 && j == 7 && piece->type() == 1 && piece->timesMoved() == 0){
                    boardState->castlingRights[2] = true;
                }
                //black right rook
                if (i == 7 && j == 7 && piece->type() == 1 && piece->timesMoved() == 0){
                    boardState->castlingRights[3] = true;
                }
                //black king
                if (i == 4 && j == 7 && piece->type() == 5 && piece->timesMoved() == 0){
                    bKingMoved = false;
                }
                
                if (piece->colour() == 0){
                    boardState->numWPieces += 1;
                    if (piece->type() == 0) {
                        boardState->numWPawns += 1;
                        if (piece->boardPos().y == 1 || (piece->boardPos().y == 3 && piece->wasJustMoved())){
                            boardState->enPassantRights[piece->ID()] = true;        
                        }
                    }
                    boardState->board[i][j] = piece->type()+1;
                }
                else{
                    boardState->numBPieces += 1;
                    if (piece->type() == 0) {
                        boardState->numBPawns += 1;
                        if (piece->boardPos().y == 6 || (piece->boardPos().y == 4 && piece->wasJustMoved())){
                            boardState->enPassantRights[8+piece->ID()] = true;        
                        }
                    }
                    boardState->board[i][j] = -1*(piece->type()+1);
                }
            }
        }
    }
    if (wKingMoved){
        boardState->castlingRights[0] = false;
        boardState->castlingRights[1] = false;
    }
    if (bKingMoved){
        boardState->castlingRights[2] = false;
        boardState->castlingRights[3] = false;
    }
    return boardState;
}

void BoardStates::printBoardState(int statenum){
    BoardState* bs = m_boardStates[statenum];
    printBoardState(bs);
}
    
void BoardStates::printBoardState(BoardState* bs){
    std::cout<<"Board: "<<std::endl;
    std::cout<<"\033[32mWhite\033[0m"<<std::endl;
    std::cout<<"\033[31mBlack\033[0m"<<std::endl;

    std::cout<<"  ";
    for (unsigned int i = 0; i < 8; i++) std::cout<<i+1<<" ";
    std::cout<<std::endl;

    for (unsigned int i = 0; i < 8; i++){
        std::cout<<(char)(97+i)<<" ";
        for (unsigned int j = 0; j < 8; j++){
            if (bs->board[i][j] < 0) std::cout << "\033[31m"<<-1*bs->board[i][j]<<"\033[0m ";
            else if (bs->board[i][j] > 0) std::cout << "\033[32m"<<bs->board[i][j]<<"\033[0m ";
            else std::cout << "0 ";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;

    std::cout<<"True: "<<true<<std::endl;
    std::cout<<"False: "<<false<<std::endl;

    std::cout<<"Castling Rights"<<std::endl;
    for (unsigned int i = 0; i < 4; i++){
        std::cout<<bs->castlingRights[i];
    }
    std::cout<<std::endl;

    std::cout<<"en Passant Rights"<<std::endl;
    for (unsigned int i = 0; i < 16; i++){
        std::cout<<bs->enPassantRights[i];
    }
    std::cout<<std::endl;

    std::cout<<"Num W Pieces: "<<bs->numWPieces<<std::endl;
    std::cout<<"Num B Pieces: "<<bs->numBPieces<<std::endl;
    std::cout<<"Num W Pawns: "<<bs->numWPawns<<std::endl;
    std::cout<<"Num B Pawns: "<<bs->numBPawns<<std::endl;

}
