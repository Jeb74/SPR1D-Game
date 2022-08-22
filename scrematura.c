//
// Created by marco on 21/02/2022.
//

#include "headers/scrematura.h"

// Con questa funzione viene calcolato la potenza di inferiore a quella più vicina e strettamente minore del
// numero di giocatori totali definiti all'inizio della partita

static int getLowestPow(int dim) {
    for (int i = 0; i < dim; i++) {
        if (pow(MINDIM, i) > dim) {
            return i-MINDIM;
        }
    }
}

// Con questa funzione calcolo qual'è la dimensione dei gruppi che devono essere creati e quanti debbano esserne creati di ogni quantità

static int **getGroupsDivision(int dim) {
    // chiamo la funzione per ottenere la potenza di due giusta
    int lowestPow = getLowestPow(dim);
    // alloco dinamicamente la matrice contenente nel primo array la dimensione dei gruppi e nel secondo la quantità
    // per ogni gruppo
    int **groups = (int**) calloc(MINDIM, sizeof(int*));
    groups[0] = (int*) calloc(MINDIM, sizeof(int));
    groups[1] = (int*) calloc(MINDIM, sizeof(int));
    _Bool calculate = true;
    groups[1][0] = 0;
    groups[1][1] = 0;

    groups[0][0] = 0;
    groups[0][1] = 1;
    // creo un ciclo for dove aumento le dimensioni dei gruppi se la ricerca della combinazione perfetta fallisce per le ricerche precedenti
    do {
        // con questo ciclo for incremento il numero dei gruppi che vanno creati per ogni dimensione e ricerco la combinazione perfetta
        for (int i = 0, j = pow(MINDIM, lowestPow), sum = 0, mul = 0; i <= pow(MINDIM, lowestPow) && j >= 0; i++, j--) {
            groups[1][0] = i;
            groups[1][1] = j;
            sum = groups[1][0] + groups[1][1];
            mul = groups[0][0] * groups[1][0] + groups[0][1] * groups[1][1];
            // La combinazione perfetta è data da un semplice calcolo: se la somma del numero dei gruppi di entrambe le dimensioni
            // è uguale al risultato di 2 elevato alla potenza ricavata precedentemente, mentre la moltiplicazione del numero dei gruppi per la
            // rispettiva dimensione deve essere uguale al numero di giocatori totali partecipanti al gioco.
            if (sum == pow(MINDIM, lowestPow) && mul == dim) {
                calculate = false;
                j = GARBAGE;
            }
        }
        if (calculate) {
            groups[0][0]++;
            groups[0][1]++;
        }
    } while (calculate);
    return groups;
}

// Questa funzione ha il solo scopo di stampare quanto i giocatori si avvicinano
// al valore da indovinare

static void distanceFromObjective(int position, int objective) {
    if (position < objective - ACQUISSIMA_CAP || position > objective + ACQUISSIMA_CAP) coloredText("Acquissima!\n", BLUE+COLOR_INTENSITY, RESET);
    else if (position < objective - ACQUA_AND_FUOCO_CAP || position > objective + ACQUA_AND_FUOCO_CAP) coloredText("Acqua!\n", BLUE, RESET);
    else if (position >= objective - FUOCHISSIMO_CAP && position <= objective + FUOCHISSIMO_CAP) coloredText("Fuochissimo!\n", RED, RESET);
    else if (position >= objective - ACQUA_AND_FUOCO_CAP && position <= objective + ACQUA_AND_FUOCO_CAP) coloredText("Fuoco!\n", RED+COLOR_INTENSITY, RESET);
}

// Questa funzione serve a far giocare un gruppo di dimensione variabile di giocatori
// al gioco "indovina il numero"

static playerArray guessNumber(playerArray *p, int arrayDim) {
    int number = rand()%HIGHER_NUM_LIMIT;
    int try = 0;
    playerArray winner;
    char *choice = NULL;
    printf("%i\n", number);
    for (int i = 0; i < arrayDim; i++) {
        printf("[%s]Prova ad indovinare il numero!\n", p[i].name);
        if (!checkIfBot(p[i].name)) {
            cString(&choice, SCREMATURA_MAX_NUM_INPUT, false, CS_CON_SCMINTOMAXNUM, UI_MESSAGE_V1, caller_);
            try = xtdnumchecker(choice, false);
        }
        else {
            try = rand()%HIGHER_NUM_LIMIT;
            printf(">>%i\n", try);
        }
        if (number == try) {
            winner = p[i];
            i = arrayDim;
        }
        else if (i == arrayDim-1) {
            distanceFromObjective(try, number);
            i = GARBAGE;
        }
        else distanceFromObjective(try, number);
    }
    for (int i = 0; i < arrayDim; i++) {
        if (word_comparison(FRONTMAN, p[i].name)) {
            winner = p[i];
        }
    }
    freeIt(&choice);
    return winner;
}

// Questa funzione genera un playerArray di dimensione N dove N è un valore randomico generato da un
// ciclo definito nella funzione di scrematura principale,
// il puntatore ad intero usedIds passato come parametro della funzione ha lo scopo di raccogliere gli ID dei
// giocatori appena inseriti e di evitare di riutilizzarli in array successivi.

