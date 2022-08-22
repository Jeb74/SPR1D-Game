//
// Created by marco on 12/02/2022.
//

#include "utils.h"

// Le funzioni della utils ho deciso di non modificarle anche se contenenti alcuni break nei cicli.
// Diciamo che ognuna di queste funzioni è stata pensata e realizzata con un notevole sforzo per quanto mi riguarda,
// e pensare a una modalità di risoluzione nuova anche solo per piccoli frangenti è qualcosa di veramente complesso.
// Per questa ragione ho preferito evitare di effettuare modifiche di qualsiasi genere.
// Spero possa comprendere ciò che intendo. In ogni caso queste funzioni può anche non considerarle come parti del
// progetto, ma come mie personali librerie che indipendentemente da come sono state scritte ho deciso di adoperare.

// Questa serie di funzioni ha lo scopo di essere una mia personale libreria che andrò man mano ampliando e migliorando

/* ###################################################################################
 * CHAR CONVERTERS TO NUMBERS FUNCTIONS
 * ###################################################################################
 */

// Questa funzione effettua un controllo sul singolo carattere e verifica che sia o meno un numero, in caso non lo sia
// restituisce 0, altrimenti il valore di quel numero

int numchecker(const char num) {
    if (num >= CHAR_LOWER_NUM && num <= CHAR_HIGHER_NUM) {
        return (int)num - (int)CHAR_LOWER_NUM;
    }
    else return 0;
}

// Questa funzione è un estensione della precedente, infatti ha lo scopo di creare numeri a più cifre:
// se ad esempio questa funzione ricevesse un array di caratteri contenente 102, restituirebbe come intero 102,
// mentre numchecker restituirebbe prima 1 poi 0 ed infine 2
// la variabile booleana skipchars ha la funzione di continuare a processare l'intero array alla ricerca di numeri da
// aggiungere saltando ogni altro carattere

int xtdnumchecker(const char nums[] , _Bool skipchars) {
    int cnum = 0;
    if (skipchars == false) {
        for (int i = 0; nums[i] >= CHAR_LOWER_NUM && nums[i] <= CHAR_HIGHER_NUM; i++) {
            if (cnum == 0) cnum = numchecker(nums[i]);
            else cnum = cnum * _DECIMAL_ + numchecker(nums[i]);
        }
    }
    else {
        for (int j = 0; j < (int)strlen(nums); j++) {
            for (int i = j; nums[i] >= CHAR_LOWER_NUM && nums[i] <= CHAR_HIGHER_NUM; i++) {
                if (cnum == 0) cnum = numchecker(nums[i]);
                else cnum = cnum * _DECIMAL_ + numchecker(nums[i]);
                j = i;
            }
        }
    }
    return cnum;
}

/* ###################################################################################
 * SUPPORT FUNCTION FOR MULTIPLE CALLS
 * ###################################################################################
 */

// Questa funzione è apparentemente inutile, infatti per poter essere sfruttata ha bisogno di essere chiamata mediante
// una macro, il suo scopo è quello di ottenere mediante l'uso di apposite macro, il FILE e la LINEA in cui è stata chiamata,
// la sua utilità è relativa alla funzione cString, la quale la sfrutta per evitare di ricreare l'array di condizioni ogni volta
// che la funzione viene richiamata nella stessa linea e nello stesso file

// Si può definire un'alternativa più elegante all'assegnare un numero qualsiasi a ciascuna chiamata della cString

int detect(const int line, const char func[], _Bool timecheck) {
    static int info_line = _GARBAGE_;
    static char info_func[] = "Not_A_Function_Just_Initialization";
    static int call_id = 0;
    if (info_line != line || strcmp(info_func, func) != 0) {
        info_line = line;
        strcpy(info_func, func);
        call_id = rand();
    }
    if (timecheck == true) {
        call_id = rand();
    }
    return call_id;
}

/* ###################################################################################
 * FUNCTIONS FOR INSERTIONS CONTROLS
 * ###################################################################################
 */

// Le funzioni che seguiranno fino alla cString sono di tipo statico perché non devono e non possono essere usate
// al di fuori del file in cui è contenuta la cString, il tipo static infatti quando utilizzato su delle funzioni
// ha lo scopo di definirne la visibilità, una sorta di private in Java

// Questa funzione consente di inserire un nuovo elemento nella lista

static cl *setElement(cl *element, data data, type type) {
    cl *tmp = (cl*) calloc(1, sizeof(cl));
    (*tmp).prev = element;
    (*tmp).data = data;
    (*tmp).type = type;
    (*tmp).next = NULL;
    if (element != NULL) {
        (*element).next = tmp;
    }
    return tmp;
}

// Questa funzione consente di ottenere l'elemento successivo della lista.
// Per ottenere l'elemento successivo è sufficiente passare come parametro lista sempre la stessa lista e come parametro
// reset sempre false.
// Altrimenti se reset è true il puntatore ptr static assume come valore il top della lista che viene passato come parametro

static cl *getElement(cl *list, _Bool reset) {
    static cl *ptr = NULL;
    if (ptr == NULL) ptr = list;
    else ptr = (*ptr).next;
    if (reset) ptr = NULL;
    return ptr;
}

// Con la getListTop si ottiene semplicemente il top della lista scorrendola al contrario

static cl *getListTop(cl *list) {
    cl *top = list;
    while ((*top).prev != NULL) {
        top = top->prev;
    }
    return top;
}

// Questa funzione libera interamente ogni elemento della lista

