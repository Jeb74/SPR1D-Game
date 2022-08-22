//
// Created by marco on 01/03/2022.
//

#include "../headers/impiccato.h"

//Apre un file e prende una frase a caso dalla lista
void game_setup(char **phrase, char **copy) {
    FILE *ptr = fopen(DEFAULT_DICTIONARY, FILE_READ);
    char *nump = (char*) calloc(NUMMAXLEN, sizeof(char));
    // Ottengo il numero di frasi
    fgets(nump,NUMMAXLEN,ptr);
    // trovo la prima corrispondenza dentro una stringa con uno dei caratteri presenti nella lista successiva e
    // rimuovo quella prima corrispondenza sostituendola con un terminatore di stringa
    nump[strcspn(nump, LINUXENDCHARS)] = ENDSTRING;
    // Converto il numero da stringa a carattere
    int maxfnum = xtdnumchecker(nump, false);
    free(nump);
    // Controllo che il numero di frasi sia minore o uguale al numero massimo di frasi disponibile, in caso contrario
    // il numero massimo di frasi viene impostato forzatamente al numero massimo di frasi.
    if (maxfnum > MAX_DICTIONARY_LENGTH) maxfnum = MAX_DICTIONARY_LENGTH;
    // determino randomicamente il numero della frase da ottenere come frase di gioco
    maxfnum = rand() % maxfnum + 1;
    // alloco nella frase principale un quantitativo di memoria pari alla macro contenente la massima lunghezza della
    // frase + 1
    *phrase = (char*) calloc(PHRASE_MAX_LENGTH+1, sizeof(char));
    // Ottengo la frase corrispondente all'indice determinato in precedenza.
    for (int i = 0; i < maxfnum; i++) {
        fgets(*phrase, PHRASE_MAX_LENGTH, ptr);
        (*phrase)[strcspn(*phrase, LINUXENDCHARS)] = ENDSTRING;
    }
    fclose(ptr);
    // Creo una copia della frase dove sostituisco tutte le lettere con dei '_'
    *copy = calloc(strlen(*phrase) + 1, sizeof(char));
    strcpy(*copy, *phrase);
    *phrase = (char*) realloc(*phrase, (strlen(*phrase)+1) * sizeof(char));
    for (int i = 0; i < strlen(*copy); i++) {
        if ((phrase[0][i] >= CA_CHAR && phrase[0][i] <= CZ_CHAR) || (phrase[0][i] >= A_CHAR && phrase[0][i] <= Z_CHAR)) {
            copy[0][i] = UNDERLINE;
        }
    }
}

// La funzione errorsCounter e la funzione bodycreator sono due funzioni che non coesistono nella stessa versione
// dell'impiccato. errorsCounter è una funzione che viene chiamata nel caso di una partita di tipo adapted all'impiccato,
// mentre bodycreator nel caso opposto.

// Questa funzione serve ad "avvisare" l'utente degli errori commessi dai bot e dall'utente stesso

static _Bool errorsCounter(int *errors, char *name) {
    (*errors)++;
    printf("[");
    coloredText(name, (*errors)+COLOR_INTENSITY, RESET);
    printf("] hai commesso un errore, sei a quota %i errori!\n", *errors);
    if ((*errors) == ADAPTEDV_ERRORS_LIMIT) return false;
    else return true;
}

/*
 * Questa funzione serve ad avvisare l'utente riguardo quanti errori mancano al completamento del corpo dell'impiccato
 */

