#include "stdio.h"
#include "defs.h"

#define MATE 29000

int rootDepth;

//check if time is up
static void CheckIn(S_SEARCHINFO *info) {
    if(info->timeset == TRUE && GetTimeMs() > info->stoptime) {
        info->stopped = TRUE;
    }
    ReadInput(info);
}

static void PickNextMove(int moveNum, S_MOVELIST *list) {
    S_MOVE temp;
    int index = 0;
    int bestscore = 0;
    int bestNum = moveNum;

    for(index = moveNum; index < list->count; ++index) {
        if(list->moves[index].score > bestscore) {
            bestscore = list->moves[index].score;
            bestNum = index;
        }
    }
    temp = list->moves[moveNum];
    list->moves[moveNum] = list->moves[bestNum];
    list->moves[bestNum] = temp;
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
            pos->searchKillers[index][index2] = 0;
        }
    }

    ClearTable(pos->PvTable);
    pos->ply = 0;

    info->stopped = 0;
    info->nodes = 0;
    info->fh = 0;
    info->fhf = 0;
}

static int Quiescence(int alpha, int beta, S_BOARD *pos, S_SEARCHINFO *info) {
    ASSERT(CheckBoard(pos));

    if( (info->nodes & 2047 ) == 0) {
        CheckIn(info);
    }
    info->nodes++;

    if(IsRepetition(pos) || pos->fiftyMove >= 100) {
        return 0;
    }

    if(pos->ply > MAXDEPTH - 1) {
        return EvalPosition(pos);
    }

    int Score = EvalPosition(pos);

    if(Score >= beta)  {
        return beta;
    }

    if(Score > alpha) {
        alpha = Score;
    }

    S_MOVELIST list[1];
    GenerateAllCaptures(pos, list);
    int MoveNum = 0;
    int Legal = 0;
    int OldAlpha = alpha;
    int BestMove = NOMOVE;
    Score = -INFINITE;
    int PvMove = ProbePvTable(pos);

    for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {
        PickNextMove(MoveNum, list);

        if(!MakeMove(pos, list->moves[MoveNum].move)) {
            continue;
        }

        Legal++;
        Score = -Quiescence(-beta, -alpha, pos, info);
        TakeMove(pos);

        if(info->stopped == TRUE) {
            return 0;
        }

        if(Score > alpha) {
            if(Score >= beta) {
                if(Legal == 1) {
                    info->fhf++;
                }
                info->fh++;
                return beta;
            }
            alpha = Score;
            BestMove = list->moves[MoveNum].move;
        }
    }

    if(alpha != OldAlpha) {
        StorePvMove(pos, BestMove);
    }

    return alpha;
}

