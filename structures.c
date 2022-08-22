//
// Created by marco on 15/02/2022.
//
#include "headers/structures.h"

// Le funzioni seguenti sono purtroppo stato costretto a copiarle e incollarle anche solo per modificare pochi parametri,
// il motivo è che solo dopo aver effettuato alcuni test su linux ho potuto constatare che determinati caratteri
// necessitassero delle codifiche differenti, e di conseguenza invece di modificare interamente le funzioni ho preferito
// cambiarne alcuni elementi e utilizzare la compilazione condizionale.

#if defined WIN32 || defined WIN64

/*###############################################################################################
 *                                       WINNER MESSAGES
 *###############################################################################################
 */

// Questa funzione crea un array di size SO_FORMAT il quale contiene al suo interno il messaggio "Stai per giocare a "
// ed il nome del gioco.
// Mediante alcuni calcoli matematici cerco inoltre di centrare il messaggio all'interno dell'array.

static void fillGameStamp(char **gameName, const char name[]) {
    static const char default_Message[] = "Stai per giocare a ";
    int totAdd = (int)(strlen(default_Message) + strlen(name));
    int startIndex = SO_FORMAT/MINDIM - totAdd/MINDIM;
    if (totAdd % MINDIM != 0) startIndex--;
    int endIndex = startIndex + totAdd - 1;
    char *tmp = calloc(strlen(default_Message) + strlen(name) + 1, sizeof(char));
    tmp = strcat(strcpy(tmp, default_Message), name);
    gameName[0][SO_FORMAT - 1] = ENDSTRING;
    for (int i = 0; i < SO_FORMAT-MINDIM; i++) {
        gameName[0][i] = SPACE_CHAR;
        if (i >= startIndex && i <= endIndex) gameName[0][i] = tmp[i - startIndex];
    }
    free(tmp);
}

// Questa funzione stampa il nome del gioco e lo racchiude all'interno di un rettangolo di simboli appartenenti alla
// Extended ASCII

void gameTypeStamp(const char gameName[], games game) {
    NEWLINE_FUNC
    char *gameMessage = calloc(SO_FORMAT, sizeof(char));
    fillGameStamp(&gameMessage, gameName);
    for (int i = 0; i < SO_LINES; i++) {
        switch (i) {
            default: {
                for (int j = 0; j < SO_FORMAT; j++) {
                    if (j == 0) {
                        if (i == 0) coloredText(charToString((char)SO_C_2), YELLOW, RESET);
                        else coloredText(charToString((char)SO_C_3), YELLOW, RESET);
                    }
                    else if (j == SO_FORMAT-1) {
                        if (i == 0) coloredText(charToString((char)SO_C_5), YELLOW, RESET);
                        else coloredText(charToString((char)SO_C_4), YELLOW, RESET);
                        NEWLINE_FUNC
                    }
                    else coloredText(charToString((char)SO_C_1), YELLOW, RESET);
                }
            }
                break;
            case 1: {
                coloredText(charToString((char)SO_C_6), YELLOW, RESET);
                coloredText(gameMessage, game, RESET);
                coloredText(charToString((char)SO_C_6), YELLOW, RESET);
                NEWLINE_FUNC
                free(gameMessage);
            }
                break;
        }
    }
    NEWLINE_FUNC
}

/*###############################################################################################
 *                                       WINNER MESSAGES
 *###############################################################################################
 */

// Questa funzione svolge il medesimo ruolo della funzione fillGameStamp con la differenza che funziona con i nomi dei
// giocatori e cambia il messaggio.

static void fill(char **winMessage, const char name[]) {
    static const char default_Message[] = "Il vincitore e': ";
    int totAdd = (int)(strlen(default_Message) + strlen(name));
    int startIndex = SO_FORMAT/MINDIM - totAdd/MINDIM;
    if (totAdd % MINDIM != 0) startIndex--;
    int endIndex = startIndex + totAdd - 1;
    char *tmp = calloc(strlen(default_Message) + strlen(name) + 1, sizeof(char));
    tmp = strcat(strcpy(tmp, default_Message), name);
    winMessage[0][SO_FORMAT-1] = ENDSTRING;
    for (int i = 0; i < SO_FORMAT-MINDIM; i++) {
        winMessage[0][i] = SPACE_CHAR;
        if (i >= startIndex && i <= endIndex) winMessage[0][i] = tmp[i-startIndex];
    }
    free(tmp);
}

