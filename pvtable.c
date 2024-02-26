#include "stdio.h"
#include "defs.h"

const int PvSize = 0x100000 * 2;

int GetPvLine(const int depth, S_BOARD *pos) {
    ASSERT(depth < MAXDEPTH);

    int move = ProbePvTable(pos);
    int count = 0;

    while(move != NOMOVE && count < depth) {
        ASSERT(count < MAXDEPTH);

        if(MoveExists(pos, move)) {
            MakeMove(pos, move);
            pos->PvArray[count++] = move;
        }
        else {
            break;
        }
        move = ProbePvTable(pos);
    }

    while(pos->ply > 0) {
        TakeMove(pos);
    }

    return count;
}

void ClearTable(S_PVTABLE *table) {
    S_PVENTRY *PvEntry;
    
    for(PvEntry = table->pTable; PvEntry < table->pTable + table->numEntries; ++PvEntry) {
        PvEntry->posKey = 0ULL;
        PvEntry->move = NOMOVE;
    }
}

void InitPvTable(S_PVTABLE *table) {
    table->numEntries = PvSize / sizeof(S_PVENTRY);
    //to make sure not to exceed bounds via indexing method
    table->numEntries -= 2;
    free(table->pTable);
    table->pTable = (S_PVENTRY *) malloc(table->numEntries * sizeof(S_PVENTRY));
    ClearTable(table);
    printf("PvTable init complete with %d entries\n", table->numEntries);
}

void StorePvMove(const S_BOARD *pos, const int move) {
    int index = pos->posKey % pos->PvTable->numEntries;
    ASSERT(index >= 0 && index <= pos->PvTable->numEntries - 1);

    pos->PvTable->pTable[index].move = move;
    pos->PvTable->pTable[index].posKey = pos->posKey;    
}

int ProbePvTable(const S_BOARD *pos) {
    int index = pos->posKey % pos->PvTable->numEntries;
    ASSERT(index >= 0 && index <= pos->PvTable->numEntries - 1);

    if(pos->PvTable->pTable[index].posKey == pos->posKey) {
        return pos->PvTable->pTable[index].move;
    }

    return NOMOVE;
}