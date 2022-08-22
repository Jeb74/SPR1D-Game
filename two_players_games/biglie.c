//
// Created by marco on 20/02/2022.
//

#include "../headers/biglie.h"

/*
 *
 */

// Questa funzione implementa l'intero gameplay delle biglie e restituisce un valore booleano che determina
// il vincitore della partita
static _Bool gameplay_biglie(playerArray *p) {
    // Creo un doppio puntatore che contiene i nomi dei due giocatori, e li copio al suo interno.
    char **players = (char**) calloc(MINDIM, sizeof(char*));
    players[0] = (char*) calloc(strlen(p[0].name)+1, sizeof(char));
    players[1] = (char*) calloc(strlen(p[1].name)+1, sizeof(char));
    strcpy(players[0], p[0].name);
    strcpy(players[1], p[1].name);
    char *choice = NULL; // Creo un puntatore che diventerà il futuro array che conterrà le scelte dei giocatori
    int biglie[MINDIM] = {STARTING_POINTS, STARTING_POINTS}; // Definisco un array che contiene le biglie totali dei singoli giocatori
    int choices[MINDIM]; // Creo un array che conterrà le biglie scommesse dai singoli giocatori
    _Bool turn = rand()%MINDIM; // Variabile booleana da utilizzare come switch dei turni tra i due giocatori
    _Bool rWin;
    // Ciclo principale della partita che termina solo con la vittoria di uno dei giocatori
    while (true) {
        for (int i = 0; i < MINDIM; i++){// Informo il giocatore del totale di biglie da lui possedute
            printf("[%s]Scegli quante biglie scommettere, al momento hai un totale di %i Biglie\n", players[i], biglie[i]);
            // Ciclo di controllo dell'inserimento che si ripete finché l'utente non avrà inserito un valore corretto
            if (!checkIfBot(players[i])){
                _Bool valid = false;
                while (!valid) {
                    cString(&choice, MINDIM, false, CS_CON_BIGLIEMINMAXINPUT, UI_MESSAGE_V1, caller_);
                    if (xtdnumchecker(choice, false) <= biglie[i]) {
                        choices[i] = xtdnumchecker(choice, false);
                        valid = true;
                    }
                }
            }
            else {
                choices[i] = rand()%biglie[i]+1;
            }
        }
        printf("[%s]Secondo te il tuo avversario ha scommesso un numero di biglie pari o dispari?\n", players[turn]);
        if (!checkIfBot(players[turn])) {
            _Bool valid = false;
            while (!valid) {
                cString(&choice, BIGLIE_MAX_INPUT, false, CS_CON_BIGLIECHOICEALLOWEDCHARS, UI_MESSAGE_V1, caller_);
                valid = sString(&choice, SS_COM_PARIDISPARI, SS_COMM_NOCREMSPACES);
            }
        }
        else {
            double_bot(&choice);
            printf(">>%s\n", choice);
        }
        turn = !turn;
        if (word_comparison(choice, BPARI)) {
            rWin = (choices[turn] % MINDIM == 0) ? !turn : turn;
        }
        else {
            rWin = (choices[turn] % MINDIM != 0) ? !turn : turn;
        }
        printf("Questo round l'ha vinto ");
        coloredText(players[rWin], GREEN+COLOR_INTENSITY, RESET);
        NEWLINE_FUNC
        // stampa delle biglie rimaste a ciascun giocatore
        printf("[Perdente]Biglie di %s: %i - %i = %i\n", players[!rWin], (biglie[!rWin] + choices[!rWin]), choices[!rWin], biglie[!rWin]-=choices[!rWin]);
        printf("[Vincitore]Biglie di %s: %i + %i = %i\n", players[rWin], (biglie[rWin] - choices[!rWin]), choices[!rWin], biglie[rWin]+=choices[!rWin]);
        freeIt(&choice);
        if (biglie[0] == 0) {
            return true;
        }
        if (biglie[1] == 0) {
            return false;
        }
    }
}

int biglie_game(playerArray *p) {
    // Chiamo la funzione di gioco e ottengo il vincitore in forma di valore booleano
    _Bool winner = gameplay_biglie(p);

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