// Questa funzione svolge lo stesso lavoro della funzione gameTypeStamp, con la differenza che stampa i giocatori vincenti
void winner_stamp(const char name[]) {
    NEWLINE_FUNC
    char *winMessage = calloc(SO_FORMAT, sizeof(char));
    fill(&winMessage, name);
    for (int i = 0; i < SO_LINES; i++) {
        switch (i) {
            default: {
                for (int j = 0; j < SO_FORMAT; j++) {
                    if (j == 0) {
                        if (i == 0) coloredText(charToString((char)SO_C_2), YELLOW, RESET);
                        else coloredText(charToString((char)SO_C_3), YELLOW, RESET);
                    }
                    else if (j == SO_FORMAT-1) {
                        if (i == 0) coloredText(charToString((char)SO_C_5), YELLOW, RESET);
                        else coloredText(charToString((char)SO_C_4), YELLOW, RESET);
                        NEWLINE_FUNC
                    }
                    else coloredText(charToString((char)SO_C_1), YELLOW, RESET);
                }
            }
                break;
            case 1: {
                coloredText(charToString((char)SO_C_6), YELLOW, RESET);
                coloredText(winMessage, GREEN, RESET);
                coloredText(charToString((char)SO_C_6), YELLOW, RESET);
                NEWLINE_FUNC
                free(winMessage);
            }
                break;
        }
    }
    NEWLINE_FUNC
}
#elif defined __linux__ || defined __APPLE__

/*###############################################################################################
 *                                       WINNER MESSAGES
 *###############################################################################################
 */

static void fillGameStamp(char **gameName, const char name[]) {
    static const char default_Message[] = "Stai per giocare a ";
    int totAdd = (int)(strlen(default_Message) + strlen(name));
    int startIndex = SO_FORMAT/MINDIM - totAdd/MINDIM;
    if (totAdd % MINDIM != 0) startIndex--;
    int endIndex = startIndex + totAdd - 1;
    char *tmp = calloc(strlen(default_Message) + strlen(name) + 1, sizeof(char));
    tmp = strcat(strcpy(tmp, default_Message), name);
    gameName[0][SO_FORMAT - 1] = ENDSTRING;
    for (int i = 0; i < SO_FORMAT-MINDIM; i++) {
        gameName[0][i] = SPACE_CHAR;
        if (i >= startIndex && i <= endIndex) gameName[0][i] = tmp[i - startIndex];
    }
    free(tmp);
}

void gameTypeStamp(const char gameName[], games game) {
    NEWLINE_FUNC
    char *gameMessage = calloc(SO_FORMAT, sizeof(char));
    fillGameStamp(&gameMessage, gameName);
    for (int i = 0; i < SO_LINES; i++) {
        switch (i) {
            default: {
                for (int j = 0; j < SO_FORMAT; j++) {
                    if (j == 0) {
                        if (i == 0) coloredText(SO_C_2, YELLOW, RESET);
                        else coloredText(SO_C_3, YELLOW, RESET);
                    }
                    else if (j == SO_FORMAT-1) {
                        if (i == 0) coloredText(SO_C_5, YELLOW, RESET);
                        else coloredText(SO_C_4, YELLOW, RESET);
                        NEWLINE_FUNC
                    }
                    else coloredText(SO_C_1, YELLOW, RESET);
                }
            }
                break;
            case 1: {
                coloredText(SO_C_6, YELLOW, RESET);
                coloredText(gameMessage, game, RESET);
                coloredText(SO_C_6, YELLOW, RESET);
                NEWLINE_FUNC
                free(gameMessage);
            }
                break;
        }
    }
    NEWLINE_FUNC
}

/*###############################################################################################
 *                                       WINNER MESSAGES
 *###############################################################################################
 */

static void fill(char **winMessage, const char name[]) {
    static const char default_Message[] = "Il vincitore e': ";
    int totAdd = (int)(strlen(default_Message) + strlen(name));
    int startIndex = SO_FORMAT/MINDIM - totAdd/MINDIM;
    if (totAdd % MINDIM != 0) startIndex--;
    int endIndex = startIndex + totAdd - 1;
    char *tmp = calloc(strlen(default_Message) + strlen(name) + 1, sizeof(char));
    tmp = strcat(strcpy(tmp, default_Message), name);
    winMessage[0][SO_FORMAT-1] = ENDSTRING;
    for (int i = 0; i < SO_FORMAT-MINDIM; i++) {
        winMessage[0][i] = SPACE_CHAR;
        if (i >= startIndex && i <= endIndex) winMessage[0][i] = tmp[i-startIndex];
    }
    free(tmp);
}

