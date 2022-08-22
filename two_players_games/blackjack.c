//
// Created by marco on 23/05/2022.
//

#include "../headers/blackjack.h"

// Creo un hand ordinato con tutte le carte del mazzo ordinate in 4 colonne sulla base del seme.

static void createDeck(deck ***fullDeck) {
    *fullDeck = (deck**) calloc(SEEDS, sizeof(deck*));
    for (int i = 0; i < SEEDS; i++) {
        (*fullDeck)[i] = (deck*) calloc(CARDNUM, sizeof(deck));
        for (int j = 0; j < CARDNUM; j++) {
            (*fullDeck)[i][j].used = false;
            (*fullDeck)[i][j].card.seed = i;
            (*fullDeck)[i][j].card.value = j;
        }
    }
}

// Questa funzione serve ad assegnare al giocatore le carte.

static void giveCards(bjPlayer *player, int *playerCards) {
    static deck **fullDeck = NULL;
    // Il hand viene creato una sola volta, e dura per tutta la partita, tuttavia se il giocatore passato è NULL
    // il hand viene liberato e resettato.
    if (fullDeck == NULL) createDeck(&fullDeck);
    if (player == NULL) {
        free(fullDeck[0]);
        free(fullDeck[1]);
        free(fullDeck[2]);
        free(fullDeck[3]);
        free(fullDeck);
        fullDeck = NULL;
        return;
    }
    int cardNum = 1;
    if (*playerCards == 0) {
        (*player).hand = (cards*) calloc(MINDIM, sizeof(cards));
        cardNum = MINDIM;
    }
    else {
        (*player).hand = (cards*) realloc((*player).hand, sizeof(cards)*((*playerCards)+1));
    }
    for (int i = 0, seed = 0, value = 0; i < cardNum; i++) {
        seed = rand()%SEEDS;
        value = rand()%CARDNUM;
        if (!fullDeck[seed][value].used) {
            (*player).hand[*playerCards] = fullDeck[seed][value].card;
            fullDeck[seed][value].used = true;
            (*playerCards)++;
        }
        else {
            i--;
        }
    }
}

// Questa funzione calcola il valore della mano del giocatore e lo restituisce

static int cardSum(bjPlayer *player, int playerCards) {
    int aceCount = 0;
    int SUM = 0;
    for (int i = 0; i < playerCards; i++) {
        if ((*player).hand[i].value == ACE) {
            aceCount++;
        }
        if ((*player).hand[i].value >= ACE && (*player).hand[i].value <= TEN) {
            SUM = SUM + ((*player).hand[i].value) + 1;
        }
        else {
            SUM += ACE_SEC_VALUE;
        }
    }
    if (SUM + ACE_SEC_VALUE <= BLACKJACK && aceCount > 0) {
        SUM += ACE_SEC_VALUE;
    }
    return SUM;
}

// Questa funzione stampa la parte superiore della carta contenente il valore spostato a sinistra

static void printTopValue(bjPlayer *player, int playerCards) {
    char *top = (char*) calloc(CARDDIM, sizeof(char));
    for (int i = 0; i < CARDDIM; i++) {
        top[i] = SPACE_CHAR;
    }
    for (int i = 0; i < playerCards; i++) {
        // Con gli switch in base al valore effettuo delle conversioni della stringa che sto per stampare
        switch((*player).hand[i].value) {
            default: {
                top[1] = (*player).hand[i].value + ASCII_CODE_1;
                top[MINDIM] = SPACE_CHAR;
            }
                break;
            case TEN: {
                top[1] = ASCII_CODE_1;
                top[MINDIM] = ASCII_CODE_1 - 1;
            }
                break;
            case JACK: {
                top[1] = SJACK;
                top[MINDIM] = SPACE_CHAR;
            }
                break;
            case QUEEN: {
                top[1] = SQUEEN;
                top[MINDIM] = SPACE_CHAR;
            }
                break;
            case KING: {
                top[1] = SKING;
                top[MINDIM] = SPACE_CHAR;
            }
                break;
            case ACE: {
                top[1] = SACE;
                top[MINDIM] = SPACE_CHAR;
            }
                break;
        }
        switch((*player).hand[i].seed) {
            case SPADES:
                coloredText(top, BLACK, WHITE+COLOR_INTENSITY);
                coloredText(SPADE, WHITE+COLOR_INTENSITY, WHITE+COLOR_INTENSITY);
                break;
            case HEARTS:
                coloredText(top, RED+COLOR_INTENSITY, WHITE+COLOR_INTENSITY);
                coloredText(SPADE, WHITE+COLOR_INTENSITY, WHITE+COLOR_INTENSITY);
                break;
            case DIAMONDS:
                coloredText(top, RED+COLOR_INTENSITY, WHITE+COLOR_INTENSITY);
                coloredText(SPADE, WHITE+COLOR_INTENSITY, WHITE+COLOR_INTENSITY);
                break;
            case CLUBS:
                coloredText(top, BLACK, WHITE+COLOR_INTENSITY);
                coloredText(SPADE, WHITE+COLOR_INTENSITY, WHITE+COLOR_INTENSITY);
                break;
        }
        printf(SPACE_STRING);
    }
    NEWLINE_FUNC
    free(top);
}

