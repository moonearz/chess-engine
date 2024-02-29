#include "stdio.h"
#include "defs.h"
#include "stdlib.h"
#include "string.h"

//fen examples
#define FEN1 "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define FEN2 "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define FEN3 "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"
#define FEN4 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"

//move generation examples
#define TEST_PAWN_MOVES "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR b KQkq - 0 1"
#define TEST_KNIGHTS_KINGS "5k2/1n6/4n3/6N1/8/3N4/8/5K2 w - - 0 1"
#define TEST_ROOKS "6k1/8/5r2/8/1nR5/5N2/8/6K1 b - - 0 1"
#define TEST_QUEENS "6k1/8/4nq2/8/1nQ5/5N2/1N6/6K1 w - - 0 1"
#define TEST_BISHOPS "6k1/1b6/4n3/8/1n4B1/1B3N2/1N6/2b3K1 b - - 0 1"
#define TEST_CASTLING_1 "r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1"
#define TEST_CASTLING_2 "3rk2r/8/8/8/8/8/6p1/R3K2R w KQk - 0 1"
#define TEST_TRICKY "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
#define PERFT_1 "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - "
#define PERFT_2 "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"
#define PERFT_3 "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"
#define PERFT_4 "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"
#define MATE_IN_3 "2rr3k/pp3pp1/1nnqbN1p/3pN3/2pP4/2P3Q1/PPB4P/R4RK1 w - -"
#define SCOTCH_GAME "r1b1k2r/ppppnppp/2n2q2/2b5/3NP3/2P1B3/PP3PPP/RN1QKB1R w KQkq - 0 1"

int main() {
    AllInit();
    
    S_BOARD pos[1];
    S_SEARCHINFO info[1];
    InitPvTable(pos->PvTable);

    printf("type console for console mode\n");

    char line[256];
	while (TRUE) {
		memset(&line[0], 0, sizeof(line));

		fflush(stdout);
		if (!fgets(line, 256, stdin))
			continue;
		if (line[0] == '\n')
			continue;
		if (!strncmp(line, "uci",3)) {
			UCILoop(pos, info);
			if(info->quit == TRUE) break;
			continue;
		} else if (!strncmp(line, "xboard", 6))	{
			XBoardLoop(pos, info);
			if(info->quit == TRUE) break;
			continue;
		} else if (!strncmp(line, "console", 7))	{
			ConsoleLoop(pos, info);
			if(info->quit == TRUE) break;
			continue;
		} else if(!strncmp(line, "quit", 4))	{
			break;
		}
	}

    free(pos->PvTable->pTable);
    return 0;
}

//PREVIOUS CODE TESTS:
    //INITIALIZATIONS
    /*
    AllInit();

    S_BOARD board[1];
    InitPvTable(board->PvTable);
    S_MOVELIST list[1];
    S_SEARCHINFO info[1];
    */

   //TEST MOVE GENERATION
    /*
    ParseFen(START_FEN, board);
    perfttest(4, board);
    */

    //ITERATE THROUGH MOVE LIST
    /*
    GenerateAllMoves(board, list);

    
    int MoveNum = 0;
    int move = 0;

    
    PrintBoard(board);
    getchar();

    for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {
        move = list->moves[MoveNum].move;

        if(!MakeMove(board, move)) {
            continue;
        }

        printf("\nmade the move: %s\n", PrMove(move));
        PrintBoard(board);

        TakeMove(board);
        printf("\nundid: %s\n", PrMove(move));

        getchar();
    }
    */
    /*
    //PARSE MOVES, NEED VALID INPUT

    ParseFen(SCOTCH_GAME, board);
    char input[6];
    int Move = NOMOVE;
    int pvnum = 0;
    int max = 0;

    while(TRUE) {
        PrintBoard(board);
        printf("enter a move: ");
        fgets(input, 6, stdin);

        if(input[0] == 'q') {
            break;
        }
        else if(input [0] == 't') {
            TakeMove(board);
        }
        else if(input[0] == 's') {
            info->depth = 6;
            info->starttime = GetTimeMs();
            info->stoptime = GetTimeMs() + 200000;
            SearchPosition(board, info);
        }
        else {
            Move = ParseMove(input, board);
            if(Move != NOMOVE) {
                StorePvMove(board, Move);
                MakeMove(board, Move);
            }
            else {
                printf("%s not parsed\n", input);
            }
        }

        fflush(stdin);
    }
    */