static int AlphaBeta(int alpha, int beta, int depth, S_BOARD *pos, S_SEARCHINFO *info, int DoNull) {
    ASSERT(CheckBoard(pos));

    if(depth <= 0) {
        return Quiescence(alpha, beta, pos, info);
    }

    if( (info->nodes & 2047 ) == 0) {
        CheckIn(info);
    }
    info->nodes++;

    if(IsRepetition(pos) || pos->fiftyMove >= 100 && pos->ply) {
        return 0;
    }

    if(pos->ply > MAXDEPTH - 1) {
        return EvalPosition(pos);
    }

    int InCheck = SqAttacked(pos->KingSq[pos->side], pos->side^1, pos);

    if(InCheck == TRUE) {
        depth++;
    }

    int Score = -INFINITE;

    //there are rare cases where this would cause problems, zugzwang with pieces
    if(DoNull && !InCheck && pos->ply && (pos->Pces[pos->side] > 1) && depth >= 4) {
        MakeNullMove(pos);
        Score = -AlphaBeta(-beta, -beta + 1, depth - 3, pos, info, FALSE);
        TakeNullMove(pos);
        if(info->stopped == TRUE) {
            return 0;
        }
        if(Score >= beta)  {
            return beta;
        }
    }

    S_MOVELIST list[1];
    GenerateAllMoves(pos, list);
    int MoveNum = 0;
    int Legal = 0;
    int OldAlpha = alpha;
    int BestMove = NOMOVE;
    int PvMove = ProbePvTable(pos);
    Score = -INFINITE;

    if(PvMove != NOMOVE) {
        for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {
            if(list->moves[MoveNum].move == PvMove) {
                list->moves[MoveNum].score = 2000000;
                break;
            }
        }
    }
    
    for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {
        PickNextMove(MoveNum, list);

        if(!MakeMove(pos, list->moves[MoveNum].move)) {
            continue;
        }

        Legal++;
        Score = -AlphaBeta(-beta, -alpha, depth-1, pos, info, TRUE);
        TakeMove(pos);

        if(info->stopped == TRUE) {
            return 0;
        }

        if(Score > alpha) {
            if(Score >= beta) {
                if(Legal == 1) {
                    info->fhf++;
                }
                info->fh++;

                if(!(list->moves[MoveNum].move & MFCAP)) {
                    pos->searchKillers[1][pos->ply] = pos->searchKillers[0][pos->ply];
                    pos->searchKillers[0][pos->ply] = list->moves[MoveNum].move;
                }

                return beta;
            }
            alpha = Score;
            BestMove = list->moves[MoveNum].move;
            if(!(list->moves[MoveNum].move & MFCAP)) {
                pos->searchHistory[pos->pieces[FROMSQ(BestMove)]][TOSQ(BestMove)] += depth;
            }
        }
    }

    if(Legal == 0) {
        if(InCheck) {
            return -MATE + pos->ply;
        }
        else {
            return 0;
        }
    }

    if(alpha != OldAlpha) {
        StorePvMove(pos, BestMove);
    }
    
    return alpha;
}

void SearchPosition(S_BOARD *pos, S_SEARCHINFO *info) {
    int bestmove = NOMOVE;
    int bestscore = -INFINITE;
    int currentDepth = 0;
    int pvMoves = 0;
    int pvNum = 0;
    ClearForSearch(pos, info);

    //iterative deepening
    if(bestmove == NOMOVE) {
		for( currentDepth = 1; currentDepth <= info->depth; ++currentDepth ) {
			rootDepth = currentDepth;
			bestscore = AlphaBeta(-INFINITE, INFINITE, currentDepth, pos, info, TRUE);

			if(info->stopped == TRUE) {
				break;
			}

			pvMoves = GetPvLine(currentDepth, pos);
			bestmove = pos->PvArray[0];
			if(info->GAME_MODE == UCIMODE) {
				printf("info score cp %d depth %d nodes %ld time %d ",
					bestscore,currentDepth,info->nodes,GetTimeMs()-info->starttime);
			} else if(info->GAME_MODE == XBOARDMODE && info->POST_THINKING == TRUE) {
				printf("%d %d %d %ld ",
					currentDepth,bestscore,(GetTimeMs()-info->starttime)/10,info->nodes);
			} else if(info->POST_THINKING == TRUE) {
				printf("score:%d depth:%d nodes:%ld time:%d(ms) ",
					bestscore,currentDepth,info->nodes,GetTimeMs()-info->starttime);
			}
			if(info->GAME_MODE == UCIMODE || info->POST_THINKING == TRUE) {
				pvMoves = GetPvLine(currentDepth, pos);
				if(!info->GAME_MODE == XBOARDMODE) {
					printf("pv");
				}
				for(pvNum = 0; pvNum < pvMoves; ++pvNum) {
					printf(" %s",PrMove(pos->PvArray[pvNum]));
				}
				printf("\n");
            }
		}
	}
    if(info->GAME_MODE == UCIMODE) {
		printf("bestmove %s\n", PrMove(bestmove));
	} else if(info->GAME_MODE == XBOARDMODE) {
		printf("move %s\n", PrMove(bestmove));
		MakeMove(pos, bestmove);
	} else {
		printf("\n\nmethane moves %s\n\n", PrMove(bestmove));
		MakeMove(pos, bestmove);
		PrintBoard(pos);
	}
}