static playerArray *generatePlayerGroup(int **usedIds, int groupSize, playerArray *players, int dim, int *usedIdsElement) {
    playerArray *array = (playerArray*) calloc(groupSize, sizeof(playerArray));
    _Bool valid;
    for (int i = 0, id; i < groupSize; i++) {
        valid = false;
        id = rand()%dim;
        for (int j = 0; j < (*usedIdsElement) && !valid; j++) {
            if (id == (*usedIds)[j]) {
                valid = true;
            }
        }
        if (valid) {
            i--;
        }
        else {
            array[i] = players[id];
            (*usedIds)[(*usedIdsElement)] = id;
            (*usedIdsElement)++;
        }
    }
    return array;
}

// questa funzione contiene sia la chiamata alla funzione per generare un array di giocatori
// e sia la chiamata alla funzione che fa giocare una partita ad indovina il numero, tuttavia se l'array di giocatori
// è formato solo da giocatori BOT il vincitore verrà generato randomicamente e non verrà giocata la partita.
// Del vincitore viene poi restituito come output solo l'id.

static int setSurvivor(int **usedIds, int groupSize, playerArray *players, int dim) {
    static int usedIds_Element = 0;
    playerArray survivor;
    playerArray *array = generatePlayerGroup(usedIds, groupSize, players, dim, &usedIds_Element);
    _Bool playerPresence = false;
    for (int i = 0; i < groupSize; i++) {
        if (!checkIfBot(array[i].name)) {
            playerPresence = true;
        }
    }
    if (playerPresence) {
        survivor = guessNumber(array, groupSize);
        winner_stamp(survivor.name);
    }
    else {
        survivor = array[rand()%groupSize];
    }
    free(array);
    return survivor.id;
}

// Questa è la funzione principale della scrematura

void scrematura(int **inGame, int dim, playerArray *players) {
    // Da questo funzione ottengo le dimensioni dei gruppi e le loro quantità
    // ad esempio:
    // dimensioni: [4][5]
    // quantità:   [9][8]
    int **groups = getGroupsDivision(dim);
    int totGroups = groups[1][0] + groups[1][1];
    // Creo un array per gli ID che sono già stati utilizzati e di cui è già stato determinato il sopravvissuto
    int *usedIds = (int*) calloc(dim, sizeof(int));
    // Riempio ogni cella di memoria legata a usedIds con un "Garbage" value ovvero -1 che non può essere mai assunto da alcun giocatore
    for (int i = 0; i < dim; i++) {
        usedIds[i] = GARBAGE;
    }
    // Alloco un altro array che questa volta raccoglie solo gli ID dei sopravvissuti
    int *survivors = (int*) calloc(totGroups, sizeof(int));
    // Con questo ciclo determino la dimensione del prossimo gruppo (quindi diversa per ogni ciclo) di giocatori e chiamo la funzione
    // setSurvivors per ricavare il vincitore di quel gruppo
    for (int i = 0, groupSize = 0; i < totGroups; i++) {
        // groupSize inizialmente assume un valore compreso tra 0 e 1 per determinare quale dimensione dei gruppi debba assumere,
        // poi si controlla che di quella dimensione ci siano ancora gruppi disponibili da creare, in caso contrario si passa all'altra
        // dimensione
        groupSize = rand()%MINDIM;
        if (groups[1][groupSize] != 0) {
            groups[1][groupSize] -= 1;
            groupSize = groups[0][groupSize];
        }
        // L'espediente del 1 - groupSize serve a scambiare la posizione del controllo senza cambiare il valore di groupSize
        // utilizzerò questo espediente anche nel TRIS e anche in altre situazioni
        else if (groups[1][1 - groupSize] != 0) {
            groups[1][(1 - groupSize)] -= 1;
            groupSize = groups[0][(1 - groupSize)];
        }
        survivors[i] = setSurvivor(&usedIds, groupSize, players, dim);
    }
    free(groups[1]);
    free(groups[0]);
    free(groups);
    // chiamo la funzione che implementa il bubbleSort per riordinare l'array di sopravvissuti al fine di non avere bug nel
    // prossimo ciclo che andrà ad eliminare (settare a false) tutti i giocatori che non sono sopravvissuti
    bubbleSortInt(&survivors, totGroups);
    // Trovandomi a lavorare con un array ordinato posso permettermi di effettuare un lavoro di questo tipo:
    // per ogni valore di i ottengo un ID giocatore, tutti i valori al di sotto di quell'ID giocatore vengono settati a false e j viene
    // incrementato. Una volta che j arriva ad i viene incrementato ancora fino a superarlo, a quel punto i viene incrementato a meno
    // che non sia arrivato al suo ultimo valore disponibile, in tal caso tutti gli altri valori successivi vengono settati a false.
    // esempio:
    // j ha come top 10, i ha come top 3
    // survivors[i] (con i = 0) ha come valore 2
    // j < 2 quindi j = 0 è false e j = 1 è false
    // j arriva a 2 e poi passa direttamente a 3,
    // i viene incrementato di 1 e survivors[i] vale questa volta 3,
    // j == survivors[i] quindi j viene incrementato
    // i viene incrementato in quanto survivors[i] minore di j
    // i arriva a 2 e survivors[2] = 7 quindi j = 4 false j = 5 false j = 6 false j = 7 viene skippato
    // infine i non può più essere incrementato quindi: j = 8 false e j = 9 false
    for (int i = 0, j = 0; i < totGroups && j < dim;) {
        if (j == survivors[i]) {
            j++;
        }
        else if (j > survivors[i] && i != (totGroups - 1)) {
            i++;
        }
        else {
            (*inGame)[j] = false;
            j++;
        }
    }
    free(survivors);
    free(usedIds);
}
