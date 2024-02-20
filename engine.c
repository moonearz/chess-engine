#include "stdio.h"
#include "defs.h"

//fen examples
#define FEN1 "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define FEN2 "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define FEN3 "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"
#define FEN4 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
//if white to move add e6 ep square, w 
#define TEST_PAWN_MOVES "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR b KQkq - 0 1"
#define TEST_KNIGHTS_KINGS "5k2/1n6/4n3/6N1/8/3N4/8/5K2 w - - 0 1"

int main() {
    AllInit();

    S_BOARD board[1];

    //testing move generation
    ParseFen(TEST_KNIGHTS_KINGS, board);
    PrintBoard(board);

    S_MOVELIST list[1];

    GenerateAllMoves(board, list);

    //PrintMoveList(list);   
    
    return 0;
}