void winner_stamp(const char name[]) {
    NEWLINE_FUNC
    char *winMessage = calloc(SO_FORMAT, sizeof(char));
    fill(&winMessage, name);
    for (int i = 0; i < SO_LINES; i++) {
        switch (i) {
            default: {
                for (int j = 0; j < SO_FORMAT; j++) {
                    if (j == 0) {
                        if (i == 0) coloredText(SO_C_2, YELLOW, RESET);
                        else coloredText(SO_C_3, YELLOW, RESET);
                    }
                    else if (j == SO_FORMAT-1) {
                        if (i == 0) coloredText(SO_C_5, YELLOW, RESET);
                        else coloredText(SO_C_4, YELLOW, RESET);
                        NEWLINE_FUNC
                    }
                    else coloredText(SO_C_1, YELLOW, RESET);
                }
            }
                break;
            case 1: {
                coloredText(SO_C_6, YELLOW, RESET);
                coloredText(winMessage, GREEN, RESET);
                coloredText(SO_C_6, YELLOW, RESET);
                NEWLINE_FUNC
                free(winMessage);
            }
                break;
        }
    }
    NEWLINE_FUNC
}
#endif

/*###############################################################################################
 *                                     PLAYER LIST MANAGING
 *###############################################################################################
 */
 // Questa funzione crea una lista di giocatori, e con lista si intende proprio una struttura dati lista e non un array,
 // e ogni elemento della lista è composto da 3 campi: campo primo consiste in un puntatore ad un elemento precedente,
 // o NULL nel caso l'elemento in questione sia il primo; il nome del giocatore, il quale viene preso dall'array di
 // giocatori passato come parametro; e il terzo campo contiene un puntatore all'elemento successivo, o NULL nel caso
 // l'elemento in questione sia l'ultimo della lista.
 // Il motivo dell'impiego di questa struttura dati è la facilità di rimozione dei giocatori che non devono più
 // partecipare ad un determinato gioco, aspetto fondamentale nell'IMPICCATO e nel PONTE DI VETRO.
playerList* listCreator(playerArray pArray[]) {
    playerList *top;
    playerList *list = NULL;
    for (int i = 0; i < GROUP_MAX_SIZE; i++) {
        playerList *tmp = (playerList*) calloc(1, sizeof(playerList));
        strcpy((*tmp).name, pArray[i].name);
        (*tmp).prev = list;
        (*tmp).next = NULL;
        if (list == NULL) {
            list = tmp;
            top = tmp;
        }
        else {
            (*list).next = tmp;
            list = (*list).next;
        }
    }
    return top;
}

// Questa funzione consente di ottenere un elemento di una lista di giocatori, essa restituisce non l'elemento stesso ma
// il nome contenuto nel campo name dell'elemento.

char* listReader(playerList *list, _Bool reset) {
    static playerList *this_List = NULL;
    if (this_List == NULL || reset) this_List = list;
    else this_List = this_List->next;
    return (this_List != NULL) ? (*this_List).name : NULL;
}

// Questa funzione rimuove un elemento dalla lista sulla base del nome passato.
// Oltre a rimuovere l'elemento sistema la lista in modo da evitare NULL Pointer o HEAP Corruption.
void listRemover(playerList **list, char *player) {
    playerList *tmp;
    while (true) {
        if (word_comparison((**list).name, player)) {
            if ((**list).prev != NULL) {
                tmp = (**list).prev;
                (*tmp).next = (**list).next;
                if ((**list).next != NULL) (*((**list).next)).prev = tmp;
                break;
            }
            else {
                tmp = (**list).next;
                (*tmp).prev = (**list).prev;
                if ((**list).prev != NULL) (*((**list).prev)).next = tmp;
                break;
            }
        }
        else *list = (**list).next;
    }
    while ((*tmp).prev != NULL) {
        tmp = (*tmp).prev;
    }
    *list = tmp;
}

// Questa funzione libera interamente la lista in questione.
void freeList(playerList *list) {
    while(list != NULL) {
        playerList *tmp = list;
        list = list->next;
        free(tmp);
    }
}

/*###############################################################################################
 *                                       PLAYER ARRAY MANAGING
 *###############################################################################################
 */

