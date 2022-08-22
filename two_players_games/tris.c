//
// Created by marco on 11/02/2022.
//

#include "../headers/tris.h"

static void map_Style(const int counter, const int *id) {
    const char x_o[] = {'X', 'O', ' '};
    const char column_letter = 'A';
    if (counter < ROWS) {
        if (counter - 1 == 1)
            printf("       %c    %c    %c  ", column_letter, column_letter+1, column_letter+2);
        else printf("                    ");
    }
    else {
        if (counter % ROWS == 1) {
            printf("  %i  ", (int)((float)counter*GRAPHIC_CA));
#if defined WIN32 || defined WIN64
            printf("%c ", SO_C_6);
            coloredText(charToString(x_o[id[0]]), (id[0] == 0) ? RED+COLOR_INTENSITY : BLUE, RESET);
            printf(" %c", SO_C_6);
            printf("%c ", SO_C_6);
            coloredText(charToString(x_o[id[1]]), (id[1] == 0) ? RED+COLOR_INTENSITY : BLUE, RESET);
            printf(" %c", SO_C_6);
            printf("%c ", SO_C_6);
            coloredText(charToString(x_o[id[MINDIM]]), (id[MINDIM] == 0) ? RED+COLOR_INTENSITY : BLUE, RESET);
            printf(" %c", SO_C_6);
#else
            printf("%s ", SO_C_6);
            coloredText(charToString(x_o[id[0]]), (id[0] == 0) ? RED+COLOR_INTENSITY : BLUE, RESET);
            printf(" %s", SO_C_6);
            printf("%s ", SO_C_6);
            coloredText(charToString(x_o[id[1]]), (id[1] == 0) ? RED+COLOR_INTENSITY : BLUE, RESET);
            printf(" %s", SO_C_6);
            printf("%s ", SO_C_6);
            coloredText(charToString(x_o[id[MINDIM]]), (id[MINDIM] == 0) ? RED+COLOR_INTENSITY : BLUE, RESET);
            printf(" %s", SO_C_6);
#endif
        }
        else {
            printf(FIVE_SPACES);
#if defined WIN32 || defined WIN64
            for (int i = 0; i < ROWS; i++) {
                if (counter % ROWS == 0)
                    printf("%c%c%c%c%c", SO_C_2, SO_C_1, SO_C_1, SO_C_1, SO_C_5);
                else
                    printf("%c%c%c%c%c", SO_C_3, SO_C_1, SO_C_1, SO_C_1, SO_C_4);
            }
#else
            for (int i = 0; i < ROWS; i++) {
                if (counter % ROWS == 0)
                    printf("%s%s%s%s%s", SO_C_2, SO_C_1, SO_C_1, SO_C_1, SO_C_5);
                else
                    printf("%s%s%s%s%s", SO_C_3, SO_C_1, SO_C_1, SO_C_1, SO_C_4);
            }
#endif
        }
    }
}

static void map_creator(const int map[ROWS][ROWS]) {
    for (int i = 0; i < ROWS*ROWS+ROWS; i++) {
        map_Style(i, map[(int)(i * GRAPHIC_CA - 1)]);
        NEWLINE_FUNC
    }
}

static _Bool getMove(int map[ROWS][ROWS], const _Bool player) {
    char *move = NULL;
    _Bool back;
    // Chiamo la cString per ottenere un input valido
    cString(&move, MINDIM, true, CS_CON_TRISALLOWEDINS, UI_MESSAGE_V2, caller_);
    for (int i = 0; i < MINDIM; i++) {
        // Effettuo un controllo sull'inserimento e verifico che siano presenti delle lettere
        if ((move[i] >= CTRIS_LOWER_CHAR && move[i] <= CTRIS_HIGHER_CHAR) || (move[i] >= TRIS_LOWER_CHAR && move[i] <= TRIS_HIGHER_CHAR)) {
            // Faccio in modo che se vi sono lettere minuscole vengano reimpostate a maiuscole
            if (move[i] >= TRIS_LOWER_CHAR && move[i] <= TRIS_HIGHER_CHAR) move[i] -= (int)SPACE_CHAR;
            // Mando la stringa mossa con la posizione della lettera alla funzione switch
            // sulla base della lettera viene selezionata la colonna.
            // Poi sulla base di un calcolo si determina la posizione del numero nella risposta dell'utente,
            // esempio: Il programma accetta come input sia A1 che 1A, questo perchè: se i, quindi la posizione della lettera
            // è in posizione 1 allora per determinare la posizione del numero basta fare DIMENSIONE MASSIMA (2) - 1 -
            // posizione della lettera, questo vale anche per la lettera in posizione 0.
            // In caso di due lettere senza il numero l'inserimento viene semplicemente fatto ripetere senza cambiare il turno.
            switch(move[i]) {
                case CTRIS_HIGHER_CHAR: {
                    if (map[numchecker(move[MINDIM - 1 - i]) - 1][MINDIM] == SPACE_ID) {
                        if (player == false) map[numchecker(move[MINDIM - 1 - i]) - 1][MINDIM] = 0;
                        else map[numchecker(move[MINDIM - 1 - i]) - 1][MINDIM] = 1;
                        freeIt(&move);
                        return true;
                    }
                    else {
                        freeIt(&move);
                        return false;
                    }
                }
                    break;
                case CTRIS_HIGHER_CHAR-1: {
                    if (map[numchecker(move[MINDIM - 1 - i]) - 1][1] == SPACE_ID) {
                        if (player == false) map[numchecker(move[MINDIM - 1 - i]) - 1][1] = 0;
                        else map[numchecker(move[MINDIM - 1 - i]) - 1][1] = 1;
                        freeIt(&move);
                        return true;
                    }
                    else {
                        freeIt(&move);
                        return false;
                    }
                }
                    break;
                default: {
                    if (map[numchecker(move[MINDIM - 1 - i]) - 1][0] == SPACE_ID) {
                        if (player == false) map[numchecker(move[MINDIM - 1 - i]) - 1][0] = 0;
                        else map[numchecker(move[MINDIM - 1 - i]) - 1][0] = 1;
                        freeIt(&move);
                        return true;
                    }
                    else {
                        freeIt(&move);
                        return false;
                    }
                }
                    break;
            }
        }
    }
}

