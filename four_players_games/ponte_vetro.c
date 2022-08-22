//
// Created by marco on 10/05/2022.
//

#include "../headers/ponte_vetro.h"


// Questa funzione stampa la mappa com'è attualmente, con annessa la posizione del giocatore corrente, la quale viene
// indicata da una V che punta verso il quadrante su cui si trova il giocatore.

static void map(_Bool glasses[GLASS_LINES][GLASS_COLUMNS], int pos[MINDIM]) {
    NEWLINE_FUNC
    for (int i = GLASS_LINES-1; i > GARBAGE; i--) {
        if (pos != NULL && pos[0] == i) {
            switch (pos[1]) {
                // Sulla base della posizione determino su quale quadrante posizionare la V
                case 0: printf("\t\t\t\t\t  V\n");
                    break;
                case 1: printf("\t\t\t\t\t        V\n");
                    break;
            }
        }
        // Definisco due stampe, una per i sistemi windows una per i sistemi UNIX-like
#if defined WIN32 || defined WIN64
        printf("\t\t\t\t\t%c%c%c%c%c %c%c%c%c%c\n", SO_C_2, SO_C_1, SO_C_1, SO_C_1, SO_C_5, SO_C_2, SO_C_1, SO_C_1, SO_C_1, SO_C_5);
        if (glasses[i][0] == true) printf("\t\t\t\t\t%c%c%c%c%c ", SO_C_6, GLASS_PRESENCE, GLASS_PRESENCE, GLASS_PRESENCE, SO_C_6);
        else printf("\t\t\t\t\t%c   %c ", SO_C_6, SO_C_6);
        if (glasses[i][1] == true) printf("%c%c%c%c%c\n", SO_C_6, GLASS_PRESENCE, GLASS_PRESENCE, GLASS_PRESENCE, SO_C_6);
        else printf("%c   %c\n", SO_C_6, SO_C_6);
        printf("\t\t\t\t\t%c%c%c%c%c %c%c%c%c%c\n", SO_C_3, SO_C_1, SO_C_1, SO_C_1, SO_C_4, SO_C_3, SO_C_1, SO_C_1, SO_C_1, SO_C_4);
#elif defined __linux__ || defined __APPLE__
        printf("\t\t\t\t\t%s%s%s%s%s %s%s%s%s%s\n", SO_C_2, SO_C_1, SO_C_1, SO_C_1, SO_C_5, SO_C_2, SO_C_1, SO_C_1, SO_C_1, SO_C_5);
        if (glasses[i][0] == true) printf("\t\t\t\t\t%s%s%s%s%s ", SO_C_6, GLASS_PRESENCE, GLASS_PRESENCE, GLASS_PRESENCE, SO_C_6);
        else printf("\t\t\t\t\t%s   %s ", SO_C_6, SO_C_6);
        if (glasses[i][1] == true) printf("%s%s%s%s%s\n", SO_C_6, GLASS_PRESENCE, GLASS_PRESENCE, GLASS_PRESENCE, SO_C_6);
        else printf("%s   %s\n", SO_C_6, SO_C_6);
        printf("\t\t\t\t\t%s%s%s%s%s %s%s%s%s%s\n", SO_C_3, SO_C_1, SO_C_1, SO_C_1, SO_C_4, SO_C_3, SO_C_1, SO_C_1, SO_C_1, SO_C_4);
#endif
    }
    // Stampo la posizione di inizio nel caso la posizione del giocatore attuale sia NULL
    if (pos == NULL) {
        printf("\t\t\t\t\t     V\n");
        printf("\t\t\t\t\t  INIZIO\n");
    }
}

// Questa funzione determina la nuova posizione del giocatore che ha eseguito la mossa sulla base della sua posizione
// corrente e della direzione impostata.

