//
// Created by marco on 26/05/2022.
//

#ifndef FINAL_PROJECT_BOTS_H
#define FINAL_PROJECT_BOTS_H

#include "structures.h"

_Bool checkIfBot(char *name);
int tris_bot(int map[ROWS][ROWS], char *name, int turn);
void double_bot(char **choice);
void morraCinese_bot(char **choice);
void impiccato_bot(char **choice);
void ponte_bot(char **choice, _Bool realmap[GLASS_LINES][GLASS_COLUMNS], int *pos, _Bool map[GLASS_LINES][GLASS_COLUMNS]);
void blackjack_bot(char **choice, int *pPoints, _Bool switch_);

#endif //FINAL_PROJECT_BOTS_H
