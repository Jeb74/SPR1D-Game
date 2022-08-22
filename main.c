#include "headers/structures.h"

static void lastGameOfTheGame(playerArray **pArray);

/*
 * TODO: commentare il structures
 */

int main(int argc, char *argv[]) {
    // Imposto il seed randomico
    srand(time(NULL)*rand());
    // Dichiaro il puntatore di giocatori da utilizzare nelle varie fasi di gioco
    player *p;
    // Dichiaro variabili e puntatori utili
    // fileElement è una variabile che consente di tenere traccia del file di salvataggio utilizzato per caricare o per creare una nuova partita
    int k, _fileElement, N = GARBAGE, M = 0, *pId = NULL, *alivePlayers;
    // Dichiaro il puntatore di tipo playerArray che andrà a caricare i nomi e gli id di giocatori e bot
    playerArray **pArray;
    // Chiamo la funzione mainFileManager e delego la scelta dell'apertura o creazione del file alla medesima funzione
    int inGame = mainFileManager(&k, &p, &_fileElement, &N, &M, &pId, &alivePlayers);
    // Se p è NULL significa che i giocatori non sono stati caricati e di conseguenza il file è stato creato da 0
    if (p == NULL) {
        optionsForNewSavings(&k, &p);
        if (saveGame(k, &p, _fileElement, inGame, N, M, &pId, &alivePlayers)) exit(EXIT_SUCCESS);
    }
    // Controllo se la partita è già in corso, nel caso non lo sia faccio eseguire il setup di base per una partita
    if (!inGame) gameSetup(k, &p, &N, &M, &pId, &alivePlayers);
    // Ottengo il numero di giocatori non eliminati e attraverso un check dell'array alivePlayers
    // Il motivo di ciò è che il numero di giocatori non per forza è pari ad N, infatti se il salvataggio è stato caricato
    // e non creato da 0 il numero di giocatori potrebbe essere minore.
    int alive = checkNumAlive(alivePlayers, N);
    // Creo un array di giocatori (Sia Utente che Bot) con annesso nome e id.
    pArray = arrayGenerator(alivePlayers, p, alive, N, M, pId);
    // Carico all'interno di tre funzioni con parametro static il numero di giocatori attivi,
    // l'array di giocatori e l'array di id dei giocatori che stanno partecipando.
    getNumOfActivePlayers(M);
    getPlayers(&p);
    getPlayersIds(pId);
    // Verifico che la partita non sia in corso e che il numero di giocatori sia strettamente maggiore o uguale a 16,
    // e in caso entrambe le condizioni siano verificate si inizia la fase di scrematura
    if (!inGame && alive >= MINPLAYER) {
        gameTypeStamp(SCREMATURA, RED);
        scrematura(&alivePlayers, N, *pArray);
    }
    // Successivamente per tutti i sopravvissuti (Utenti) alla scrematura viene incrementata la statistica degli SPR1D giocati
    increaseStats(*pArray, alive, SURVIVOR_OF_SCREMATURA, GARBAGE);
    inGame = true;
    // Ora viene liberata la memoria dell'array di giocatori precedentemente creato (Utenti e Bot)
    for (int i = 0; i < alive; i++) {
        if (checkIfBot(pArray[0][i].name)) {
            free(pArray[0][i].name);
        }
    }
    free(pArray[0]);
    free(pArray);
    // Verifico la correttezza del salvataggio per evitare che venga caricato un file di salvataggio corrotto
    _Bool valid = false;
    alive = checkNumAlive(alivePlayers, N);
    for (int i = 0; i < N && !valid; i++) {
        if (pow(MINDIM, i) == alive) valid = true;
        else if (pow(MINDIM, i) > alive) i = N;
    }
    if (valid == false) {
        printf("Salvataggio non valido! Si prega di rimuovere...\n");
        exit(EXIT_FAILURE);
    }
    // Chiedo nuovamente all'utente se intende salvare la partita oppure no.
    if (saveGame(k, &p, _fileElement, inGame, N, M, &pId, &alivePlayers)) exit(EXIT_SUCCESS);
    // Se i giocatori rimasti sono più di 2 allora la funzione mainGames farà svolgere i giochi che compongono lo SPR1D game
    if (alive > MINDIM) mainGames(&pId, &p, &alivePlayers, N, M, k, _fileElement);
    // creo l'array di giocatori finale formato da sole due posizioni
    pArray = arrayGenerator(alivePlayers, p, MINDIM, N, M, pId);
    // faccio giocare l'ultima partita del gioco, il blackjack e successivamente una volta liberata la memoria
    // dell'array di giocatori chiedo all'utente se intende salvare la partita
    lastGameOfTheGame(pArray);
    free(pArray[0]);
    free(pArray);
    // Questa funzione viene chiamata per chiedere all'utente se intende salvare la partita una volta definitivamente terminata.
    saveGame(k, &p, _fileElement, !inGame, N, M, &pId, &alivePlayers);
    // Resetto l'ultima lista di condizioni della cString
    cString(NULL, 0, false, RESET_CONDITION, NULL, caller_);
}

static void lastGameOfTheGame(playerArray **pArray) {
    // Controllo inizialmente che tra gli ultimi due giocatori vi sia almeno un Utente,
    // se così non fosse allora il vincitore viene generato randomicamente
    _Bool playerPresence = false;
    for (int i = 0; i < MINDIM; i++) {
        if (!checkIfBot(pArray[0][i].name)) playerPresence = true;
    }
    if (playerPresence) {
        // stampo il nome del gioco
        gameTypeStamp(BLACKJACK_NAME, GREEN+COLOR_INTENSITY);
        int id = blackjack(*pArray);
        // aumento le statistiche del giocatore vincitore o perdente nel caso peggiore
        increaseStats(*pArray, MINDIM, FINAL, id);
    }
    else winner_stamp(pArray[0][rand()%MINDIM].name);
    // successivamente controllo libero la memoria occupata dal nome del giocatore che non era un utente
    for (int i = 0; i < MINDIM; i++) {
        if (checkIfBot(pArray[0][i].name)) {
            free(pArray[0][i].name);
        }
    }
}