// Questa funzione stampa la parte centrale della carta contenente il seme posizionato al centro

static void printMiddleSeed(bjPlayer *player, int playerCards) {
    for (int i = 0 ; i < playerCards; i++) {
        switch ((*player).hand[i].seed) {
            case SPADES:
                coloredText(THREE_WHITE_SPACES, WHITE, WHITE + COLOR_INTENSITY);
                coloredText(SPADE, BLACK, WHITE + COLOR_INTENSITY);
                coloredText(THREE_WHITE_SPACES, WHITE, WHITE + COLOR_INTENSITY);
                break;
            case HEARTS:
                coloredText(THREE_WHITE_SPACES, WHITE, WHITE + COLOR_INTENSITY);
                coloredText(HEART, RED + COLOR_INTENSITY, WHITE + COLOR_INTENSITY);
                coloredText(THREE_WHITE_SPACES, WHITE, WHITE + COLOR_INTENSITY);
                break;
            case DIAMONDS:
                coloredText(THREE_WHITE_SPACES, WHITE, WHITE + COLOR_INTENSITY);
                coloredText(DIAMOND, RED + COLOR_INTENSITY, WHITE + COLOR_INTENSITY);
                coloredText(THREE_WHITE_SPACES, WHITE, WHITE + COLOR_INTENSITY);
                break;
            case CLUBS:
                coloredText(THREE_WHITE_SPACES, WHITE, WHITE + COLOR_INTENSITY);
                coloredText(CLUB, BLACK, WHITE + COLOR_INTENSITY);
                coloredText(THREE_WHITE_SPACES, WHITE, WHITE + COLOR_INTENSITY);
                break;
        }
        printf(SPACE_STRING);
    }
    NEWLINE_FUNC
}

// Questa funzione stampa il valore della carta in basso posizionato a destra

static void printBottomValue(bjPlayer *player, int playerCards) {
    char *bottom = (char*) calloc(CARDDIM, sizeof(char));
    for (int i = 0; i < CARDDIM; i++) {
        bottom[i] = SPACE_CHAR;
    }
    for (int i = 0; i < playerCards; i++) {
        switch((*player).hand[i].value) {
            default: {
                bottom[TARGET_BEF] = SPACE_CHAR;
                bottom[TARGET_BEFV] = (*player).hand[i].value + ASCII_CODE_1;
            }
                break;
            case TEN: {
                bottom[TARGET_BEF] = ASCII_CODE_1;
                bottom[TARGET_BEFV] = ASCII_CODE_1 - 1;
            }
                break;
            case JACK: {
                bottom[TARGET_BEF] = SPACE_CHAR;
                bottom[TARGET_BEFV] = SJACK;
            }
                break;
            case QUEEN: {
                bottom[TARGET_BEF] = SPACE_CHAR;
                bottom[TARGET_BEFV] = SQUEEN;
            }
                break;
            case KING: {
                bottom[TARGET_BEF] = SPACE_CHAR;
                bottom[TARGET_BEFV] = SKING;
            }
                break;
            case ACE: {
                bottom[TARGET_BEF] = SPACE_CHAR;
                bottom[TARGET_BEFV] = SACE;
            }
                break;
        }
        switch((*player).hand[i].seed) {
            case SPADES:
                coloredText(bottom, BLACK, WHITE + COLOR_INTENSITY);
                coloredText(SPADE, WHITE+COLOR_INTENSITY, WHITE+COLOR_INTENSITY);
                break;
            case HEARTS:
                coloredText(bottom, RED + COLOR_INTENSITY, WHITE + COLOR_INTENSITY);
                coloredText(SPADE, WHITE+COLOR_INTENSITY, WHITE+COLOR_INTENSITY);
                break;
            case DIAMONDS:
                coloredText(bottom, RED + COLOR_INTENSITY, WHITE + COLOR_INTENSITY);
                coloredText(SPADE, WHITE+COLOR_INTENSITY, WHITE+COLOR_INTENSITY);
                break;
            case CLUBS:
                coloredText(bottom, BLACK, WHITE + COLOR_INTENSITY);
                coloredText(SPADE, WHITE+COLOR_INTENSITY, WHITE+COLOR_INTENSITY);
                break;
        }
        printf(SPACE_STRING);
    }
    free(bottom);
}