// Questa funzione genera un nome per un bot appartenente all'id dato.
static char *botNamer(int id) {
    char *num = numToString(id);
    char *name = (char*) calloc(strlen(BOTS_ROOT) + strlen(num)+1, sizeof(char));
    strcpy(name, BOTS_ROOT);
    strcat(name, num);
    free(num);
    return name;
}

// Questa funzione serve a determinare se il giocatore appartenente a quell'id è o meno un giocatore utente, in caso
// esso sia un bot gli viene generato un nome in base all'id.

static char *checkTypeOfPlayer(player *players, int id, const int listIds[], int numPlayers) {
    for (int i = 0; i < numPlayers; i++) {
        if (id == players[listIds[i]].id) {
            return players[listIds[i]].name;
        }
    }
    return botNamer(id+1);
}

// Questa funzione serve a determinare il numero di giocatori ancora vivi.
int checkNumAlive(const int *array, int dim) {
    int count = 0;
    for (int i = 0; i < dim; i++) {
        if (array[i] != false) {
            count++;
        }
    }
    return count;
}

// Questa funzione genera un doppio puntatore contenente un numero variabile di puntatori (in definitiva un array di
// array) i quali contengono gli id dei giocatori e i loro rispettivi nomi (CPU-(ID+1)) nel caso dei bot.
playerArray **arrayGenerator(int *array, player *players, int size, int dim, int numPlayers, int listIds[]) {
    int numGroups = checkNumAlive(array, dim)/size;
    playerArray **arrays = (playerArray**) calloc(numGroups, sizeof(playerArray*));
    // Questo array supplementare serve a tenere traccia degli ID precedentemente utilizzati in modo da non riusarli
    // in gruppi successivi.
    int *usedIds = (int*) calloc(numGroups*size, sizeof(int));
    for (int i = 0; i < numGroups*size; i++) {
        usedIds[i] = GARBAGE;
    }
    for (int i = 0, usedIdsElement = 0; i < numGroups; i++) {
        arrays[i] = (playerArray*) calloc(size, sizeof(playerArray));
        for (int j = 0, id; j < size;) {
            _Bool valid = true;
            id = rand()%dim;
            for (int k = 0; k < numGroups*size && valid; k++) {
                // Qui verifico sia se il giocatore è già stato inserito in qualche gruppo e sia se è già stato eliminato
                // in qualche fase precedente di gioco.
                if (id == usedIds[k] || array[id] == false) {
                    valid = false;
                }
            }
            if (valid) {
                usedIds[usedIdsElement] = id;
                usedIdsElement++;
                arrays[i][j].id = id;
                arrays[i][j].name = checkTypeOfPlayer(players, id, listIds, numPlayers);
                j++;
            }
        }
    }
    free(usedIds);
    return arrays;
}

/*###############################################################################################
 *                                       GAMEPLAY FUNCTIONS
 *###############################################################################################
 */

// Questa funzione serve a far giocare la partita al gruppo di giocatori passato come parametro.

static int playThisGame(playerArray *array, games _game) {
    const char *gamesName[] = {GAMES};
    switch (_game) {
        case DADI: {
            // La sezione game in realtà rappresenta il colore da assegnare ad ogni gioco, il problema è che il bianco
            // per il gioco dadi mi sembrava troppo triste e quindi ho deciso di cambiarlo.
            // Contemporaneamente non avevo voglia di assegnare un colore preciso ad ogni gioco, quindi ho semplicemente
            // preferito mantenere la logica iniziale per gli altri giochi.
            gameTypeStamp(gamesName[DADI], BLUE+COLOR_INTENSITY);
            return dadi(array);
        }
            break;
        case BIGLIE: {
            gameTypeStamp(gamesName[BIGLIE], _game);
            return biglie_game(array);
        }
            break;
        case MORRACINESE: {
            gameTypeStamp(gamesName[MORRACINESE], _game);
            return morraCinese(array);
        }
            break;
        case MURRA: {
            gameTypeStamp(gamesName[MURRA], _game);
            return murraGame(array);
        }
            break;
        case PARI_O_DISPARI: {
            gameTypeStamp(gamesName[PARI_O_DISPARI], _game);
            return pari_dispari(array);
        }
            break;
        case TRIS: {
            gameTypeStamp(gamesName[TRIS], _game);
            return tris_game(array);
        }
            break;
        case IMPICCATO: {
            gameTypeStamp(gamesName[IMPICCATO], _game);
            return impiccato(array);
        }
            break;
        case PONTE_DI_VETRO: {
            gameTypeStamp(gamesName[PONTE_DI_VETRO], _game);
            return ponteVetro(array);
        }
            break;
    }
}

