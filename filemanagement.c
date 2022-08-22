//
// Created by marco on 25/02/2022.
//

#include "headers/filemanagement.h"



// Queste funzioni hanno lo scopo di fornire il FILE corrispondente alla posizione data e la posizione stessa
// L'unica differenza fra questa funzione e la successiva è la modalità di apertura del file, che in una permette
// la sola lettura mentre nell'altra permette la sola scrittura

static int fileOpenerByPosROnly(int _pos, FILE **ptr) {
    FILE *filestorage;
    char *filename;
    filestorage = fopen(FILENAMES_DATA, FILE_READ);
    for (int i = 1, breaks = false; breaks == false; i++) {
        filename = calloc(FILENAME_MAX, sizeof(char));
        fgets(filename, FILENAME_MAX, filestorage);
        if (_pos == i) {
            breaks = true;
        }
    }
    fclose(filestorage);
    char *filedir = calloc((int)strlen(FILE_INPUT)+(int)strlen(filename)+MINDIM,sizeof(char));
    int dim = (int)strlen(filename)-1;
    filename[dim] = ENDSTRING;
    strcpy(filedir, DEFAULT_FILE_DIR);
    strcat(filedir,filename);
    if (filedir[strlen(filedir) - 1] == UNIDENTIFIED) filedir[strlen(filedir) - 1] = ENDSTRING;
    strcat(filedir,FILE_EXT);
    filedir[strlen(filedir)] = ENDSTRING;
    ptr[0] = fopen(filedir,FILE_READB);
    free(filename);
    free(filedir);
    if (ptr[0] == NULL) return 0;
    else return _pos;
}

static int fileOpenerByPosWOnly(int _pos, FILE **ptr) {
    FILE *filestorage;
    char *filename;
    filestorage = fopen(FILENAMES_DATA, FILE_READ);
    for (int i = 1, breaks = false; breaks == false; i++) {
        filename = calloc(FILENAME_MAX, sizeof(char));
        fgets(filename, FILENAME_MAX, filestorage);
        if (_pos == i) {
            breaks = true;
        }
    }
    fclose(filestorage);
    char *filedir = calloc((int)strlen(FILE_INPUT)+(int)strlen(filename)+MINDIM,sizeof(char));
    int dim = (int)strlen(filename)-1;
    filename[dim] = ENDSTRING;
    strcpy(filedir, DEFAULT_FILE_DIR);
    strcat(filedir,filename);
    if (filedir[strlen(filedir) - 1] == UNIDENTIFIED) filedir[strlen(filedir) - 1] = ENDSTRING;
    strcat(filedir, FILE_EXT);
    filedir[strlen(filedir)] = ENDSTRING;
    ptr[0] = fopen(filedir, FILE_WRCB);
    free(filename);
    free(filedir);
    if (ptr[0] == NULL) return 0;
    else return _pos;
}

//Questa funzione ha lo scopo di stampare a schermo la lista dei FILE esistenti e di fornire la dimensione della lista stessa

static int showData() {
    FILE *filestorage = fopen(FILENAMES_DATA, FILE_READ);
    char *filename;
    int dim = 1;
    for (int breaks = false; breaks == false; dim++) {
        filename = calloc(FILENAME_MAX, sizeof(char));
        fgets(filename, FILENAME_MAX, filestorage);
        if (filename[0] < SPACE_CHAR || filename[0] > Z_CHAR) {
            breaks = true;
        }
        else {
            printf("%i]%c %s", dim, FILE_ICON, filename);
        }
        free(filename);
    }
    fclose(filestorage);
    return dim;
}

//Questa funzione ha lo scopo di far aprire il FILE richiesto direttamente dall'utente

static void fileOpener(FILE **ptr, int ***_pos) {
    int dim = 1;
    char *ans = NULL;
    // Stampa dei nomi dei salvataggi già creati e ottenimento della dimensione totale della lista dei salvataggi già creati
    dim = showData();
    // Scelta del salvataggio da caricare
    printf("Inserisci il numero corrispondente al salvataggio che intendi caricare");
    for (int breaks = false; breaks == false;) {
        cString(&ans, MINDIM, false, CS_CON_FILENUM, UI_MESSAGE_V2, caller_);
        if (xtdnumchecker(ans, false) <= dim) breaks = true;
    }
    int tmp =xtdnumchecker(ans, false);
    // Apro un file grazie alla posizione ma in modalità di sola lettura
    ***_pos = fileOpenerByPosROnly(tmp,&ptr[0]);
}

//Questa funzione ha lo scopo di far creare all'utente un nuovo FILE di salvataggio da 0

static void fileCreator(FILE **ptr, int ***_pos) {
    FILE *filestorage = fopen(FILENAMES_DATA, FILE_AC);
    if (filestorage == NULL) {
        perror(PERRORARG);
        exit(FILE_NOT_FOUND_ERROR);
    }
    char *fname = NULL;
    char *fname_s;
    printf("Inserisci il nome del file di salvataggio da creare senza estensione");
    // prendo il nome del file e lo manipolo in modo da aprirlo nella stessa cartella degli altri e in modo da aggiungerlo alla lista
    cString(&fname, FILENAME_MAX, false, CS_CON_ALLOWEDFILENAMECHARS, UI_MESSAGE_V2, caller_);
    fname_s = calloc((int)strlen(fname)+(int)strlen(FILE_INPUT)+MINDIM,sizeof(char));
    // creo il nome del file da creare con annessa directory ed estensione
    strcpy(fname_s,DEFAULT_FILE_DIR);
    strcat(fname_s,fname);
    strcat(fname_s,FILE_EXT);
    fname_s[strlen(fname_s)] = ENDSTRING;
    // carico il nome del nuovo file nel registro dei file
    fputs(fname, filestorage);
    fputs(NEWLINE,filestorage);
    free(fname);
    fclose(filestorage);
    ptr[0] = fopen(fname_s,FILE_WRCB);
    if (ptr[0] == NULL) {
        perror(PERRORARG);
        exit(FILE_NOT_FOUND_ERROR);
    }
    free(fname_s);
    filestorage = fopen(FILENAMES_DATA, FILE_READ);
    for (int dim = 1, breaks = false; breaks == false; dim++) {
        fname = calloc(FILENAME_MAX, sizeof(char));
        fgets(fname, FILENAME_MAX, filestorage);
        if (fname[0] == ENDSTRING) breaks = true;
        else ***_pos = dim;
    }
}

