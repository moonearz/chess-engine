#include "stdio.h"
#include "defs.h"

const int PvSize = 0x100000 * 2;

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
