#include "stdio.h"
#include "defs.h"
#include "string.h"

#define INPUTBUFFER 400 * 6

void ParseGo(char* line, S_SEARCHINFO *info, S_BOARD *pos) {

}
void ParsePosition(char* lineIn, S_BOARD *pos) {

}

void UCILoop() {
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    char line[INPUTBUFFER];
    printf("id name %s\n", NAME);
    printf("id author mbjemaa\n");
    printf("uciok\n");

    S_BOARD pos[1];
    S_SEARCHINFO info[1];
    InitPvTable(pos->PvTable);

    while(TRUE) {
        memset(&line[0], 0, sizeof(line));
        fflush(stdout);
        if(!fgets(line, INPUTBUFFER, stdin)) {
            continue;
        }

        if(line[0] == '\n') {
            continue;
        }

        if(!strncmp(line, "isready", 7)) {
            printf("readyok\n");
            continue;
        }
        else if(!strncmp(line, "position", 8)) {
            ParsePosition(line, pos);
        }
        else if(!strncmp(line, "ucinewgame", 10)) {
            ParsePosition("position startpos\n", pos);
        }
        else if(!strncmp(line, "go", 2)) {
            ParseGo(line, info, pos);
        }
        else if(!strncmp(line, "quit", 4)) {
            info->quit = TRUE;
            break;
        }
        else if(!strncmp(line, "uci", 3)) {
            printf("id name %s\n", NAME);
            printf("id author mbjemaa\n");
            printf("uciok\n");
        }
        if(info->quit) break;

    }
}