static _Bool bodycreator(char *name) {
    static int bodyparts = 0;
    // Questa condizione serve a resettare il valore di bodyparts nel caso in cui la frase venga completata prima di
    // completare il corpo dell'impiccato.
    if (name == NULL) {
        bodyparts = 0;
        return false;
    }
    bodyparts++;
    switch (bodyparts) {
        case HEAD:
            coloredText("La testa e' stata aggiunta al corpo dell'impiccato!\n", BLUE + COLOR_INTENSITY, RESET);
            break;
        case NECK:
            coloredText("Il collo e' stato aggiunto al corpo dell'impiccato!\n", GREEN + COLOR_INTENSITY, RESET);
            break;
        case CHEST:
            coloredText("Il busto e' stato aggiunto al corpo dell'impiccato!\n", GREEN, RESET);
            break;
        case RIGHT_ARM:
            coloredText("Il braccio destro e' stato aggiunto al corpo dell'impiccato!\n", YELLOW + COLOR_INTENSITY,RESET);
            break;
        case LEFT_ARM:
            coloredText("Il braccio sinistro e' stato aggiunto al corpo dell'impiccato!\n",YELLOW + COLOR_INTENSITY, RESET);
            break;
        case LEFT_LEG:
            coloredText("La gamba sinistra e' stata aggiunta al corpo dell'impiccato!\n", RED + COLOR_INTENSITY,RESET);
            break;
        case RIGHT_LEG:
            coloredText("La gamba destra e' stata aggiunta al corpo dell'impiccato!\n", RED, RESET);
            break;
    }
    if (bodyparts == FULLBODY) {
        bodyparts = 0;
        NEWLINE_FUNC
        for (int i = 0; i < SO_FORMAT; i++) {
            coloredText(SPACE_STRING, WHITE, YELLOW);
        }
        NEWLINE_FUNC
        coloredText("Il corpo e' stato completato! L'impiccato e' morto. ", RED, RESET);
        coloredText(name, YELLOW, RESET);
        coloredText(" hai perso!\n", RED, RESET);
        for (int i = 0; i < SO_FORMAT; i++) {
            coloredText(SPACE_STRING, RESET, YELLOW);
        }
        NEWLINE_FUNC
        NEWLINE_FUNC
        return false;
    } else return true;
}

// Questa funzione restituisce un valore booleano in base al tipo di carattere inserito nella frase.

static _Bool listOfCharacters(char c) {
    switch (c) {
        case VIRGOLA: return true;
        case PUNTO: return true;
        case DUE_PUNTI: return true;
        case PUNTO_E_VIRGOLA: return true;
        case INTERROGATIVO: return true;
        case ESCLAMATIVO: return true;
        case SPAZIO: return true;
        case ACCENTO_VIRGOLETTA: return true;
    }
    return false;
}

// Questa funzione viene chiamata per controllare se i caratteri o le parole o l'intera frase inserita dall'utente è
// parte/presente d/nella frase principale

static void checkPresenceInPhrase(char **choice, char *phrase, char **copy, char *pName, _Bool *signal, _Bool classicGame) {
    match *matches = contains(*choice, phrase, false);
    // Se sono stati trovati dei match all'interno della stringa allora vengono effettuate le dovute analisi, altrimenti
    // la funzione passa a modificare il valore di signal a false.
    if ((*matches).success) {
        // Se la ricerca ha dato risultati positivi si effettua una verifica sulla dimensione dell'input dell'utente.
        // Se la dimensione è maggiore di 1 si da per scontato che sia una parola, in caso contrario la verifica
        // viene effettuata come singola lettera
        int **result = checkMatches(&matches);
        if (!(strlen(*choice) > 1)) {
            for (int i = 0, j = 0; i < result[0][0]; i++) {
                j = result[1][i];
                (*copy)[j] = phrase[j];
            }
        }
        else {
            _Bool match = false;
            for (int i = 0, j = 0; i < result[0][0]; i++) {
                j = result[1][i];
                // se la parola non è una sottostringa di un'altra parola essa viene trattata come normale parola e
                // viene sostituita in quanto tale, altrimenti nel caso lo sia essa viene sostituita all'interno della
                // parola principale, esempio:
                // Frase: supercalifragilistico... super.
                // se ad esempio l'utente inserisce la parola super, la prima parte di supercalifragilistico non verrà
                // visualizzata, ma verrà visualizzata solo la parola super prima del punto.
                // Se quella parola prima del punto non ci fosse stata invece sarebbe stata trovata la sottostringa super
                // che compone la superstringa "supercalifragilistico..."
                if ((j == 0 || listOfCharacters(phrase[j-1])) && (listOfCharacters(phrase[j + strlen(*choice)]) || j + strlen(*choice) == strlen(phrase))) {
                    match = true;
                    for (int k = 0; k < strlen(*choice); k++) {
                        (*copy)[k + j] = phrase[k + j];
                    }
                }
            }
            if (!match) {
                for (int i = 0; i < result[0][0]; i++) {
                    for (int j = 0; j < strlen(*choice); j++) {
                        (*copy)[j + result[1][i]] = phrase[j + result[1][i]];
                    }
                }
            }
        }
        free(result[1]);
        free(result[0]);
        free(result);
    }
    else {
        if (classicGame) *signal = bodycreator(pName);
        else *signal = false;
        free((*matches).indexes);
        free(matches);
    }
}