// Questa funzione restituisce il vincitore di ogni gruppo di giocatori, il quale viene generato casualmente solo se
// non vi sono giocatori utente all'interno di esso, altrimenti viene generato giocando la partita del gioco prescelto.

int gameManagement(playerArray *arrays, int **playingPlayers, games _game) {
    _Bool playerPresence = false;
    int id = GARBAGE;
    int dim = (_game <= TRIS) ? MINDIM : GROUP_MAX_SIZE;
    for (int i = 0; i < dim; i++) {
        if (!checkIfBot(arrays[i].name)) {
            playerPresence = true;
        }
    }
    if (playerPresence) {
        id = playThisGame(arrays, _game);
    }
    else {
        id = arrays[rand()%dim].id;
    }
    for (int i = 0; i < dim; i++) {
        if (arrays[i].id != id) {
            (*playingPlayers)[arrays[i].id] = false;
        }
    }
    if (playerPresence) return id;
    else return GARBAGE;
}

// Questa funzione serve a incrementare le statistiche del singolo giocatore sulla base del tipo di obiettivo raggiunto:
// se il giocatore partecipa a una partita egli avrà come obiettivo: CLASSIC_GAME; se invece partecipa ad una finale:
// FINAL; se invece partecipa solo alla scrematura e non sopravvive allora avrà: SCREMATURA;
// Gli obiettivi raggiunti servono poi a determinare cosa bisogna controllare di un giocatore, ad esempio:
// se si partecipa a un minigioco (CLASSIC_GAME) i controlli che verranno effettuati riguarderanno: se il giocatore
// ha partecipato (naturalmente ha partecipato altrimenti non avrebbe preso l'obiettivo, quindi riceve un incremento
// dei giochi giocati, e se l'id del vincitore corrisponde al suo, in tal caso riceve un incremento anche dei giochi
// vinti. In definitiva il giocatore riceve un incremento alla statistica tgp (TOTAL GAMES PLAYED) e tgw (TOTAL GAMES WON).
// Questa stessa logica viene applicata anche alla finale, con la differenza che in quel caso specifico verrebbero
// incrementate anche le statistiche FINALS (Finali giocate) e WINS (SPR1D GAMES VINTI).
void increaseStats(playerArray *pArray, int size, stats type, int id) {
    player **players = getPlayers(NULL);
    int *pIds = getPlayersIds(NULL);
    int numPlayers = getNumOfActivePlayers(0);
    for (int i = 0; i < size; i++) {
        if (!checkIfBot(pArray[i].name)) {
            for (int j = 0; j < numPlayers; j++) {
                if (pArray[i].id == (*players)[pIds[j]].id) {
                    switch (type) {
                        case SURVIVOR_OF_SCREMATURA: {
                            (*players)[pIds[j]].played++;
                        }
                            break;
                        case CLASSIC_GAME: {
                            (*players)[pIds[j]].tgp++;
                            if (id != GARBAGE && (*players)[pIds[j]].id == id) (*players)[pIds[j]].tgw++;
                        }
                            break;
                        case FINAL: {
                            (*players)[pIds[j]].tgp++;
                            (*players)[pIds[j]].finals++;
                            if (id != GARBAGE && (*players)[pIds[j]].id == id) {
                                (*players)[pIds[j]].tgw++;
                                (*players)[pIds[j]].wins++;
                            }
                        }
                            break;
                    }
                }
            }
        }
    }
}

static int *isThereRichard(player *p, const int *pIds, int M) {
    for (int i = 0; i < M; i++) {
        if (word_comparison(p[pIds[i]].name, "Riccardo Scateni")) {
            int *yesHeIs = (int*) calloc(MINDIM, sizeof(int));
            yesHeIs[0] = 1;
            yesHeIs[1] = p[pIds[i]].id;
            return yesHeIs;
        }
    }
    return NULL;
}