// Questa funzione restituisce un valore di tipo Booleano sulla base del tipo di FILE che l'utente ha aperto
// se il file è nuovo il valore di ritorno è false, altrimenti true.

static bool mainFileManagerExt(int **_pos, FILE **ptr) {
    char *ans = NULL;
    _Bool breaks = false;
    printf("Vuoi aprire un vecchio salvataggio o crearne uno da capo? (new/old)\n");
    while (breaks == false) {
        cString(&ans, FILEMANAGEMENT_MAX_INPUT, true, CS_CON_ALLOWEDCHARSFORFILEOPTIONS, UI_MESSAGE_V1, caller_);
        breaks = sString(&ans, SS_COM_FILEOPTIONS, SS_COMM_NOCREMSPACES);
    }
    if(word_comparison(ans,FILEOPTION_NEW) == true) {
        fileCreator(&ptr[0], &_pos);
        free(ans);
        if (*ptr == NULL) exit(FILE_NOT_FOUND_ERROR);
        return false;
    }
    if (*ptr == NULL || word_comparison(ans,FILEOPTION_OLD) == true) {
        fileOpener(&ptr[0], &_pos);
        free(ans);
        if (*ptr == NULL) exit(FILE_NOT_FOUND_ERROR);
        return true;
    }
}

// Il mainFileManager esegue la lettura del file che gli viene passato dal mainFileManagerExt(ended),
// nel caso il file sia stato generato da 0 non viene eseguita alcuna lettura ed il file viene creato e per tutta la durata della partita
// ne viene salvata la posizione all'interno della lista dei file.

int mainFileManager(int *K, player **parray, int *_pos, int *N, int *M, int **pid, int **alivep) {
    FILE *ptr = NULL;
    _Bool ftype = mainFileManagerExt(&_pos, &ptr);
    int inGame = false;
    if (ftype == true) {
        int pvdim;
        fread(&pvdim, sizeof(int), 1, ptr);
        *K = pvdim;
        parray[0] = calloc(pvdim,sizeof(player));
        fread(parray[0],sizeof(player), pvdim, ptr);
        fread(&inGame, sizeof(int), 1, ptr);
        if (inGame == true) {
            fread(N, sizeof(int), 1, ptr);
            fread(M, sizeof(int), 1, ptr);
            *pid = xtdynmem(int,*M);
            *alivep = xtdynmem(int,*N);
            fread(*pid, sizeof(int), *M, ptr);
            fread(*alivep, sizeof(int), *N, ptr);
        }
        fclose(ptr);
    }
    else {
        *parray = NULL;
        fclose(ptr);
    }
    return inGame;
}

// Questa funzione esegue l'update del file che l'utente ha deciso a inizio programma.

static void fileUpdater(const int DIM, const player *pArray, const int _pos, const int inGame, const int N, const int M, const int *pIds, const int *alivePlayers) {
    FILE *ptr = NULL;
    fileOpenerByPosWOnly(_pos, &ptr);
    fwrite(&DIM, sizeof(int), 1, ptr);
    fwrite(pArray, sizeof(player), DIM, ptr);
    fwrite(&inGame, sizeof(int), 1, ptr);
    if (inGame == true) {
        fwrite(&N, sizeof(int), 1, ptr);
        fwrite(&M, sizeof(int), 1, ptr);
        fwrite(pIds, sizeof(int), M, ptr);
        fwrite(alivePlayers, sizeof(int), N, ptr);
    }
    fclose(ptr);
}

// Con questa funzione chiedo all'utente se intende salvare e cosa fare dopo il salvataggio

_Bool saveGame(int DIM, player **pArray, int _pos, int inGame, int N, int M, int **pIds, int **alivePlayers) {
    char *choice = NULL;
    int convChoice = 0;
    printf("Scegli una tra le seguenti opzioni:\n");
    printf("1 >> Salva ed esci dal gioco...\n");
    printf("2 >> Salva e continua il gioco...\n");
    printf("3 >> Continua a giocare senza salvare\n");
    cString(&choice, 1, true, CS_CON_SAVINGOPTION, UI_MESSAGE_V1, caller_);
    convChoice = numchecker(choice[0]);
    free(choice);
    if (convChoice == SAVING_OPTION_3) return false;
    fileUpdater(DIM, *pArray, _pos, inGame, N, M, *pIds, *alivePlayers);
    if (convChoice == SAVING_OPTION_2) return false;
    // Se l'utente decide di uscire dal gioco allora tutti gli array vengono liberati e l'output è true e in questo modo si esce dal programma.
        free(*pArray);
    if (inGame) {
        free(*pIds);
        free(*alivePlayers);
    }
    return true;
}