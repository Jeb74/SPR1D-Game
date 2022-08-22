//
// Created by marco on 20/02/2022.
//

#include "../headers/murra.h"

_Bool gameplay_murra (playerArray *p) {
    // Creo un doppio puntatore che contiene i nomi dei due giocatori, e li copio al suo interno.
    char **players = (char**) calloc(MINDIM, sizeof(char*));
    players[0] = (char*) calloc(strlen(p[0].name)+1, sizeof(char));
    players[1] = (char*) calloc(strlen(p[1].name)+1, sizeof(char));
    strcpy(players[0], p[0].name);
    strcpy(players[1], p[1].name);
    int sum = 0;
    char *num = NULL;
    _Bool turn = rand()%MINDIM;
    _Bool valid = true;
    int number;
    while (true) {
        for (int i = 0; i < MINDIM; i++) {
            printf("[%s]Che numero vorresti dichiarare?\n", players[i]);
            if (!checkIfBot(players[i])) {
                cString(&num, 1, true, CS_CON_MURRARANGE, UI_MESSAGE_V1, caller_);
                sum += numchecker(num[0]);
            }
            else {
                number = rand()%HUMANFINGERS+1;
                printf(">>%i\n", number);
                sum += number;
            }
        }
        printf("[%s]A quanto ammonta la somma dei numeri che avete dichiarato?\n", players[turn]);
        if (!checkIfBot(players[turn])) {
            // Se è il giocatore a inserire il numero risultante, egli avrà un tempo di soli due secondi per farlo,
            // questa scelta è dovuta a una ricerca di rendere questo gioco il più simile possibile alla realtà, dove
            // la chiamata dei numeri e della loro somma avviene quasi simultaneamente alla calata dei due numeri da
            // parte dei due giocatori.
            time_t init = time(NULL);
            cString(&num, MINDIM, false, CS_CON_MURRATOTALRANGE, UI_MESSAGE_V1, caller_);
            time_t final = time(NULL);
            valid = ((final-init) < MINDIM);
            number = xtdnumchecker(num,false);
        }
        else {
            number = rand()%MURRA_TOP_EX+MINDIM;
            printf(">>%i\n", number);
            valid = true;
        }
        if (valid && number == sum) {
            freeIt(&num);
            free(players[0]);
            free(players[1]);
            free(players);
            return turn;
        }
        else {
            turn = !turn;
            sum = 0;
        }
    }
}

int murraGame(playerArray *p) {
    _Bool winner = gameplay_murra(p);
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