static void *freeList(cl *list) {
    while((*list).next != NULL) {
        list = (*list).next;
    }
    cl *tmp = list;
    if ((*list).prev != NULL){
        list = (*list).prev;
        (*list).next = NULL;
    }
    else {
        free(tmp);
        return NULL;
    }
    free(tmp);
    freeList(list);
}

// La funzione getInsertion viene chiamata da cString per ottenere l'input dell'utente, verificandone la dimensione
// è infatti possibile definire una dimensione massima dell'input e fare in modo che essa sia obbligatoria o restringibile
// la variabile fixedDim ha proprio lo scopo di fare in modo che l'input sia o meno di dimensione fissa
// Inoltre essa stampa anche un messaggio (message) che può essere specificato o meno.
// Se l'input inserito è maggiore della dimensione definita esso viene automaticamente resizato alla dimensione massima.

static int getInsertion(char **getIns, int DIM, const _Bool fixedDim, const char message[]) {
    int actualDim;
    _Bool valid = false;
    while (!valid) {
        if (message != NULL) printf("%s", message);
        if (*getIns == NULL) *getIns = (char*) calloc(DIM, sizeof(char));
        scanf("%[^\n]s", *getIns);
        getchar();
        (*getIns)[DIM-1] = ASCII_END;
        if ((*getIns)[0] != ASCII_END){
            if (strlen(*getIns) < DIM && !fixedDim) {
                *getIns = (char *) realloc(*getIns, (strlen(*getIns) + 1) * sizeof(char));
                actualDim = (int)strlen(*getIns);
                (*getIns)[actualDim] = ASCII_END;
                valid = true;
            } else if (strlen(*getIns) == (DIM) - 1) {
                actualDim = DIM - 1;
                (*getIns)[actualDim] = ASCII_END;
                valid = true;
            } else {
                freeIt(getIns);
            }
        }
    }
    return actualDim;
}

// Questa funzione consente di verificare se un carattere fa parte della lista dei caratteri speciali ristretti dalla
// cString

static _Bool checkIfSpecial(const char _char) {
    return SPECIAL(_char);
}

// Questa funzione consente di controllare se un carattere è un numero o meno

static _Bool checkIfNumber(const char _char) {
    return (_char >= CHAR_LOWER_NUM && _char <= CHAR_HIGHER_NUM);
}

// La funzione definita qui ha lo scopo di valutare la condizione relativa ai caratteri in forma di range
// Il suo meccanismo è piuttosto semplice, quasi identico a quello della sua compagna che invece lavora
// sugli interi

// Prima di spiegare il funzionamento della suddetta funzione è necessario illustrare che cosa controlla:
// Le "regole grammaticali" delle condizioni definite qui sotto sono le regole generali che specificano in che modo
// andrebbero formulate le condizioni, e ne evidenziano ogni possibile forma di applicazione:
/*
 * Mettere un ! prima di una qualsiasi definizione di condizione andrà a negare quest'ultima, quindi se ad esempio
 * abbiamo la necessità di ricevere un inserimento di un utente che non deve contenere numeri possiamo applicare la
 * seguente condizione: !(^0|^9) , dove ^ è un carattere che consente d'ignorare ogni tipo di conversione (ad esempio
 * se si volessero bloccare i numeri in generale come nel caso precedente i numeri andrebbero valutati in quanto
 * caratteri e non in quanto interi, perchè scrivere !(0|9) significa che non possono essere inseriti numeri da 0 a 9,
 * quindi il numero 10 andrebbe invece preso come corretto
 *
 * Come già detto il carattere ^ ha lo scopo di ignorare le conversioni, ed inoltre questa proprietà è anche riflessiva:
 * se si volesse creare ad esempio un range dal carattere ^ al carattere z si dovrebbe fare (^^|z)
 *
 * La successione di due punti (..) all'interno di una condizione ne definisce una lista: (..ABCDEFG) una lista è
 * una forma di condizione che consente di abilitare o disabilitare un insieme definito di caratteri, infatti scrivere
 * (A|Z) e (..ABCDEFGH...Z) è la medesima cosa. Inoltre anche le liste possono essere negate.
 *
 * Per quanto riguarda gli interi esistono due tipi di notazioni: la prima, quella più classica è il range:
 * (0|100) vengono accettati tutti i numeri che vanno da 0 a 100;
 * (0|1000) vengono accettati tutti i numeri che vanno da 0 a 1000;
 * la seconda è:
 * (0->) Questa notazione (dove lo 0 può essere sostituito da qualsiasi numero intero) serve a definire un intervallo
 * che va da 0 a infinito, quindi ogni numero compreso in questo intervallo risulta accettabile
 *
 */

/*
 * !(A|Z) negated char range
 * (c|c) char range
 * (..!?^^zxy) char list
 * !(..!?^zxy) negated char list
 * (0->) unlimited int
 * (^1|z) negated conversion symbol
 * (-1) incoming
 */

// Tornando alla regexChars: Questa funzione come già detto esegue i controlli sui range di caratteri, essa verifica che
// il range non sia stato negato e se vi è la presenza di ^ che ignorano le conversioni.
// Essa non restituisce alcun valore, tuttavia mediante puntatori e regole di astrazione modifica il valore sia della
// lista di parametri e sia della posizione del contatore associato al ciclo principale utilizzato nello scorrimento
// dell'array conditions

static void regexChars(cl **list, const char conditions[], int *pos, _Bool negated) {
    data data;
    data._c = ASCII_END;
    type type;
    for (int i = *pos; conditions[i] != WRAPPER_EXIT; i++) {
        if (conditions[i] == SEPARATOR) i++;
        if (conditions[i] == _IGNORE) i++;
        type = char_t;
        if (negated) type = n_char_t;
        data._c = conditions[i];
        *list = setElement(*list, data, type);
        *pos = i;
    }
}

