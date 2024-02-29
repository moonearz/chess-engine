#include "stdio.h"
#include "defs.h"
#include "string.h"

int NumReps(const S_BOARD *pos) {
    int i = 0, r = 0;
    for(i = 0; i < pos->hisPly; ++i) {
        if(pos->history[i].posKey == pos->posKey) {
            r++;
        }
    }
    return r;
}

int DrawMaterial(const S_BOARD *pos) {
    if(pos->pceNum[wP] || pos->pceNum[bP]) return FALSE;
    if(pos->pceNum[wQ] || pos->pceNum[bQ] || pos->pceNum[wR] || pos->pceNum[bR]) return FALSE;
    if(pos->pceNum[wB] > 1 || pos->pceNum[bB] > 1) {return FALSE;}
    if(pos->pceNum[wN] > 1 || pos->pceNum[bN] > 1) {return FALSE;}
    if(pos->pceNum[wN] && pos->pceNum[wB]) return FALSE;
    if(pos->pceNum[bN] && pos->pceNum[bB]) return FALSE;

    return TRUE;
}

int checkresult(S_BOARD *pos) {
    if(pos->fiftyMove > 100) {
        printf("1/2-1/2 by fifty move rule\n"); return TRUE;
    }
    if(NumReps(pos) >= 2) {
        printf("1/2-1/2 by three-fold repetition\n"); return TRUE;
    }
    if(DrawMaterial(pos) == TRUE) {
        printf("1/2-1/2 by insufficient mating material\n"); return TRUE;
    }

    S_MOVELIST list[1];
    GenerateAllMoves(pos, list);

    int MoveNum = 0;
    int found = 0;
    for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {
        if(!MakeMove(pos, list->moves[MoveNum].move)) {
            continue;
        }
        found++;
        TakeMove(pos);
        break;
    }

    if(found != 0) {
        return FALSE;
    }

    int InCheck = SqAttacked(pos->KingSq[pos->side], pos->side^1, pos);
    if(InCheck == TRUE) {
        if(pos->side == WHITE) {
            printf("0-1 by checkmate\n"); return TRUE;
        }
        else {
            printf("1-0 by checkmate\n"); return TRUE;
        }
    }
    else {
        printf("1/2-1/2 by stalemate\n"); return TRUE;
    }
}