static char *impiccato_gameplay(playerList *list, _Bool classicGame) {
    int playersNum = GROUP_MAX_SIZE;
    // Array della frase da completare
    char *phrase = NULL;
    // Array della copia della frase che inizialmente è formato da _ che vengono riempiti ogni volta che viene trovata una lettera o una parola interna alla frase
    char *copy;
    // puntatore che salva la scelta dell'utente (frase / parola / lettera)
    char *choice = NULL;
    char *pName = NULL;
    int *fails = (int*) calloc(GROUP_MAX_SIZE, sizeof(int));
    int _switch = GARBAGE;
    while (true) {
        // setup della partita, con apertura del file di frasi e creazione dell'array con la frase e la copia censurata
        while (phrase == NULL || phrase[0] == ENDSTRING) {
            game_setup(&phrase, &copy);
        }
        // variabile booleana che determina se la partita corrente è finita o meno (più che partita si intende il round,
        // è infatti possibile che la frase non venga indovinata e che uno dei giocatori venga eliminato e rimosso dai partecipanti
        // della partita
        _Bool signal = true;
        // Nome del giocatore
        // Ciclo della partita
        _Bool full = false;
        char *tmp = NULL;
        while (signal && !full) {
            printf("La frase e':\n");
            coloredText(copy, YELLOW, RESET);
            NEWLINE_FUNC
            // Ottengo il nome del giocatore successivo al precedente, o del primo giocatore nel caso la partita sia
            // appena iniziata.
            pName = listReader(list, false);
            // Nel caso si sia arrivati sul fondo della lista a quel punto la lista viene resettata.
            if (pName == NULL) pName = listReader(list, true);
            printf("[%s] Inserisci una lettera, una parola o una frase\n", pName);
            if (!checkIfBot(pName)) {
                cString(&choice, FILENAME_MAX, false, CS_CON_IMPICCATOGAME, UI_MESSAGE_V1, caller_);
            }
            else {
                impiccato_bot(&choice);
                printf(">>%s\n", choice);
                // la funzione sleep ha il solo scopo di ritardare l'inserimento del bot per renderlo più simile ad un giocatore
                sleep(1);
            }
            NEWLINE_FUNC
            // Chiamo la funzione per ricercare la frase/parola/lettera inserita dall'utente o dal bot nella frase.
            checkPresenceInPhrase(&choice, phrase, &copy, pName, &signal, classicGame);
            // Nel caso in cui classicGame abbia come valore false (e quindi si stia giocando alla versione adattata)
            // si entra in un ulteriore controllo successivo alla checkPresenceInPhrase, nel quale si stabilisce (sulla
            // base del valore assunto da signal alla fine della checkPresence) se al giocatore del turno debba essere
            // assegnato un errore o meno, successivamente l'id della posizione dell'array numerico viene incrementata o
            // resettata in base al suo valore.
            if (!classicGame) {
                _switch++;
                if (_switch >= playersNum) _switch = 0;
                if (!signal) signal = errorsCounter(&(fails[_switch]), pName);
            }
            freeIt(&choice);
            // Verifico se la frase è stata completata sfruttando l'algoritmo di ricerca per verificare la presenza di '_'
            // nella copia della frase.
            match *matches = contains(UNDERLINE_STRING, copy, false);
            if (!(*matches).success) {
                free((*matches).indexes);
                free(matches);
                full = true;
            }
            else {
                free((*matches).indexes);
                free(matches);
            }
            if (!signal) {
                // se signal è rimasto false, e quindi i giocatori hanno commesso il massimo numero di errori disponibili
                // sia nel caso della versione classic (errori globali) che adapted (errori del singolo)
                // allora il numero dei giocatori viene decrementato e il giocatore corrente viene rimosso dal gioco.
                playersNum--;
                tmp = pName;
            }
            if (tmp != NULL) {
                // Qui avviene la rimozione del giocatore dal gioco nel caso la precedente condizione sia vera.
                listRemover(&list, tmp);
                tmp = NULL;
                // Se la partita è in adapted mode allora anche gli errori dei singoli giocatori vengono resettati.
                // E si ricomincia con una nuova frase e un giocatore in meno.
                if (!classicGame) {
                    free(fails);
                    fails = (int*) calloc(playersNum, sizeof(int));
                }
            }
        }
        printf("La frase era:\n%s\n", phrase);
        freeIt(&phrase);
        freeIt(&copy);
        freeIt(&choice);
        if (playersNum == 1) {
            // Se il numero dei giocatori si è ridotto a 1 allora la partita termina definitivamente
            // bodycreator viene resettata
            // il nome ad essere fornito come output è quello dell'ultimo giocatore rimasto.
            free(fails);
            bodycreator(NULL);
            pName = listReader(list, true);
            if (pName == NULL) pName = listReader(list, false);
            return pName;
        }
        else if (signal) {
            // nel caso la frase sia stata completata invece viene fornito il nome dell'ultimo giocatore che ha giocato
            // e sia il bodycreator che l'array di fallimenti dei giocatori vengono resettati, indipendentemente dalla
            // modalità di gioco.
            free(fails);
            bodycreator(NULL);
            return pName;
        }
        // Se nessuna delle precedenti condizioni è verificata allora signal viene resettata
        signal = true;
        // il nome del giocatore del prossimo turno viene riportato al primo giocatore della lista.
        pName = listReader(list, true);
    }
}