/*
 * La regexIntegers invece valuta le componenti numeriche delle condizioni, le quali arrivano a questa funzione solo
 * ed esclusivamente se prima di esse non vi è il carattere che consente d'ignorare le conversioni
 */

static void regexIntegers(cl **list, const char conditions[], int *pos, _Bool negated) {
    data data;
    data._n = 0;
    type type;
    for (int i = *pos; conditions[i] != WRAPPER_EXIT; i++) {
        for (int j = i; checkIfNumber(conditions[j]); j++){
            if (data._n == 0) {
                data._n = numchecker(conditions[j]);
            } else {
                data._n = data._n * _DECIMAL_ + numchecker(conditions[j]);
            }
            type = int_t;
            if (negated) type = n_int_t;
            i = j;
        }
        if (conditions[i+1] == INF_FRAG && conditions[i+MINDIM] == INF_FRAG_) {
            type = unlimited;
            i += MINDIM;
        }
        if (conditions[i] != SEPARATOR) {
            *list = setElement(*list, data, type);
            data._n = 0;
            *pos = i + 1;
        }
    }
}

/*
 * Questa funzione viene utilizzata per comporre la lista di parametri da utilizzare per i confronti delle condizioni
 */

static void regexCompiler(cl **list, const char conditions[]) {
    // Variabile di controllo della negazione del range o della lista
    _Bool negated = false;
    data data;
    type type;
    // Ciclo scorrimento dell'array di conditions
    for (int i = 0; i <= strlen(conditions)-1;) {
        if (conditions[i] == WRAPPER_EXIT) {
            if (i == strlen(conditions)-1) break; // break
            else i++;
        }
        if (conditions[i] == NEGATION) {
            negated = true;
            i++;
        }
        i++;
        if (conditions[i] == LIST_FRAG) {
            for (int j = i+MINDIM, ignoreSymbol = 0; true; j++) {
                if (conditions[j] == WRAPPER_EXIT && !ignoreSymbol) {
                    i = j;
                    break; // break
                }
                if (conditions[j] == _IGNORE) {
                    j++;
                }
                type = list_t;
                if (negated) type = n_list_t;
                data._c = conditions[j];
                *list = setElement(*list, data, type);
            }
        }
        else if (checkIfNumber(conditions[i])) {
            regexIntegers(&(*list), conditions, &i, negated);
        }
        else {
            regexChars(&(*list), conditions, &i, negated);
        }
        if (negated) negated = false;
    }
    *list = getListTop(*list);
}

/*
 * ERROR CODES:
 * - 89: less parenthesis than expected
 * - -89: more parenthesis than expected
 * - 46: expected more '.' to define a list
 * - 124: No range extremities separator
 * - -33: Wrong negation symbol position
 * - 33: Illegal argument
 * "(A|Z)(..!"£$%&)!(..^!"£$%&)!(A|Z)"
 */
/*
 * Errori:
 * 1 Mancanza di parentesi chiuse e aperte -> (..., ...)
 * 2 Mancanza di separatore di estremi nei range -> (.. ..)
 * 3 Scrittura al di fuori delle parentesi -> ..89765(..)
 * 4 Lista mal formulata -> (.!)("/...)
 * 5 Unlimited range mal impostato -> (^5->), (50|>)
 * 6 Numeri e lettere mescolati in range
 */

/*
 * Questa funzione ha lo scopo di valutare la sintassi delle condizioni e restituire un errore a runtime nel caso
 * una o più condizioni non siano state formattate correttamente.
 */

