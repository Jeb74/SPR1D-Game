//
// Created by marco on 25/02/2022.
//

#ifndef FINAL_PROJECT_FILEMANAGEMENT_H
#define FINAL_PROJECT_FILEMANAGEMENT_H

#include "structures.h"

int mainFileManager(int *K, player **parray, int *_pos, int *N, int *M, int **pid, int **alivep);
_Bool saveGame(int DIM, player **pArray, int _pos, int inGame, int N, int M, int **pIds, int **alivePlayers);
#endif //FINAL_PROJECT_FILEMANAGEMENT_H
