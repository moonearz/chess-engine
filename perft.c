#include "defs.h"
#include "stdio.h"

//this variable is only visible here, not good
long leafNodes;

void perft(int depth, S_BOARD *pos) {
    ASSERT(CheckBoard(pos));

    if(depth == 0) {
        leafNodes++;
        return;
    }

    S_MOVELIST list[1];
    GenerateAllMoves(pos, list);

    int MoveNum = 0;
    for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {
        if(!MakeMove(pos, list->moves[MoveNum].move)) {
            continue;
        }
        perft(depth - 1, pos);
        TakeMove(pos);
    }

    return;
}

void perfttest(int depth, S_BOARD *pos) {
    ASSERT(CheckBoard(pos));

    PrintBoard(pos);

    printf("\ntesting to depth %d\n", depth);
    leafNodes = 0;

    S_MOVELIST list[1];
    GenerateAllMoves(pos, list);

    int move;
    int MoveNum = 0;
    for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {
        move = list->moves[MoveNum].move;
        if(!MakeMove(pos, move)) {
            continue;
        }
        long sumNodes = leafNodes;
        //error is here
        perft(depth - 1, pos);
        TakeMove(pos);
        long oldNodes = leafNodes - sumNodes;
        printf("move %d: %s: %ld\n", MoveNum + 1, PrMove(move), oldNodes);
    }

    printf("\ntest complete visiting %ld leaf nodes\n", leafNodes);
    return;
}