// PS in questa funzione ho lasciato i break perché per il tempo che ci è voluto a definirne la logica ho preferito
// evitare anche solo di provare a modificarla, anche se in futuro mi toccherà farlo visto che dovrò aggiungere alcune
// features.
static void evaluateSyntax(const char conditions[]) {
    for (int i = 0, parenthesis = 0, verticalSlash = 0, ignoreSymbol = 0; i < strlen(conditions); i++) {
        // Controllo sulle parentesi di apertura
        if (parenthesis == 0 && conditions[i] != NEGATION && conditions[i] != WRAPPER_ACCESS)
            exit(ERROR_ILLEGAL_ARGUMENT);
        else if (parenthesis == 0 && conditions[i] == WRAPPER_ACCESS) {
            parenthesis++;
            i++;
        }
        if (parenthesis == 1 && conditions[i] == WRAPPER_ACCESS && conditions[i-1] != _IGNORE && i != CHAR_LOWER_NUM)
            exit(ERROR_MORE_PARENTHESIS_THAN_EXPECTED);
        // Controllo sulle parentesi di chiusura
        if (parenthesis == 1 && conditions[i] == WRAPPER_EXIT) {
            if (conditions[i-1] == _IGNORE
                && (conditions[i+1] == NEGATION || conditions[i+1] == WRAPPER_ACCESS || i+1 == strlen(conditions))
                && ignoreSymbol) exit(ERROR_NOT_ENOUGHT_PARENTHESIS);
            else  parenthesis--;
        }
        if (parenthesis) {
            // Controllo sul tipo lista
            if (conditions[i-1] == WRAPPER_ACCESS && conditions[i] == LIST_FRAG && conditions[i+1] == LIST_FRAG) {
                i += MINDIM;
                for (int j = i; true; j++) {
                    if (conditions[j] == WRAPPER_EXIT && (conditions[j-1] != _IGNORE || !ignoreSymbol)) break; // break
                    if (conditions[j] == _IGNORE) {
                        ignoreSymbol++;
                        if (!checkIfSpecial(conditions[j+1]) && !checkIfNumber(conditions[j+1]))
                            exit(ERROR_ILLEGAL_ARGUMENT);
                        else {
                            j++;
                            ignoreSymbol--;
                        }
                    }
                    else if (checkIfSpecial(conditions[j]) || checkIfNumber(conditions[j]))
                        exit(ERROR_ILLEGAL_ARGUMENT);
                    i = j;
                }
            }
                // Controllo sul tipo numerico
            else if (conditions[i-1] == WRAPPER_ACCESS && checkIfNumber(conditions[i])) {
                // Entro all'interno di un ciclo per scorrere in modo autonomo la parte numerica
                for (int j = i, onlyNumbers = false, unlimited = false; conditions[j] != WRAPPER_EXIT && unlimited != 1; j++) {
                    i = j;
                    if (!onlyNumbers && !unlimited) {
                        if (conditions[j] == SEPARATOR && checkIfNumber(conditions[j-1])) {
                            onlyNumbers = true;
                            j++;
                        }
                        if (conditions[j] == INF_FRAG && checkIfNumber(conditions[j-1]) && !onlyNumbers) {
                            if (conditions[j+1] == INF_FRAG_ && conditions[j+MINDIM] == WRAPPER_EXIT) {
                                unlimited = true;
                                i = j+1;
                            }
                            else exit(ERROR_ILLEGAL_ARGUMENT);
                        }
                        else if (!(checkIfNumber(conditions[j]))) exit(ERROR_ILLEGAL_ARGUMENT);
                    }
                    if (onlyNumbers) {
                        if (!(checkIfNumber(conditions[j]))) {
                            exit(ERROR_ILLEGAL_ARGUMENT);
                        }
                        else i = j;
                    }
                }
            }
                // Controllo sul tipo carattere
            else {
                for (int j = i; true; j++) {
                    if (conditions[j] == _IGNORE) {
                        ignoreSymbol++;
                        if (checkIfSpecial(conditions[j + 1]) || checkIfNumber(conditions[j + 1])) {
                            ignoreSymbol--;
                            j++;
                        }
                        else exit(ERROR_ILLEGAL_ARGUMENT);
                        if ((conditions[j+1] == SEPARATOR && conditions[j-MINDIM] == WRAPPER_ACCESS) ||
                            (conditions[j+1] == WRAPPER_EXIT && conditions[j-MINDIM] == SEPARATOR)
                            ) j++;
                        else exit(ERROR_ILLEGAL_ARGUMENT);
                    }
                    else if (checkIfSpecial(conditions[j]) || checkIfNumber(conditions[j]))
                        exit(ERROR_ILLEGAL_ARGUMENT);
                    else {
                        if ((conditions[j+1] == SEPARATOR && conditions[j-1] == WRAPPER_ACCESS) ||
                            (conditions[j+1] == WRAPPER_EXIT && conditions[j-1] == SEPARATOR)
                            ) j++;
                    }
                    if (conditions[j] == WRAPPER_EXIT && (conditions[j-1] != _IGNORE || !ignoreSymbol)) {
                        i = j-1;
                        break; // break
                    }
                    i = j;
                }
            }
        }
    }
}

/*
 * Questa funzione analizza le condizioni negate della cString.
 */

static void negativeConditions(cl *list, cl **listPointer, const char *getIns, _Bool *validity) {
    // ottengo il tipo del dato da analizzare
    type type = (**listPointer).type;
    // invertedValidity è semplicemente una variabile booleana che applica il concetto di validità al contrario
    // per ogni volta che la variabile diventa false il ciclo si ferma e la funzione ritorna il valore di invertedValidity
    _Bool invertedValidity = true;
    int num = 0;
    // Position è una variabile che raccoglie i numeri analizzati dalla n_int_t type case, il suo scopo è quello
    // di poter processare separatamente i singoli caratteri numerici invece di semplicemente saltarli e processarli
    // unitamente come numero.
    // Ad esempio se volessi vietare il carattere 6 ma rendessi possibile inserire valori da 0 a infinito allora dovrei
    // prima convalidare il valore numerico e poi i singoli numeri come caratteri.
    int position = _GARBAGE_;
    // Questo ciclo analizza i valori contenuti nella stringa al medesimo modo del ciclo dei valori positivi,
    // con la differenza che se le corrispondenze vengono trovate la funzione restituisce false come valore e
    // l'inserimento viene fatto ripetere.
    for (int i = 0; i < strlen(getIns) && invertedValidity; i++) {
        while (invertedValidity && (*listPointer) != NULL) {
            type = (**listPointer).type;
            switch (type) {
                case n_int_t: {
                    if (checkIfNumber(getIns[i]) && i > position) {
                        for (int j = i; checkIfNumber(getIns[j]); j++) {
                            if (num == 0) num = numchecker(getIns[j]);
                            else num = num * _DECIMAL_ + numchecker(getIns[j]);
                            position = j;
                        }
                        cl *tmp = getElement(list, false);
                        invertedValidity = !((**listPointer).data._n <= num && num <= (*tmp).data._n);
                    }
                }
                    break;
                case n_char_t: {
                    cl *tmp = getElement(list, false);
                    invertedValidity = !((**listPointer).data._c <= getIns[i] && getIns[i] <= (*tmp).data._c);
                }
                    break;
                case n_list_t: {
                    invertedValidity = ((**listPointer).data._c != getIns[i]);
                }
                    break;
                default: break;
            }
            *listPointer = getElement(list, false);
        }
        *listPointer = getElement(list, true);
        *listPointer = getElement(list, false);
    }
    if (!invertedValidity) *validity = invertedValidity;
    *listPointer = getElement(list, true);
}

