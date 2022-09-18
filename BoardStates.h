#include <vector>
#include "Piece.h"

struct BoardState{
    int board[8][8];
    bool castlingRights[4]; //white left, white right, black left, black right
    bool enPassantRights[16];
    int numWPieces; // These are not necessary for checking equivalence of board states
    int numWPawns;  // but make checking uniqueness quicker
    int numBPieces; //
    int numBPawns;  // 
};

class BoardStates
{
    public:
        BoardStates();
        ~BoardStates();
        int addState(Piece* pieces[8][8]); //returns highest number of repetitions (3 becomes draw)
        int repetitions(BoardState * boardState);
        BoardState * getBoardState(Piece* pieces[8][8]);
        void printBoardState(BoardState * bs);
        void printBoardState(int statenum);
        
        //int getBoard()[8][8] { return m_boardStates.back().board; } //last board
        //int getBoard(int movenum)[8][8] {return m_boardStates[movenum].board; }

    private:
        std::vector<BoardState *> m_boardStates;
        std::vector<int> m_boardStateCounts;
        std::vector<int> m_uniqueBoardStateInds; //vector of unique boardstate indexes

};
