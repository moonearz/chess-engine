#include "stdio.h"
#include "defs.h"

//fen examples
#define FEN1 "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define FEN2 "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define FEN3 "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"
#define FEN4 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"

int main() {
    AllInit();

    S_BOARD board[1];

    //testing from-to move
    int move = 0;
    int from = A2; int to = H7;
    int cap = wR; int prom = bQ;
    move = ( ( from ) | ( to << 7 ) | ( cap << 14 ) | ( prom << 20 ) ) ;
    printf("from: %d to: %d cap: %d prom: %d\n", FROMSQ(move), TOSQ(move), CAPTURED(move), PROMOTED(move));
    printf("Algebraic from: %s\n", PrSq(from));
    printf("Algebraic to: %s\n", PrSq(to));
    printf("Algebraic move: %s\n", PrMove(move));

    //testing fen examples
    /*
    ParseFen(FEN4, board);
    PrintBoard(board);

    ASSERT(CheckBoard(board));
    */


    //testing move macros
    /*
    int move = 0;
    int from = 6; int to = 12;
    int cap = wR; int prom = bR;

    move = ( ( from ) | ( to << 7 ) | ( cap << 14 ) | ( prom << 20 ) ) ;

    printf("\ndec: %d hex: %X\n", move, move);

    printf("from: %d to: %d cap: %d prom: %d\n", FROMSQ(move), TOSQ(move), CAPTURED(move), PROMOTED(move));
    */
    
    return 0;
}