// Questa funzione mostra al singolo giocatore che gli viene passato la situazione della sua mano.

static void displayCardsFor(bjPlayer *player, int playerCards) {
    printTopValue(player, playerCards);
    printMiddleSeed(player, playerCards);
    printBottomValue(player, playerCards);
    NEWLINE_FUNC
    printf("[%s] Queste sono le tue carte ^^^\n", (*player).name);
    printf("Valore attuale della tua mano: ");
    int cardSumNum = cardSum(player, playerCards);
    char *sum = numToString(cardSumNum);
    if (cardSumNum > BLACKJACK_SAFE_SCORE) coloredText(sum, YELLOW + COLOR_INTENSITY, RESET);
    else if (cardSumNum > BLACKJACK_SAFE_SCORE-SAFERANGE_HIGHER) coloredText(sum, YELLOW, RESET);
    else if (cardSumNum > BLACKJACK_SAFE_SCORE-SAFERANGE_LOWER) coloredText(sum, GREEN, RESET);
    else coloredText(sum, RED+COLOR_INTENSITY, RESET);
    NEWLINE_FUNC
    free(sum);
}

static _Bool gamePlay_BlackJack(playerArray *p) {
    // Creo una nuova struttura contenente i due giocatori, questa struttura contiene:
    // un campo dedicato al nome del giocatore, ed un campo dedicato alle carte che ha in mano.
    bjPlayer players[MINDIM] = {
            {NULL, p[0].name},
            {NULL, p[1].name}
    };
    int playerCards[MINDIM] = {0, 0};
    char *choice = NULL;
    int *pPoints = (int*) calloc(MINDIM, sizeof(int));
    while (true) {
        giveCards(&(players[0]), &(playerCards[0]));
        giveCards(&(players[1]), &(playerCards[1]));
        _Bool switch_ = rand()%MINDIM;
        _Bool breaks = false;
        int i = 0;
        while (i < MINDIM && !breaks) {
            // Stampo la situazione del giocatore 1 e 2
            displayCardsFor(&(players[0]),playerCards[0]);
            displayCardsFor(&(players[1]),playerCards[1]);
            // Assegno i punteggi di ciascuna mano alla cella di memoria corrispondente al giocatore corretto.
            pPoints[0] = cardSum((&players[0]), playerCards[0]);
            pPoints[1] = cardSum((&players[1]), playerCards[1]);
            if (pPoints[0] > BLACKJACK) {
                breaks = true;
            }
            if (pPoints[1] > BLACKJACK) {
                breaks = true;
            }
            if (!breaks) {
                printf("\n[");
                coloredText(players[switch_].name, GREEN + COLOR_INTENSITY, RESET);
                printf("] Vuoi restare o pescare? (resto/pesco)\n");
                if (!checkIfBot(players[switch_].name)) {
                    _Bool valid = false;
                    while (!valid) {
                        cString(&choice, BOID, true, CS_CON_BLACKJACKCHOICES, UI_MESSAGE_V1, caller_);
                        valid = sString(&choice, SS_COM_BLACKJACK, SS_COMM_NOCREMSPACES);
                    }
                } else {
                    blackjack_bot(&choice, pPoints, switch_);
                    printf(">>%s\n", choice);
                    sleep(1);
                }
                if (word_comparison(choice, FIRST_BJ_MOVE)) {
                    giveCards(&(players[switch_]), &(playerCards[switch_]));
                } else {
                    switch_ = !switch_;
                    i++;
                }
                NEWLINE_FUNC
            }
        }
        pPoints[0] = cardSum((&players[0]), playerCards[0]);
        pPoints[1] = cardSum((&players[1]), playerCards[1]);
        free(players[0].hand);
        free(players[1].hand);
        giveCards(NULL, 0);
        playerCards[0] = 0;
        playerCards[1] = 0;
        if (pPoints[0] == pPoints[1]) {
        }
        else {
            free(choice);
            if (pPoints[0] > BLACKJACK) {
                free(pPoints);
                return true;
            }
            else if (pPoints[1] > BLACKJACK) {
                free(pPoints);
                return false;
            }
            _Bool result = (pPoints[0] > pPoints[1]) ? false : true;
            free(pPoints);
            return result;
        }
    }
}

int blackjack(playerArray *p) {
    // Chiamo la funzione gameplay_blackjack passando come parametri i due giocatori selezionati al di fuori del gioco
    _Bool winner = gamePlay_BlackJack(p);

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