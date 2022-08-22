//
// Created by marco on 26/05/2022.
//

#include "headers/bots.h"

// Questa funzione serve a controllare se il nome del giocatore che sta giocando sia di un bot o di un giocatore utente

_Bool checkIfBot(char *name) {
    match *match = contains(BOTS_ROOT, name, true);
    int **result = checkMatches(&match);
    if (result == NULL) return false;
    else {
        if (result[0][0] > 1) {
            free(result);
            return false;
        }
        else if (result[1][0] != 0) {
            free(result);
            return false;
        }
        else {
            free(result);
            return true;
        }
    }
}

/*#################################################################
 *                              TRIS BOT
 *#################################################################
 */

static _Bool checkMove(int map[ROWS][ROWS], int pDim, int turn, int ***positions) {
    // Prendo in esame una singola posizione scelta ogni volta dal ciclo i.
    for (int i = 0; i < pDim && pDim > 1; i++) {
        // Con il ciclo j scorro tutte le altre posizioni che compongono l'array, partendo dalla posizione i + 1 in modo
        // da non ripetere i controlli
        for (int j = i + 1; j < pDim; j++) {
            // Verifico se la posizione i e la posizione j hanno le stesse coordinate della riga
            if ((*positions)[i][0] == (*positions)[j][0]) {
                int x = (*positions)[i][0];
                // Effettuo una sottrazione sulle colonne per determinare l'ipotetica posizione in cui potrebbe essere
                // inserito il simbolo della prossima mossa del bot.
                int y = (ROWS - (*positions)[j][1]) - (*positions)[i][1];
                // Se la posizione è vuota (c'è uno spazio) allora effettuo la mossa libero l'array di posizioni e
                // restituisco il valore true
                if (map[x][y] == MINDIM) {
                    map[x][y] = turn;
                    printf("%c%i\n", y + LOWEST_VALUE_TRIS, x + 1);
                    for (int k = 0; k < pDim; k++) {
                        free((*positions)[k]);
                    }
                    free(*positions);
                    return true;
                }
                // Verifico se la posizione i e la posizione j hanno le stesse coordinate della colonna
            } else if ((*positions)[i][1] == (*positions)[j][1]) {
                // Applico lo stesso calcolo per questa situazione ma questa volta sulle righe
                int x = (ROWS - (*positions)[j][0]) - (*positions)[i][0];
                int y = (*positions)[i][1];
                if (map[x][y] == MINDIM) {
                    map[x][y] = turn;
                    printf("%c%i\n", y + LOWEST_VALUE_TRIS, x + 1);
                    for (int k = 0; k < pDim; k++) {
                        free((*positions)[k]);
                    }
                    free(*positions);
                    return true;
                }
                // Effettuo un controllo sulle diagonali
            } else {
                // Per le diagonali invece il ragionamento deve essere applicato sia alle colonne che alle righe
                int x = (ROWS - (*positions)[j][0]) - (*positions)[i][0];
                int y = (ROWS - (*positions)[j][1]) - (*positions)[i][1];
                if (map[x][y] == MINDIM) {
                    map[x][y] = turn;
                    printf("%c%i\n", y + LOWEST_VALUE_TRIS, x + 1);
                    for (int k = 0; k < pDim; k++) {
                        free((*positions)[k]);
                    }
                    free(*positions);
                    return true;
                }
            }
        }
    }
    for (int i = 0; i < pDim; i++) {
        free((*positions)[i]);
    }
    free(*positions);
    return false;
}

static _Bool registerMovePos(int map[ROWS][ROWS], int turn) {
    int pDim = 0;
    int **positions = (int**) calloc(MAX_ALLOWED_POS, sizeof(int*));
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < ROWS; j++) {
            // Salvo la posizione di tutte le mosse effettuate dal bot nella mappa.
            if (map[i][j] == turn) {
                pDim++;
                positions[pDim-1] = (int*) calloc(MINDIM, sizeof(int));
                positions[pDim-1][0] = i;
                positions[pDim-1][1] = j;
            }
        }
    }
    if (positions != NULL) return checkMove(map, pDim, turn, &positions);
    else return false;
}

int tris_bot(int map[ROWS][ROWS], char *name, int turn) {
    printf("%s ha fatto la sua mossa:", name);
    // Verifico se è possibile effettuare una mossa vincente
    _Bool smartPos = registerMovePos(map, turn);
    _Bool breaks = false;
    // Altrimenti il bot effettua una mossa qualsiasi
    if (!smartPos) {
        while (!breaks) {
            int row = rand() % ROWS;
            int columns = rand() % ROWS;
            if (map[row][columns] == MINDIM) {
                map[row][columns] = turn;
                printf("%c%i\n", columns + LOWEST_VALUE_TRIS, row + 1);
                breaks = true;
            }
        }
    }
    return (turn == 0) ? turn+1 : turn-1;
}

// Queste due sezioni successive non hanno bisogno di alcun bot dedicato in quanto questi due giochi si basano solo sulla
// generazione di un numero randomico

/*#################################################################
 *                              DADI BOT
 *#################################################################
 */

// Nessun bisogno di un bot dedicato per questo gioco

