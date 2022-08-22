//
// Created by marco on 17/02/2022.
//

#include "../headers/dadi.h"

//Questa funzione implementa l'intero gameplay del gioco DADI
// Questa funzione ha come output un valore booleano per identificare chi sia il vincitore della partita a DADI
static _Bool gameplay_dadi(playerArray *p) {
    // Creo un array di interi da sfruttare come comparatori tra il valore precedente del giocatore 1
    // e quello precedente del giocatore 2
    int *comparators = (int*) calloc(MINDIM, sizeof(int));
    // Creo un doppio puntatore che contiene i nomi dei due giocatori, e li copio al suo interno.
    char **players = (char**) calloc(MINDIM, sizeof(char*));
    players[0] = (char*) calloc(strlen(p[0].name)+1, sizeof(char));
    players[1] = (char*) calloc(strlen(p[1].name)+1, sizeof(char));
    strcpy(players[0], p[0].name);
    strcpy(players[1], p[1].name);
    _Bool turn = rand()%MINDIM;
    int num = 0;
    while (true) {
        // Verifico che il giocatore attuale non sia un bot.
        if (!checkIfBot(players[turn])) {

            if (comparators[turn] != 0) {
                printf("Il numero precedente e': ");
                coloredText(numToString(comparators[turn]), GREEN+COLOR_INTENSITY, RESET);
                NEWLINE_FUNC
            }
            printf("[%s]Premi [ENTER] per lanciare i tuoi dadi!\n", players[turn]);
            char a;
            do {
                scanf("%c", &a);
                if ((int) a == ENTER) num = (rand() % (MAXFACE) + MINFACE) + (rand() % (MAXFACE) + MINFACE);
            } while (a != ENTER);
            a = ENDSTRING;
        }
        else {
            // Se il giocatore attuale è un bot simulo un lancio di dadi.
            if (comparators[turn] != 0) {
                printf("Il numero precedente e': ");
                coloredText(numToString(comparators[turn]), GREEN+COLOR_INTENSITY, RESET);
                NEWLINE_FUNC
            }
            num = (rand() % (MAXFACE) + MINFACE) + (rand() % (MAXFACE) + MINFACE);
        }
        printf("[%s]Il tuo numero: %i\n\n", players[turn], num);
        // verifico se uno dei due giocatori ha o meno ottenuto un valore più basso del precedente.
        if (num < comparators[turn]) {
            return !turn;
        }
        else {
            comparators[turn] = num;
            turn = !turn;
        }
        sleep(1);
    }
}

int dadi(playerArray *p) {
    // Chiamo la funzione gameplay_dadi passando come parametri i due giocatori selezionati al di fuori del gioco
    _Bool winner = gameplay_dadi(p);

    // Applico il controllo sui trucchi richiesto dalle specifiche e stampo il vincitore
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