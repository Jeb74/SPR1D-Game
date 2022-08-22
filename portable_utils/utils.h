//
// Created by marco on 12/02/2022.
//
#ifndef FINAL_PROJECT_UTILS_H
#define FINAL_PROJECT_UTILS_H

#pragma once

#define MINDIM 2
#define MAXCOMMAND_DIM 15
#define MAXWORD_LIST 20

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

typedef union {
    int _n;
    char _c;
} data;
typedef enum {
    char_t = 0,
    int_t = 1,
    list_t = 2,
    n_char_t = 3,
    n_int_t = 4,
    n_list_t = 5,
    unlimited = 6
} type;

typedef struct conditions_list {
    struct conditions_list *prev;
    data data;
    type type;
    struct conditions_list *next;
} cl;

typedef struct {
    _Bool success;
    int index;
    int *indexes;
} match;

#define RESET_CONDITION "reset"

#define _GARBAGE_ -1

#define CHAR_LOWER_NUM '0'
#define CHAR_HIGHER_NUM '9'
#define _DECIMAL_ 10

#define ASCII_END '\0'
#define SPECIAL(x) ( \
(x) == '!' ||        \
(x) == '.'||         \
(x) == '('||         \
(x) == ')'||         \
(x) == '|'||         \
(x) == '-'||         \
(x) == '>'||         \
(x) == '^'           \
)

#define NEGATION '!'
#define LIST_FRAG '.'
#define WRAPPER_ACCESS '('
#define WRAPPER_EXIT ')'
#define SEPARATOR '|'
#define INF_FRAG '-'
#define INF_FRAG_ '>'
#define _IGNORE '^'

#ifndef REGEX_ERRORS
#define REGEX_ERRORS
#define ERROR_NOT_ENOUGHT_PARENTHESIS 89
#define ERROR_MORE_PARENTHESIS_THAN_EXPECTED (-89)
#define ERROR_LIST_DEFINITION 46
#define ERROR_NO_SEPARATOR_FOUND 124
#define ERROR_NEGATION_POSITION (-33)
#define ERROR_ILLEGAL_ARGUMENT 33
#endif /* REGEX ERRORS */

#define Z_CHAR 'z'
#define A_CHAR 'a'
#define CA_CHAR 'A'
#define CZ_CHAR 'Z'
#define SPACE_CHAR ' '

#define SS_SEPARATOR ';'

#ifndef SS_COMMANDS
#define SS_COMMANDS
#define REMOVE_INITIAL_SPACES "rispaces"
#define REMOVE_CENTRAL_SPACES "rcspaces"
#define REMOVE_ALL_SPACES "raspaces"
#define INITIAL_CHAR_AS_CAPS "capsinits"
#define INITIAL_CHARS_OF_SUBSTRINGS_AS_CAPS "capsinitw"
#define ALL_CHARS_AS_CAPS "allcaps"
#define NO_CAPS "nocaps"
#define CLASSICAL_NAMES_FORMAT "classicalnames"
#define RESET_COMMAND "NULL"
#endif

#define MAXDIGITS_LEN 11

int numchecker(char num);
int xtdnumchecker(const char nums[] , _Bool skipchars);
_Bool word_comparison(const char string[], const char string_[]);
int detect(int line, const char func[], _Bool timecheck);
void cString(char **getins, int DIM, _Bool fixedDim, const char conditions[], const char message[], int caller);
_Bool sString (char **getIns, const char words[], const char commands[]);
_Bool ccLess_Comparison(const char string[], const char string_[]);
void freeIt(char **var);
match *contains(char *_chars, char *_str, _Bool caseSensitive);
int **checkMatches(match **matches);
char *numToString(int num);
char *charToString(const char _char);
void bubbleSortInt(int **array, int dimension);

#define xtdynmem(type,size) (type*)calloc(size,sizeof(type))
#define caller_ detect(__LINE__,__func__,false)
#define caller_t detect(__LINE__,__func__,true)
#endif //FINAL_PROJECT_UTILS_H