/*
 * Questa funzione effettua dei controlli sull'inserimento: nello specifico verifica che ogni carattere inserito
 * sia o abilitato o non negato dal programmatore.
 *
 * Per effettuare le valutazioni necessarie è stata sfruttata la struttura dati lista, in modo da avere uno scorrimento
 * più agevole dei parametri da utilizzare per la convalida, evitando cicli con indici che avrebbero sicuramente creato
 * maggiore difficoltà di lettura del codice.
 */

static _Bool evaluateInsertion(const char *getIns, const int DIM,  const char conditions[], int caller) {
    // Inizializzo una variabile che avrà la funzione di tenere traccia della chiave generata dal caller a ogni
    // chiamata della funzione lo scopo di questa variabile è quello di evitare di ripetere l'analisi delle condizioni
    // se sia la linea che la funzione in cui è stata chiamata la cString non cambiano.
    static int lastCaller = 0;
    // Anche la lista viene marcata come static per mantenerne il valore e viene aggiornata (e naturalmente la memoria
    // precedente viene liberata) solo quando il caller andrà a cambiare
    static cl *list = NULL;
    // Se il caller cambia allora la lista viene liberata e ricreata sulla base delle nuove condizioni
    if (caller != lastCaller && conditions != NULL) {
        if (list != NULL) freeList(list);
        list = NULL;
        // Questa funzione andrà a creare la lista
        regexCompiler(&list, conditions);
    }
    // Se conditions è NULL o è uguale alla stringa "reset" allora la memoria della lista viene liberata e la funzione
    // ritorna automaticamente true come output.
    if (conditions == NULL) {
        freeList(list);
        return true;
    }
    // In questo punto il lastCaller viene aggiornato al nuovo valore
    lastCaller = caller;
    _Bool validity = true;
    // Ottengo il primo elemento della lista e lo passo alla funzione che controlla le negazioni
    cl *listPointer = getElement(list, false);
    negativeConditions(list, &listPointer, getIns, &validity);
    // Se alla fine delle condizioni negative il puntatore alla lista ha valore NULL (quindi è arrivata alla fine della lista)
    // allora il puntatore riassume il valore del top della lista.
    if (listPointer == NULL) listPointer = getElement(list, false);
    // Ciclo di scorrimento dei caratteri che compongono l'inserimento
    for (int i = 0; i < DIM && validity; i++) {
        // Nel do while vengono valutate le singole condizioni, se una condizione risulta vera la validità viene confermata
        // e il ciclo viene fermato.
        do {
            // la validità viene messa a false
            validity = false;
            // Se il puntatore alla lista è NULL il puntatore viene ripuntato al top della lista.
            if (listPointer == NULL) listPointer = getElement(list, false);
            // Qui definisco il tipo del dato da analizzare
            type type = (*listPointer).type;
            // Con lo switch applico il tipo di analisi al tipo di dato da analizzare
            switch(type) {
                case char_t: {
                    cl *tmp = getElement(list, false);
                    validity = ((*listPointer).data._c <= getIns[i] && getIns[i] <=(*tmp).data._c);
                }
                    break;
                case int_t:
                case unlimited: {
                    int num = _GARBAGE_;
                    for (int j = i; CHAR_LOWER_NUM <= getIns[j] && getIns[j] <= CHAR_HIGHER_NUM; j++) {
                        if (num == _GARBAGE_) num = numchecker(getIns[j]);
                        else num = num * _DECIMAL_ + numchecker(getIns[j]);
                        i = j;
                    }
                    if (type == int_t) {
                        cl *tmp = getElement(list, false);
                        validity = ((*listPointer).data._n <= num && num <= (*tmp).data._n);
                    }
                    else {
                        validity = ((*listPointer).data._n <= num && num != _GARBAGE_);
                    }
                }
                    break;
                case list_t: {
                    while ((*listPointer).type == list_t && (*listPointer).type == type) {
                        validity = ((*listPointer).data._c == getIns[i]);
                        if (validity) break;
                        listPointer = getElement(list, false);
                        if (listPointer == NULL) break;
                    }
                }
                    break;
                default: break;
            }
            if (listPointer != NULL && type != list_t) listPointer = getElement(list, false);
        } while (listPointer != NULL && !validity);
        // resetto il puntatore della lista
        listPointer = getElement(list, true);
    }
    // resetto il puntatore della lista
    listPointer = getElement(list, true);
    return validity;
}

/**
 * @param getIns In this section you have to pass the address of a pointer with no memory allocated that points to NULL.
 * @param DIM This is the maximum dimension of the input that you are going to receive from the user.
 * @param fixedDim Here you can specify if the dimension is fixed or the input can have a lower dimension.
 * @param conditions This is the parameter where you have to specify the conditions of an acceptable input.
 * @param message This is the message that would be shown to the user when he has to give the input.
 * @param caller This is a special section dedicated to the macro caller_ or caller_t
 *
 * @return This function has no return but it modify the pointer that you passed in the first parameter slot and gives you\n
 * the input of the user.
 */

