//
// Created by marco on 17/02/2022.
//

#include "../headers/morracinese.h"

// Questa funzione ritorna il valore corrispondente al simbolo chiamato dal giocatore/bot
static int mc_symbols(const char choice[]) {
    if (word_comparison(choice, SASSO)) return SASSO_ID;
    if (word_comparison(choice, CARTA)) return CARTA_ID;
    if (word_comparison(choice, FORBICI)) return FORBICI_ID;
}

static _Bool gameplay_MorraCinese(playerArray *p) {
    // Creo un doppio puntatore che contiene i nomi dei due giocatori, e li copio al suo interno.
    char **players = (char**) calloc(MINDIM, sizeof(char*));
    players[0] = (char*) calloc(strlen(p[0].name)+1, sizeof(char));
    players[1] = (char*) calloc(strlen(p[1].name)+1, sizeof(char));
    strcpy(players[0], p[0].name);
    strcpy(players[1], p[1].name);
    char *choice = NULL;
    int choices[MINDIM];
    while (true) {
        _Bool turn = rand() % MINDIM;
        for (int i = 0; i < MINDIM; i++) {
            printf("[%s]Gioca un simbolo (sasso, carta o forbici)", players[turn]);
            _Bool valid = false;
            if (!checkIfBot(players[turn])) {
                while (!valid) {
                    cString(&choice, MC_MAXINPUT, false, CS_CON_MORRACINESECHOICES, UI_MESSAGE_V2, caller_);
                    valid = sString(&choice, SS_COM_MORRACINESE, SS_COMM_NOCREMSPACES);
                }
            } else {
                sleep(1);
                morraCinese_bot(&choice);
                printf(":%s\n", choice);
            }
            // Ottengo per ogni giocatore il numero corrispondente al simbolo giocato.
            choices[turn] = mc_symbols(choice);
            turn = !turn;
        }
        printf("\n%s: ", p[0].name);
        if (choices[0] == SASSO_ID) PRINT_SASSO
        else if (choices[0] == CARTA_ID) PRINT_CARTA
        else if (choices[0] == FORBICI_ID) PRINT_FORBICI
        printf("%s: ", p[1].name);
        if (choices[1] == SASSO_ID) PRINT_SASSO
        else if (choices[1] == CARTA_ID) PRINT_CARTA
        else if (choices[1] == FORBICI_ID) PRINT_FORBICI
        // Per determinare il vincitore eseguo un semplice calcolo che determina qual'è la differenza tra i due valori.
        if (WINCONDITION_FIRSTPLAYER(choices[1], choices[0])) {
            return true;
        }
        else if (choices[1] != choices[0]) return false;
    }
}

int morraCinese(playerArray *p) {
    _Bool winner = gameplay_MorraCinese(p);
    // Verifico il trucco definito nelle specifiche
    if (strcmp(p[0].name,FRONTMAN) == 0 ) {
        winner_stamp(p[0].name);
        return p[0].id;
    }
    else if (strcmp(p[1].name,FRONTMAN) == 0) {
        winner_stamp(p[1].name);
        return p[1].id;
    }
    winner_stamp(p[winner].name);
    return p[winner].id;
}