static void decideWhatToPlay(games *game, int numOfPlayersAlive) {
    char *choice = NULL;
    _Bool valid = false;
    printf("Riccardo Scateni scegli pure in che modo sfidare i tuoi studenti\n");
    printf("Ma fai attenzione al numero, in molti hanno gia' deciso di abbandonare gli studi xD (Ne sono rimasti: %i)\n", numOfPlayersAlive);
    printf("1 -> DADI\n");
    printf("2 -> BIGLIE\n");
    printf("3 -> MORRACINESE\n");
    printf("4 -> MURRA\n");
    printf("5 -> PARI O DISPARI\n");
    printf("6 -> TRIS\n");
    if (numOfPlayersAlive / GROUP_MAX_SIZE >= MINDIM) {
        printf("7 -> IMPICCATO\n");
        printf("8 -> PONTE DI VETRO\n");
    }
    while (!valid) {
        cString(&choice, 1, false, CS_CON_GCOUNTLIMIT, UI_MESSAGE_V1, caller_);
        *game = xtdnumchecker(choice, false) - 1;
        if (*game < IMPICCATO || (*game >= IMPICCATO && numOfPlayersAlive / GROUP_MAX_SIZE >= MINDIM)) {
            valid = true;
        }
    }
}

// Questa funzione consente di giocare i vari minigiochi che compongono la partita di SPR1D GAME.
// Essa al suo interno raggruppa le funzioni di generazione dei gruppi di giocatori, la funzione per determinare se un
// gruppo è composto solo da bot e quindi generare casualmente il vincitore o se invece deve giocare una partita,
// la funzione per controllare se Riccardo Scateni fa parte dei giocatori, la funzione per far scegliere a
// Riccardo Scateni il prossimo gioco da giocare, e la funzione di salvataggio che richiede se si desidera salvare
// lo stato attuale della partita al termine di ogni minigioco.

void mainGames(int **pIds, player **p, int **alivePlayers, int N, int M, int k, int _fileElement) {
    int startingNum = checkNumAlive(*alivePlayers, N);
    games _game;
    playerArray **pArray;
    int *isThere = isThereRichard(*p, *pIds, M);
    while (startingNum != MINDIM) {
        if (isThere == NULL) {
            if (startingNum / GROUP_MAX_SIZE >= MINDIM) _game = rand() % (PONTE_DI_VETRO + 1);
            else _game = rand() % IMPICCATO;
        }
        else {
            decideWhatToPlay(&_game, startingNum);
        }
        int size = (_game < IMPICCATO) ? MINDIM : GROUP_MAX_SIZE;
        pArray = arrayGenerator(*alivePlayers, *p, size, N, M, *pIds);
        for (int i = ((startingNum/size)-1), id; i > GARBAGE; i--) {
            id = gameManagement(pArray[i], alivePlayers, _game);
            increaseStats(pArray[i], size, CLASSIC_GAME, id);
            for (int j = 0; j < size; j++) {
                if (checkIfBot(pArray[i][j].name)) free(pArray[i][j].name);
            }
            free(pArray[i]);
        }
        free(pArray);
        startingNum /= size;
        if (saveGame(k, p, _fileElement, true, N, M, pIds, alivePlayers)) exit(EXIT_SUCCESS);
    }
}

/*###############################################################################################
 *                                       GAME SETUP FUNCTION
 *###############################################################################################
 */

// Quando viene creato un nuovo salvataggio questa funzione viene chiamata per inserire i giocatori utente che faranno
// parte di quel salvataggio.
void optionsForNewSavings(int *k, player **p) {
    printf("Inserisci nuovi profili utente >>\n");
    _Bool breaks = false;
    char *name = NULL;
    *k = 0;
    while (!breaks) {
        do {
            printf("Inserire il nome del giocatore o 'stop' per terminare gli inserimenti\n");
            cString(&name, MAXNAME_LENGTH, false, CS_CON_PNAMEALLOWEDCHARS, UI_MESSAGE_V1, caller_);
            if (word_comparison(name, STOP_INSERT_PNAMES)) {
                if (*k < 1) coloredText("E' necessario inserire almeno un giocatore!\n", RED, RESET);
                else breaks = true;
            }
        } while(word_comparison(name, STOP_INSERT_PNAMES) && !breaks);
        if (!breaks) {
            _Bool valid = (strlen(name) >= PLAYER_NAME_MIN_LEN);
            for (int i = 0; i < *k && valid && *k > 1; i++) {
                valid = !(word_comparison(name, (*p)[i].name));
            }
            if (valid) {
                if (*p == NULL) {
                    *k = 1;
                    *p = (player *) calloc(*k, sizeof(player));
                } else {
                    (*k)++;
                    *p = (player *) realloc(*p, (*k) * sizeof(player));
                }
                (*p)[*k-1].id = -1;
                strcpy((*p)[*k-1].name, name);
                (*p)[*k-1].tgp = 0;
                (*p)[*k-1].tgw = 0;
                (*p)[*k-1].finals = 0;
                (*p)[*k-1].wins = 0;
                (*p)[*k-1].played = 0;
            }
            else {
                printf("Giocatore gia' inserito in precedenza o nome troppo corto!\n");
            }
        }
    }
}