void cString(char **getIns, int DIM, _Bool fixedDim, const char conditions[], const char message[], const int caller) {
    if (conditions != NULL && conditions[0] != 0 && !word_comparison(conditions, RESET_CONDITION)) {
        evaluateSyntax(conditions);
    }
    else if (word_comparison(conditions, RESET_CONDITION)) {
        evaluateInsertion(NULL, 0, NULL, caller);
        return;
    }
    DIM++;
    static int lastCaller = 0;
    if (*getIns != NULL) freeIt(&(*getIns));
    if (caller != lastCaller) {
        *getIns = (char *) calloc(DIM, sizeof(char));
    }
    _Bool breaks = false;
    while (!breaks) {
        int actualDim = 0;
        if (!word_comparison(conditions, RESET_CONDITION)) {
            actualDim = getInsertion(getIns, DIM, fixedDim, message);
        }
        if (evaluateInsertion(*getIns, actualDim, conditions, caller)) breaks = true;
        else freeIt(&(*getIns));
    }
}

/* ###################################################################################
 * CUSTOM FREE FUNCTION WITH REDIRECTION OF POINTERS
 * ###################################################################################
 */

/*
 * Libera la memoria di un puntatore di stringhe e gli riassegna il "valore" NULL
 */

void freeIt(char **var) {
    if (*var != NULL) free(*var);
    *var = NULL;
}

/* ###################################################################################
 * WORD MODIFICATIONS AND COMPARATIONS FUNCTIONS
 * ###################################################################################
 */

/*
 * Questa funzione compara due stringhe inizialmente sulla base della lunghezza e successivamente nel loro contenuto
 */

_Bool word_comparison(const char string[], const char string_[]) {
    if (strlen(string) != strlen(string_)) return false;
    for (int i = 0; string[i] >= SPACE_CHAR && string_[i] >= SPACE_CHAR && string[i] <= Z_CHAR && string_[i] <= Z_CHAR; i++) {
        if ((string[i] != string_[i])) return false;
    }
    return true;
}

/*
 * Questa funzione serve a eseguire dei "comandi" di modifica definiti dal programmatore su un ipotetica stringa.
 * Questo tipo di approccio alle funzioni è il risultato di una forte influenza di python e dei suoi "attributi" dei metodi:
 * del tipo il metodo/funzione print come attributo può avere end="carattere da inserire alla fine della stampa"
 */

void s_Commands_Exec(const char commands[], char **getIns) {
    unsigned int ccount = 0;
    unsigned int progression = 0;
    char *command;
    //Questo ciclo conta quanti comandi sono stati inseriti
    for (int i = 0; i < (int)strlen(commands); i++) {
        if (commands[i] == SS_SEPARATOR) ccount++;
    }
    //Con questo ciclo ci si assicura che tutti i comandi vengano eseguiti
    for (int i = 0; i < ccount; i++) {
        command = (char*) calloc(MAXCOMMAND_DIM, sizeof(char));
        //Questo ciclo separa i singoli comandi dalla lista di comandi principale
        for (unsigned int j = 0; j < (int)strlen(commands) && commands[i] != SS_SEPARATOR; j++) {
            command[j] = commands[j+progression];
            if (commands[j+progression+1] == SS_SEPARATOR) {
                progression = progression+j+MINDIM;
                j = (unsigned int)strlen(commands);
            }
        }
        if (word_comparison(command, REMOVE_INITIAL_SPACES) == 1) {
            char temporary = getIns[0][0];
            while (temporary == SPACE_CHAR) {
                for (int k = 1; k < (int) strlen(getIns[0]); k++) {
                    getIns[0][k - 1] = getIns[0][k];
                }
                getIns[0] = (char *) realloc(getIns[0], sizeof(char) * ((int) strlen(getIns[0]) - 1));
                temporary = getIns[0][0];
            }
        }
        else if (word_comparison(command, REMOVE_CENTRAL_SPACES) == 1) {
            for (int k = 0; k < (int)strlen(getIns[0]); k++) {
                if (getIns[0][k] == SPACE_CHAR && getIns[0][k+1] == SPACE_CHAR) {
                    for (int j = k + 1; j < (int) strlen(getIns[0]); j++) {
                        getIns[0][j] = getIns[0][j + 1];
                    }
                    getIns[0] = (char*) realloc(getIns[0], sizeof(char)*((int)strlen(getIns[0])-1));
                    k--;
                }
            }
        }
        else if (word_comparison(command, REMOVE_ALL_SPACES) == 1) {
            for (int k = 0; k < (int)strlen(getIns[0]); k++) {
                if (getIns[0][k] == SPACE_CHAR) {
                    for (int j = k; j < (int) strlen(getIns[0]); j++) {
                        getIns[0][j] = getIns[0][j + 1];
                    }
                    getIns[0] = (char*) realloc(getIns[0], sizeof(char)*((int)strlen(getIns[0])-1));
                    k--;
                }
            }
        }
        else if (word_comparison(command, INITIAL_CHAR_AS_CAPS) == 1) {
            if (getIns[0][0] >= A_CHAR && getIns[0][0] <= Z_CHAR) {
                getIns[0][0] -= SPACE_CHAR;
            }
        }
        else if (word_comparison(command, INITIAL_CHARS_OF_SUBSTRINGS_AS_CAPS) == 1) {
            for (int k = 0; k < (int)strlen(getIns[0]); k++) {
                if (getIns[0][k] >= A_CHAR && getIns[0][k] <= Z_CHAR && (getIns[0][k-1] == SPACE_CHAR || k == 0)) {
                    getIns[0][k] -= SPACE_CHAR;
                }
            }
        }
        else if (word_comparison(command, ALL_CHARS_AS_CAPS) == 1) {
            for (int k = 0; k < (int)strlen(getIns[0]); k++) {
                if (getIns[0][k] >= A_CHAR && getIns[0][k] <= Z_CHAR) getIns[0][k] -= SPACE_CHAR;
            }
        }
        else if (word_comparison(command, NO_CAPS) == 1) {
            for (int k = 0; k < (int)strlen(getIns[0]); k++) {
                if (getIns[0][k] >= CA_CHAR && getIns[0][k] <= CZ_CHAR) getIns[0][k] += SPACE_CHAR;
            }
        }
        else if (word_comparison(command, CLASSICAL_NAMES_FORMAT) == 1) {
            for(int k = 0; k < (int)strlen(getIns[0]); k++) {
                if (getIns[0][k] >= CA_CHAR && getIns[0][k] <= CZ_CHAR && (getIns[0][k-1] != SPACE_CHAR || k != 0))
                    getIns[k] += SPACE_CHAR;
                else if (getIns[0][k] >= A_CHAR && getIns[0][k] <= Z_CHAR && (getIns[0][k-1] == SPACE_CHAR || k == 0))
                    getIns[k] -= SPACE_CHAR;
            }
        }
        else exit(EXIT_FAILURE);
    }
    free(command);
}