static void currentPlay(char choice, int **pos) {
    switch(choice) {
        case LEFT: {
            // Se la posizione è NULL significa che il giocatore sta partendo dall'inizio.
            if ((*pos) == NULL) {
                (*pos) = (int*) calloc(MINDIM, sizeof(int));
                (*pos)[0] = (*pos)[1] = 0;
            }
            else {
                (*pos)[0]++;
                (*pos)[1] = 0;
            }
        }
            break;
        case RIGHT: {
            if ((*pos) == NULL) {
                (*pos) = (int*) calloc(MINDIM, sizeof(int));
                (*pos)[0] = 0;
                (*pos)[1] = 1;
            }
            else {
                (*pos)[0]++;
                (*pos)[1] = 1;
            }
        }
            break;
    }
}

static char *ponteVetro_Gameplay(playerList *list) {
    // Definisco la mappa iniziale di gioco
    _Bool glasses[GLASS_LINES][GLASS_COLUMNS] = {
            {true, true},
            {true, true},
            {true, true}
    };
    // Definisco una mappa supplementare che determina in quali posizioni i vetri sono rompibili e in quali no
    _Bool breakable[GLASS_LINES][GLASS_COLUMNS] = {
            {RANDOM_ACCESS, !breakable[0][0]},
            {RANDOM_ACCESS, !breakable[1][0]},
            {RANDOM_ACCESS, !breakable[MINDIM][0]}
    };
    int pNum = GROUP_MAX_SIZE;
    char *choice = NULL;
    while (true) {
        // Nel caso il giocatore sia stato eliminato resetto la lista e do al puntatore pName il nome del giocatore
        // successivo
        char *pName = listReader(list, true);
        int *pos = NULL;
        map(glasses, pos);
        _Bool alive = true;
        while (alive) {
            printf("%s seleziona in quale vetro saltare ( '<' sinistra / destra '>' )\n", pName);
            if (!checkIfBot(pName)) {
                cString(&choice, 1, true, CS_CON_PONTEDIVETROMOVES, UI_MESSAGE_V1, caller_);
            }
            else {
                ponte_bot(&choice, breakable, pos, glasses);
                printf(">>%s\n", choice);
                sleep(1);
            }
            // Determino la posizione sulla base della mossa del giocatore
            currentPlay(choice[0], &pos);
            // Se la posizione scelta dal giocatore è in corrispondenza di un vetro che può essere rotto allora il
            // giocatore viene eliminato dal gioco e il vetro viene aggiornato come rotto
            if (breakable[pos[0]][pos[1]]) {
                pNum--;
                glasses[pos[0]][pos[1]] = !breakable[pos[0]][pos[1]];
                map(glasses, pos);
                printf("Il giocatore %s e' caduto rompendo il vetro!\n", pName);
                listRemover(&list, pName);
                free(pos);
                pos = NULL;
                alive = false;
            }
            else {
                // altrimenti stampo la mappa con la posizione attuale del giocatore.
                map(glasses, pos);
            }
            // Se la posizione della riga è uguale a MINDIM, quindi il giocatore è arrivato alla fine allora vince e
            // taglia il traguardo
            if (pos != NULL && pos[0] == MINDIM) {
                freeIt(&choice);
                printf("Il traguardo e' stato tagliato!\n");
                return pName;
            }
        }
        pName = NULL;
        if (pNum == 1) {
            freeIt(&choice);
            return listReader(list, true);
        }
    }
}

int ponteVetro(playerArray four_players[]) {
    playerList *list = listCreator(four_players);
    char *winner = ponteVetro_Gameplay(list);
    int id;
    // Verifico se uno dei giocatori è Riccardo Scateni, altrimenti stampo il vincitore normale.
    for (int i = 0; i < GROUP_MAX_SIZE; i++) {
        if (ccLess_Comparison(four_players[i].name, winner)) {
            id = four_players[i].id;
        }
        if (ccLess_Comparison(FRONTMAN, four_players[i].name)) {
            winner_stamp(FRONTMAN);
            id = four_players[i].id;
            i = GROUP_MAX_SIZE;
        }
        else if (i == GROUP_MAX_SIZE-1) {
            winner_stamp(winner);
        }
    }
    freeList(list);
    listReader(NULL, true);
    return id;
}