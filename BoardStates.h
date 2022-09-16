#include <vector>
#include "Piece.h"

struct BoardState{
    int board[8][8];
    std::vector<bool> castlingRights; //white left, white right, black left, black right
    std::vector<bool> enPassantPossible;
};

class BoardStates
{
    public:
        void addState(BoardState boardState); //figure out what is needed
        bool isUnique(BoardState boardState);
        BoardState getBoardState(Piece* pieces[8][8]);
    
    private:
        std::vector<BoardState> m_boardStates;
        std::vector<int> m_boardStateCounts;
        std::vector<int> m_uniqueBoardStateInds; //vector of unique boardstate indexes

}