/*
 * Questa funzione consente di effettuare delle comparazioni tra l'input dell'utente e dei match predefiniti dal programmatore:
 * Nel caso in cui nessuno dei match sia rispettato la funzione restituirà come risultato false.
 */

static _Bool AnalyzeWords(const char words[], char getIns[]) {
    unsigned int wcount = 0;
    int progression = 0, *wlenght = (int*) calloc(MAXWORD_LIST, sizeof(int));
    _Bool isit;
    char *word;
    for (int i = 0; i < (int)strlen(words); i++) {
        if (words[i] == SS_SEPARATOR) {
            wcount++;
            wlenght[wcount-1] = i;
        }
        if (words[i+1] == ASCII_END) {
            wlenght = realloc(wlenght,sizeof(int)*wcount);
        }
    }
    for (int i = 0; i < wcount; i++) {
        word = calloc(wlenght[i]+1,sizeof(char));
        for (int j = progression; j < (int)strlen(words); j++) {
            word[j - progression] = words[j];
            if (words[j+1] == SS_SEPARATOR) {
                progression = j+MINDIM;
                j = (int)strlen(words);
            }
        }
        word[strlen(word)] = ASCII_END;
        //Comparatore di parole
        if (word_comparison(getIns, word) == 1) {
            free(word);
            return true;
        }
        else {
            isit = false;
            free(word);
        }
    }
    return isit;
}

// questa funzione è descritta in inglese perchè avevo l'idea di rendere questa libreria opensource e disponibile anche ai colleghi
// (naturalmente dopo la consegna del progetto)

/**
 * @details This function only accepts dynamically allocated variables and arrays\n
 * You can use the MACRO NULL for the words and commands field to let them Empty!\n
 * You can concatenate commands and words: "command;command;" "word;word;"
 * @commands
 * rispaces;     -> Removes all initial spaces of the string;\n
 * rcspaces;     -> Removes multiple spaces between words of the array;\n
 * raspaces;     -> Removes all spaces;\n
 * capsinits;    -> Converts to uppercase any initial character of any string between 97 (a) and 122 (z) ASCII code;\n
 * capsinitw;    -> Converts to uppercase any initial character of any word between 97 (a) and 122 (z) ASCII code;\n
 * allcaps;      -> All lowercase become uppercase;\n
 * nocaps;       -> All uppercase become lowercase;\n
 * classicnames; -> Formats all words as the classical name writing form;\n
 * @return This function returns a Bool value based on Defined Correspondence or default true if no given matches.
 * @return This function can modify a string with the given commands listed above.
 */
_Bool sString (char **getIns, const char words[], const char commands[]) {
    if (strcmp(commands, RESET_COMMAND) != 0){
        s_Commands_Exec(commands,&(*getIns));
    }
    if (strcmp(words, RESET_COMMAND) != 0) {
        return AnalyzeWords(words, *getIns);
    }
    return true;
}

// Questa funzione serve a effettuare delle comparazioni tra stringhe ignorando le differenze dei caratteri

_Bool ccLess_Comparison(const char string[], const char string_[]) {
    unsigned int comparison;
    for (int i = 0; string[i] >= SPACE_CHAR && string_[i] >= SPACE_CHAR && string[i] <= Z_CHAR && string_[i] <= Z_CHAR; i++) {
        if (string[i] == string_[i]) comparison = 1;
        else if (string[i] >= CA_CHAR && string[i] <= CZ_CHAR && string_[i] >= A_CHAR && string_[i] <= Z_CHAR) {
            if (string[i]+SPACE_CHAR == string_[i]) {
                comparison = 1;
            }
        }
        else if (string[i] >= A_CHAR && string[i] <= Z_CHAR && string_[i] >= CA_CHAR && string_[i] <= CZ_CHAR) {
            if (string[i] == string_[i] + SPACE_CHAR) {
                comparison = 1;
            }
        }
        else return false;
    }
    return comparison;
}

/*
 * Questa funzione consente di ignorare le differenze tra caratteri lowercase e uppercase
 * (è stata creata molto dopo ccLess_Comparison...)
 */

static _Bool ignoreCaps(char _char, char _char2) {
    if ((_char >= CA_CHAR && _char <= CZ_CHAR) || (_char >= A_CHAR && _char <= Z_CHAR)) {
        if ((_char2 >= CA_CHAR && _char2 <= CZ_CHAR) || (_char2 >= A_CHAR && _char2 <= Z_CHAR)) {
            return (_char == _char2 || (_char - SPACE_CHAR) == _char2 || _char  == (_char2 - SPACE_CHAR));
        }
    }
    return _char == _char2;
}

/*
 * Questa è la mia replica della funzione insuccesso, che come la principale supporta il controllo case Sensitive
 */