int impiccato(playerArray four_players[]) {
    playerList *list = listCreator(four_players);
    printf("Si vuole provare l'esperienza del classico \"Impiccato\" o della versione adattata? (classic/adapted)\n");
    _Bool breaks = false;
    char *choice = NULL;
    while (!breaks) {
        cString(&choice, IMPICCATO_TYPE_DIM, true, CS_CON_IMPICCATOMODE, UI_MESSAGE_V1, caller_);
        breaks = sString(&choice, SS_COM_IMPICCATOMODES, SS_COMM_NOCREMSPACES);
    }
    _Bool classicGame = word_comparison(CLASSIC_MODE, choice);
    char *winner = impiccato_gameplay(list, classicGame);
    int id;
    // In questo ciclo verifico che nessuno dei giocatori sia Riccardo Scateni, in caso contrario il vincitore sarà
    // direttamente lui e l'id in output sarà il suo.
    for (int i = 0; i < GROUP_MAX_SIZE; i++) {
        if (word_comparison(winner, four_players[i].name)) {
            id = four_players[i].id;
        }
        if (word_comparison(FRONTMAN, four_players[i].name)) {
            winner_stamp(FRONTMAN);
            id = four_players[i].id;
            i = GROUP_MAX_SIZE;
        }
        else if (i == GROUP_MAX_SIZE-1) {
            winner_stamp(winner);
        }
    }
    // libero tutta la memoria occupata dalla lista
    freeList(list);
    // resetto il puntatore della lista e lo risetto a NULL.
    listReader(NULL, true);
    return id;
}