/*#################################################################
 *                              MURRA BOT
 *#################################################################
 */

// Nessun bisogno di un bot dedicato per questo gioco

/*#################################################################
 *                 PARI O DISPARI & BIGLIE BOT
 *#################################################################
 */

// Questo bot è valido sia per il pari o dispari e sia per il gioco delle biglie in quanto fa semplicemente
// scegliere randomicamente una tra le due opzioni (PARI/DISPARI)

void double_bot(char **choice) {
    switch (rand() % MINDIM) {
        default: {
            *choice = (char *) calloc(strlen(POD_DISPARI) + 1, sizeof(char));
            strcpy(*choice, POD_DISPARI);
        }
            break;
        case 1: {
            *choice = (char *) calloc(strlen(POD_PARI) + 1, sizeof(char));
            strcpy(*choice, POD_PARI);
        }
            break;
    }
}

/*#################################################################
 *                       MORRACINESE BOT
 *#################################################################
 */

// Questo bot genera randomicamente uno dei simboli del gioco MORRACINESE

void morraCinese_bot(char **choice) {
    switch(rand()%NUM_OF_OPTIONS) {
        default: {
            *choice = (char*) calloc(strlen(SASSO)+1, sizeof(char));
            strcpy(*choice, SASSO);
        }
            break;
        case 1: {
            *choice = (char*) calloc(strlen(CARTA)+1, sizeof(char));
            strcpy(*choice, CARTA);
        }
            break;
        case MINDIM: {
            *choice = (char*) calloc(strlen(FORBICI)+1, sizeof(char));
            strcpy(*choice, FORBICI);
        }
            break;
    }
}

/*#################################################################
 *                       IMPICCATO BOT
 *#################################################################
 */

// Questo bot genera molto semplicemente una lettera maiuscola qualsiasi.

void impiccato_bot(char **choice) {
    *choice = (char*) calloc(MINDIM, sizeof(char));
    *choice[0] = rand()%(NEXT_Z_CHAR - LOWEST_VALUE_TRIS) + LOWEST_VALUE_TRIS;
}

/*#################################################################
 *                         PONTE BOT
 *#################################################################
 */

// Il bot del ponte di vetro si basa su una sorta di probabilità, esso può o effettuare una mossa corretta o una mossa
// sbagliata in base al numero che esce.
// Se il numero randomico è compreso tra 70 e 99 estremi compresi allora la mossa effettuata è una mossa corretta,
// altrimenti la sua mossa viene decisa randomicamente sulla base della situazione attuale della mappa.
// Per essere più precisi: Dato che esistono due mappe per il ponte di vetro, una da mostrare al giocatore e una
// nascosta che determina quali vetri si possono rompere e quali no, il bot prende in esame l'una o l'altra mappa in base
// al numero generato. Questo fa si che la mossa effettuata possa essere per il 50% corretta e per il 50% sbagliata, in
// quanto anche nel caso in cui il bot prendesse in esame la mappa reale da far visualizzare al giocatore, esso potrebbe
// comunque compiere la scelta corretta.

void ponte_bot(char **choice, _Bool realmap[GLASS_LINES][GLASS_COLUMNS], int *pos, _Bool map[GLASS_LINES][GLASS_COLUMNS]) {
    int percentage = rand()%PROBABILITY_TO_MAKE_MOVE;
    if (*choice != NULL) freeIt(choice);
    *choice = (char*) calloc(MINDIM, sizeof(char));
    _Bool followMap = (percentage >= PROBABILITY_CAP) ? true : false;
    if (pos == NULL) {
        if (followMap) {
            if (realmap[0][0] == false) {
                (*choice)[0] = LEFT;
            } else (*choice)[0] = RIGHT;
        }
        else {
            if (map[0][0] != false) {
                (*choice)[0] = LEFT;
            }
            else (*choice)[0] = RIGHT;
        }
    }
    else {
        if (followMap) {
            if (realmap[pos[0]+1][pos[1]] == false) {
                (*choice)[0] = LEFT;
            } else (*choice)[0] = RIGHT;
        }
        else {
            if (map[pos[0]+1][pos[1]] != false) {
                (*choice)[0] = LEFT;
            }
            else (*choice)[0] = RIGHT;
        }
    }
}

/*#################################################################
 *                         BLACKJACK BOT
 *#################################################################
 */

// Il bot del blackjack si basa su un meccanismo molto semplice, esso continua a pescare fintanto che il valore della sua
// mano non raggiunge/supera il 18 (valore che ho considerato come safe)
void blackjack_bot(char **choice, int *pPoints, _Bool switch_) {
    if (*choice != NULL) freeIt(choice);
    *choice = (char*) calloc(BLACKJACK_CHOICE_LEN, sizeof(char));
    if ((pPoints[switch_] < pPoints[!switch_] && pPoints[switch_] < BLACKJACK_SAFE_SCORE) || pPoints[switch_] < BLACKJACK_SAFE_SCORE) {
        strcpy(*choice, FIRST_BJ_MOVE);
    }
    else {
        strcpy(*choice, SECOND_BJ_MOVE);
    }
}