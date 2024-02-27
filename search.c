#include "stdio.h"
#include "defs.h"

//check if time is up
static void CheckIn() {

}

static int IsRepetition(const S_BOARD *pos) {
    int index = 0;

    for(index = pos->hisPly - pos->fiftyMove; index < pos->hisPly - 1; ++index) {
        ASSERT(index >= 0 && index < MAXGAMEMOVES);
        
        if(pos->posKey  == pos->history[index].posKey) {
            return TRUE;
        }
    }

    return FALSE;
}

static void ClearForSearch(S_BOARD *pos, S_SEARCHINFO *info) {
    int index = 0;
    int index2 = 0;

    for(index = 0; index  < 13; ++index) {
        for(index2 = 0; index2 < BRD_SQ_NUM; ++index2) {
            pos->searchHistory[index][index2] = 0;
        }
    }

    for(index = 0; index  < 2; ++index) {
        for(index2 = 0; index2 < MAXDEPTH; ++index2) {
            pos->searchEnders[index][index2] = 0;
        }
    }

    ClearPvTable(pos->PvTable);
    pos->ply = 0;

    info->starttime = GetTimeMs();
    info->stopped = 0;
    info->nodes = 0;
}

static int Quiescence(int alpha, int beta, S_BOARD *pos, S_SEARCHINFO *info) {
    return 0;
}

static int AlphaBeta(int alpha, int beta, int depth, S_BOARD *pos, S_SEARCHINFO *info, int DoNull) {
    return 0;
}

void SearchPosition(S_BOARD *pos, S_SEARCHINFO *info) {

}