static int *containsSupportPos(char *_chars, _Bool caseSensitive) {
    int *insucc = (int*) calloc(strlen(_chars), sizeof(int));
    insucc[0] = 0;
    for (int i = 1, j = 0; i < strlen(_chars); i++) {
        if ((!caseSensitive && ignoreCaps(_chars[j], _chars[i])) || _chars[j] == _chars[i]) {
            j++;
            insucc[i] = j;
        }
        else if (j != 0) {
            j = insucc[j-1];
        }
        else {
            insucc[i] = 0;
        }
    }
    return insucc;
}

/*
 * La funzione contains è la mia replica dell'algoritmo di ricerca di sottostringhe KMP, con la differenza che vi è la possibilità
 * di specificare se la ricerca deve essere o meno case Sensitive.
 * Naturalmente questa modifica non cambia in alcun modo il metodo di analisi proprio dell'algoritmo. La sola differenza al livello
 * di output è che invece di avere come return una stringa di interi contenenti le possibili posizioni in cui vi è stato un match,
 * l'output prevede come return un nuovo tipo definito contenente un parametro booleano e un array di interi. In questo modo è
 * possibile sfruttare al meglio le potenzialità dell'algoritmo evitando analisi supplementari dell'output.
 */

match *contains(char *_chars, char *_str, _Bool caseSensitive) {
    match *result = (match*) calloc(1, sizeof(match));
    (*result).success = false;
    (*result).index = 0;
    (*result).indexes = (int*) calloc(1, sizeof(int));
    int *insucc = containsSupportPos(_chars, caseSensitive);
    for (int i = 0, j = 0; i < strlen(_str);) {
        if ((!caseSensitive && ignoreCaps(_str[i], _chars[j])) || _str[i] == _chars[j]) {
            i++;
            j++;
        }
        if (j == strlen(_chars)) {
            (*result).success = true;
            (*result).index++;
            (*result).indexes = (int*) realloc((*result).indexes, (*result).index * sizeof(int));
            (*result).indexes[(*result).index-1] = i-j;
            j = 0;
        }
        else if (i < strlen(_str)) {
            _Bool selection = (!caseSensitive) ? !ignoreCaps(_chars[j], _str[i]) : _chars[j] != _str[i];
            if (selection) {
                if (j != 0) {
                    j = insucc[j - 1];
                } else {
                    i++;
                }
            }
        }
    }

    free(insucc);
    return result;
}

/*
 * Questa funzione effettua dei controlli supplementari sull'output della funzione contains e restituisce un output più
 * comodo da utilizzare al livello di modifica dei dati. Se invece si intende ottenere il solo output booleano è conveniente
 * non utilizzare questa funzione dopo la contains
 */

int **checkMatches(match **matches) {
    if ((**matches).success) {
        int **result = (int**) calloc(MINDIM, sizeof(int*));
        result[0] = (int*) calloc(1, sizeof(int));
        result[0][0] = (**matches).index;
        result[1] = (int*) calloc(result[0][0], sizeof(int));
        for (int i = 0; i < result[0][0]; i++) {
            result[1][i] = (**matches).indexes[i];
        }
        free((**matches).indexes);
        free(*matches);
        return result;
    }
    free((**matches).indexes);
    free(*matches);
    return NULL;
}

/*
 * Questa funzione converte un qualsiasi numero intero positivo in una stringa rappresentante il numero stesso.
 * Per fare ciò è bastato prendere in esame il numero e mediante l'operatore modulo dividerlo nelle sue componenti,
 * una volta fatto ciò il valore di quel numero viene sommato a 48 (il valore dello 0 nella tavola ASCII):
 * Successivamente il carattere ottenuto viene assegnato alla posizione 0 dell'array stringa e al prossimo ciclo viene
 * spostata di una posizione indietro. Alla fine delle conversioni l'array viene resizato alla dimensione dell'array + 1
 * per mantenere il posto del terminatore di stringa.
 */

char *numToString(int num) {
    char *numb = (char*) calloc(MAXDIGITS_LEN, sizeof(char));
    numb[0] = CHAR_LOWER_NUM;
    int dim = 0;
    while (num != 0) {
        for (int i = dim; dim != 0 && i > -1; i--) {
            numb[i] = numb[i - 1];
        }
        numb[0] = (num % _DECIMAL_) + CHAR_LOWER_NUM;
        num /= _DECIMAL_;
        dim++;
    }
    numb = (char*) realloc(numb, sizeof(char)*(strlen(numb) + 1));
    return numb;
}

/*
 * Questa funzione converte un carattere in una stringa di caratteri di dimensione 1
 */

char *charToString(const char _char) {
    static char str[1];
    str[0] = _char;
    return str;
}

/*
 * Questa funzione implementa il bubbleSort (miglior algoritmo di ordinamento per semplicità e tempo massimo di lavoro):
 * Molto semplicemente il bubbleSort effettua comparazioni e scambi fino all'ultima posizione portando sempre
 * all'ultima posizione accessibile il valore più alto.
 * Con accessibile si intende che a ogni ciclo i il bubbleSort decrementa il valore dell'ultima posizione accessibile,
 * in questo modo si evita di avere interazioni inutili con posizioni già precedentemente ordinate
 */

void bubbleSortInt(int **array, int dimension) {
    for (int i = 0; i < dimension - 1; i++) {
        for (int j = 0, tmp; j < dimension - i - 1; j++) {
            if ((*array)[j] > (*array)[j+1]) {
                tmp = (*array)[j];
                (*array)[j] = (*array)[j+1];
                (*array)[j+1] = tmp;
            }
        }
    }
}