// Questa funzione consente di ottenere l'array di giocatori utente ogni qualvolta sia necessario.
// Serve a evitare di passarlo come parametro.
player **getPlayers(player **players) {
    static player **p = NULL;
    if (players == NULL) {
        return p;
    }
    else {
        p = players;
        return NULL;
    }
}

// Questa funzione consente di ottenere l'array di ID dei giocatori utente partecipanti ogni qualvolta sia necessario.
// Serve a evitare di passarlo come parametro.
int *getPlayersIds(int *playersIds) {
    static int *ids = NULL;
    if (playersIds == NULL) {
        return ids;
    }
    else {
        ids = playersIds;
        return NULL;
    }
}

// Questa funzione consente di ottenere il numero di giocatori utente partecipanti ogni qualvolta sia necessario.
// Serve a evitare di passarlo come parametro.
int getNumOfActivePlayers(int playersNum) {
    static int num = 0;
    if (playersNum == 0) {
        return num;
    }
    else {
        num = playersNum;
        return 0;
    }
}

// Questa funzione verifica che l'id assegnato al giocatore appena selezionato come partecipante non sia già stato
// assegnato ad un altro giocatore

static _Bool checkIfAlreadyCreated(player *array, int id, int numPlayers) {
    for (int i = 0; i < numPlayers; i++) {
        if (id == array[i].id) return true;
    }
    return false;
}

// Questa funzione ha lo scopo di far definire all'utente le principali impostazioni della partita, dal numero di
// giocatori totali al numero di giocatori utente, il quale deve essere minore o uguale al numero di giocatori utente
// inseriti in precedenza.
void gameSetup(int numPlayers, player **p, int *numEntities, int *numPlayingPlayers, int **pIds, int **alivePlayers) {
    _Bool breaks = false;
    char *choice = NULL;
    NEWLINE_FUNC
    printf("\bQuanti giocatori giocheranno questa partita? (Utenti e Bot)\n");
    cString(&choice, DECIMAL_, false, CS_CON_PMINTOMAXNUM, UI_MESSAGE_V1, caller_);
    *numEntities = xtdnumchecker(choice, false);
    while (!breaks) {
        printf("Quanti giocatori giocheranno questa partita? (Utenti: %i)\n", numPlayers);
        cString(&choice, DECIMAL_, false, CS_CON_USERACCOUNTSPLAYING, UI_MESSAGE_V1, caller_);
        *numPlayingPlayers = xtdnumchecker(choice, false);
        if (*numPlayingPlayers <= numPlayers && *numPlayingPlayers <= *numEntities) breaks = true;
        else printf("Errore: Numero inserito troppo grande... Ritenta...\n");
    }
    for (int i = 0; i < numPlayers; i++) {
        printf("%i >> %s\n", i+1, (*p)[i].name);
    }
    *pIds = (int*) calloc(*numPlayingPlayers, sizeof(int));
    printf("Inserisci il numero corrispondente al giocatore da far giocare >>\n");
    for (int i = 0, j = 0; i < *numPlayingPlayers; i++) {
        cString(&choice, strlen(numToString(numPlayers)), false, CS_CON_USERACCOUNTSPLAYING, UI_MESSAGE_V1, caller_);
        if ((xtdnumchecker(choice, false) - 1) < numPlayers) {
            int id;
            do {
                id = (rand() % (*numEntities));
            } while (checkIfAlreadyCreated(*p, id, numPlayers));
            (*p)[(xtdnumchecker(choice, false) - 1)].id = id;
            (*pIds)[j] = (xtdnumchecker(choice, false) - 1);
            j++;
        }
        else i--;
    }
    *alivePlayers = (int*) calloc(*numEntities, sizeof(int));
    for (int i = 0; i < *numEntities; i++) {
        (*alivePlayers)[i] = true;
    }
}