// Funzione che controlla la situazione della partita e verifica se qualche sequenza da 3 simboli è stata completata,
// in caso affermativo la funzione ritorna come output l'id del giocatore attuale altrimenti ritorna il valore dello
// spazio (32)
static int game_rules(_Bool *ruler, int map[ROWS][ROWS]) {
    _Bool spacePresence = false;
    for (int i = 0; i < ROWS && spacePresence != true; i++) {
        for (int j = 0; j < ROWS && spacePresence != true; j++) {
            if (map[i][j] == SPACE_ID) spacePresence = true;
        }
    }
    const int x = 1, y = 1;
    //ROWS
    if (map[x-1][y] == map[x-1][y-1] && map[x-1][y] == map[x-1][y+1] && map[x-1][y] != SPACE_ID)  {
        *ruler = false;
        return map[x-1][y];
    }
    else if (map[x][y] == map[x][y-1] && map[x][y] == map[x][y+1] && map[x][y] != SPACE_ID)  {
        *ruler = false;
        return map[x][y];
    }
    else if (map[x+1][y] == map[x+1][y-1] && map[x+1][y-1] == map[x+1][y+1] && map[x+1][y-1] != SPACE_ID)  {
        *ruler = false;
        return map[x+1][y];
    }
    //COLUMNS
    // TOP = MID & MID = BOT
    else if (map[x-1][y-1] == map[x][y-1] && map[x][y-1] == map[x+1][y-1] && map[x][y-1] != SPACE_ID)  {
        *ruler = false;
        return map[x][y-1];
    }
    // TOP = MID & MID = BOT
    else if (map[x-1][y] == map[x][y] && map[x][y] == map[x+1][y] && map[x][y] != SPACE_ID)  {
        *ruler = false;
        return map[x][y];
    }
    else if (map[x-1][y+1] == map[x][y+1] && map[x][y+1] == map[x+1][y+1] && map[x][y+1] != SPACE_ID)  {
        *ruler = false;
        return map[x][y+1];
    }
    //DIAGONALS
    else if (map[x-1][y-1] == map[x][y] && map[x][y] == map[x+1][y+1] && map[x][y] != SPACE_ID)  {
        *ruler = false;
        return map[x][y];
    }
    else if (map[x+1][y-1] == map[x][y] && map[x][y] == map[x-1][y+1] && map[x][y] != SPACE_ID )  {
        *ruler = false;
        return map[x][y];
    }
    else if (spacePresence == true) {
        *ruler = true;
        return SPACE_ID;
    }
    else {
        for (int i = 0; i < ROWS; i++) for (int j = 0; j < ROWS; j++) map[i][j] = SPACE_ID;
        *ruler = true;
        return SPACE_ID;
    }
}

static void user_interface (playerArray *p, int *winner) {
    // Creo un doppio puntatore che contiene i nomi dei due giocatori, e li copio al suo interno.
    char **players = (char**) calloc(MINDIM, sizeof(char*));
    players[0] = (char*) calloc(strlen(p[0].name)+1, sizeof(char));
    players[1] = (char*) calloc(strlen(p[1].name)+1, sizeof(char));
    strcpy(players[0], p[0].name);
    strcpy(players[1], p[1].name);
    _Bool game_running = true;
    _Bool turn = rand()%MINDIM;
    _Bool valid = true;
    int win;
    int map[ROWS][ROWS];
    for (int i = 0; i < ROWS; i++) for (int j = 0; j < ROWS; j++) map[i][j] = SPACE_ID;
    while (game_running == true) {
        // Stampa la situazione attuale della mappa
        map_creator(map);
        if (!checkIfBot(players[turn])) {
            printf("[%s]Fai la tua mossa", players[turn]);
            // Ottengo la mossa effettuata dal giocatore
            valid = getMove(map, turn);
            // Se la mossa effettuata è valida allora il turno viene cambiato altrimenti no.
            if (valid == true) (turn == 0) ? turn++ : turn--;
        }
        else {
            // Se è il bot a svolgere il turno allora è la funzione bot a scambiare il turno.
            turn = tris_bot(map, players[turn], turn);
            valid = true;
        }
        if (valid == true) win = game_rules(&game_running, map);
    }
    map_creator(map);
    *winner = win;
}

int tris_game(playerArray *p) {
    int winner;
    user_interface(p, &winner);
    // Applico il controllo sul trucco "Frontman..."
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