//
// Created by marco on 17/02/2022.
//

#include "../headers/paridispari.h"

static _Bool gameplay_pd(playerArray *p) {
    char *choice = NULL;
    _Bool turn = rand()%MINDIM;
    _Bool paridispari[MINDIM] = {false, false};
    int winner = 0;
    // Creo un doppio puntatore che contiene i nomi dei due giocatori, e li copio al suo interno.
    char **players = (char**) calloc(MINDIM, sizeof(char*));
    players[0] = (char*) calloc(strlen(p[0].name)+1, sizeof(char));
    players[1] = (char*) calloc(strlen(p[1].name)+1, sizeof(char));
    strcpy(players[0], p[0].name);
    strcpy(players[1], p[1].name);
    printf("[%s]Scegli se essere pari o dispari", players[turn]);
    if (!checkIfBot(players[turn])) {
        _Bool valid = false;
        while (!valid) {
            cString(&choice, POD_MAX_INPUT, false, CS_CON_PODCHOICEALLOWEDCHARS, UI_MESSAGE_V2, caller_);
            valid = sString(&choice, SS_COM_PARIDISPARI, SS_COMM_NOCREMSPACES);
        }
    }
    else {
        double_bot(&choice);
        printf(":%s\n", choice);
    }
    // Se la scelta dell'utente è dispari allora il turno del giocatore viene convertito al turno del giocatore pari.
    // Il motivo è che il controllo che viene effettuato è solo sul numero se è pari, di conseguenza per semplificare
    // la scelta del return è più ottimale ritornare il valore attuale nel caso in cui il numero sia pari invece
    // di determinare il return con righe di codice in più.
    if (!word_comparison(choice, POD_PARI)) turn = !turn;
    paridispari[turn] = true;
    for (int i = 0; i < MINDIM; i++) {
        printf("[%s]Inserisci un numero da 0 a 5", players[i]);
        if (!checkIfBot(players[i])) {
            cString(&choice, 1, false, CS_CON_PODRANGE, UI_MESSAGE_V2, caller_);
            winner += numchecker(choice[0]);
        }
        else {
            int num = rand()%(POD_TOP_VALUE+1);
            printf(":%i\n", num);
            winner += num;
        }
        if (i == MINDIM - 1 && winner == 0) {
            i = GARBAGE;
        }
    }
    free(players[0]);
    free(players[1]);
    free(players);
    freeIt(&choice);
    if (paridispari[turn] && winner%MINDIM == 0) return turn;
    else return !turn;
}

int pari_dispari(playerArray *p) {
    _Bool winner = gameplay_pd(p);
    // applico il controllo sul trucco "Frontman..."
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