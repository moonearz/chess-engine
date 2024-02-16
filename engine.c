#include "stdio.h"
#include "defs.h"

int main() {
    AllInit();

    //testing examples
    U64 playBitBoard = 0ULL;

    playBitBoard |= (1ULL << SQ64(D2));
    playBitBoard |= (1ULL << SQ64(D3));
    playBitBoard |= (1ULL << SQ64(D4));

    PrintBitBoard(playBitBoard);
    
    return 0;
}