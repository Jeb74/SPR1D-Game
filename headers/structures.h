//
// Created by marco on 13/02/2022.
//
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#if defined __linux__ || defined __APPLE__
#include <unistd.h>
#elif defined WIN32 || defined WIN64
#include <windows.h>
#endif
#include <time.h>

#ifndef FINAL_PROJECT_STRUCTURES_H
#define FINAL_PROJECT_STRUCTURES_H

//System output defines
//System output Format characters
#if defined WIN32 || defined WIN64
#define SO_C_1 205 //O
#define SO_C_2 201 //NW
#define SO_C_3 200 //SW
#define SO_C_4 188 //SE
#define SO_C_5 187 //NE
#define SO_C_6 186 //V
#elif defined __linux__ || defined __APPLE__
#define SO_C_1 "\xE2\x95\x90" //O
#define SO_C_2 "\xE2\x95\x94" //NW
#define SO_C_3 "\xE2\x95\x9A" //SW
#define SO_C_4 "\xE2\x95\x9D" //SE
#define SO_C_5 "\xE2\x95\x97" //NE
#define SO_C_6 "\xE2\x95\x91" //V
#endif
#define SO_FORMAT 90
//System output winner message lines number
#define SO_LINES 3

//System output winner message dimension of lines
#define SO_FORMAT 90
//System output defines </>
//bots defines
#define PROBABILITY_TO_MAKE_MOVE 100
#define PROBABILITY_CAP 70
#define BLACKJACK_SAFE_SCORE 18
#define BLACKJACK_CHOICE_LEN 6
#define BOTS_ROOT "CPU-"
#define LOWEST_VALUE_TRIS 'A'
#define MAX_ALLOWED_POS 6
#define RIGHT '>'
#define LEFT '<'
#define FIRST_BJ_MOVE "pesco"
#define SECOND_BJ_MOVE "resto"
//bots defines </>
//blackjack defines
typedef enum {HEARTS = 0, SPADES = 1, DIAMONDS = 2, CLUBS = 3} seed;
typedef enum {
    ACE = 0,
    TWO = 1,
    THREE = 2,
    FOUR = 3,
    FIVE = 4,
    SIX = 5,
    SEVEN = 6,
    EIGHT = 7,
    NINE = 8,
    TEN = 9,
    JACK = 10,
    QUEEN = 11,
    KING = 12
} value;
typedef struct {
    value value;
    seed seed;
} cards;

typedef struct {
    _Bool used;
    cards card;
} deck;

typedef struct {
    cards *hand;
    char *name;
} bjPlayer;

#define CARDNUM 13
#define SEEDS 4
#define ACE_SEC_VALUE 10
#define BLACKJACK 21
#define CARDDIM 6
#define SJACK 'J'
#define SQUEEN 'Q'
#define SKING 'K'
#define SACE 'A'

#define SAFERANGE_LOWER 8
#define SAFERANGE_HIGHER 3

#define THREE_WHITE_SPACES "   "

#define TARGET_BEF 4
#define TARGET_BEFV 5

#define BOID 5

#if defined _WIN32 || defined WIN64
#define SPADE   "\x06"
#define CLUB    "\x05"
#define HEART   "\x03"
#define DIAMOND "\x04"
#else
#define SPADE   "\xE2\x99\xA0"
#define CLUB    "\xE2\x99\xA3"
#define HEART   "\xE2\x99\xA5"
#define DIAMOND "\xE2\x99\xA6"
#endif
//blackjack defines </>
//dadi defines
#define MINFACE 1
#define MAXFACE 6
#define ENTER '\n'
//dadi defines </>
//filemanagement defines
#define FILE_ICON 175
#define FILE_NOT_FOUND_ERROR 404
#define FILENAMES_DATA "../files/filenames/filenames.txt"
#define DEFAULT_FILE_DIR "../files/filenames/data/"
#define FILE_INPUT "..files/filenames/data/.bin"
#define FILEMANAGEMENT_MAX_INPUT 3
#define FILE_EXT ".bin"
#define FILE_READ "r"
#define FILE_READB "rb"
#define FILE_WRCB "w+b"
#define FILE_AC "a+"
#define PERRORARG "fopen"
#define FILEOPTION_NEW "new"
#define FILEOPTION_OLD "old"
#define SAVING_OPTION_2 2
#define SAVING_OPTION_3 3
//filemanagement defines </>
//impiccato defines
#define DEFAULT_DICTIONARY "../files/impiccato_phrases/default.txt"
#define PHRASE_MAX_LENGTH 120
#define MAX_DICTIONARY_LENGTH 50
#define NEXT_Z_CHAR 91
#define IMPICCATO_TYPE_DIM 7
#define LINUXENDCHARS "\r\n"
#define UNDERLINE '_'
#define UNDERLINE_STRING "_"
#define NUMMAXLEN 3
#define ADAPTEDV_ERRORS_LIMIT 4
#define CLASSIC_MODE "classic"
typedef enum {
    HEAD,
    NECK,
    CHEST,
    RIGHT_ARM,
    LEFT_ARM,
    LEFT_LEG,
    RIGHT_LEG,
    FULLBODY
} body;
typedef enum {
    VIRGOLA = ',',
    PUNTO = '.',
    DUE_PUNTI = ':',
    PUNTO_E_VIRGOLA = ';',
    INTERROGATIVO = '?',
    ESCLAMATIVO = '!',
    SPAZIO = ' ',
    ACCENTO_VIRGOLETTA = '\''
} characters;
//impiccato defines </>
//morracinese defines
#define NUM_OF_OPTIONS 3
#define SASSO "sasso"
#define CARTA "carta"
#define FORBICI "forbici"
#define SASSO_ID 1
#define CARTA_ID 2
#define FORBICI_ID 3
#define PRINT_SASSO printf("sasso\n");
#define PRINT_CARTA printf("carta\n");
#define PRINT_FORBICI printf("forbici\n");
#define MC_MAXINPUT 7
#define WINCONDITION_FIRSTPLAYER(fp,sp) ((fp) - (sp) == 1 || (fp) - (sp) == -2)
//morracinese defines </>
//Ponte di vetro defines
#if defined WIN32 || defined WIN64
#define GLASS_PRESENCE 219
#elif defined __linux__ || defined __APPLE__
#define GLASS_PRESENCE "\xE2\x96\x88"
#endif
#define GLASS_LINES 3
#define GLASS_COLUMNS 2
#define RANDOM_ACCESS rand()%2
//Ponte di vetro defines </>
//Tris defines

#if defined WIN32 || defined WIN64
#define UP_DOWN_WALL 205 //O
#define NWANGLE 201 //NW
#define SWANGLE 200 //SW
#define SEANGLE 188 //SE
#define NEANGLE 187 //NE
#define VWALL 186 //V
#elif defined __linux__ || defined __APPLE__
#define UP_DOWN_WALL "\xE2\x95\x90" //O
#define NWANGLE "\xE2\x95\x94" //NW
#define SWANGLE "\xE2\x95\x9A" //SW
#define SEANGLE "\xE2\x95\x9D" //SE
#define NEANGLE "\xE2\x95\x97" //NE
#define VWALL "\xE2\x95\x91" //V
#endif

#define SPACE_ID 2

#define ROWS 3

#define GRAPHIC_CA 0.3

#define FIVE_SPACES "     "

#define CTRIS_LOWER_CHAR 'A'
#define CTRIS_HIGHER_CHAR 'C'
#define TRIS_LOWER_CHAR 'a'
#define TRIS_HIGHER_CHAR 'c'
//Tris defines </>
//Scrematura defines
#define SCREMATURA_MAX_NUM_INPUT 3
#define HIGHER_NUM_LIMIT 1000
#define ACQUISSIMA_CAP 200
#define ACQUA_AND_FUOCO_CAP 100
#define FUOCHISSIMO_CAP 30
//Scrematura defines </>
//Biglie defines
#define BIGLIE_MAX_INPUT 7
#define STARTING_POINTS 10
#define BPARI "pari"
//Biglie defines </>
//Murra defines
#define HUMANFINGERS 5
#define MURRA_TOP_EX 9
//Murra defines </>
//Pari o dispari defines
#define POD_MAX_INPUT 7
#define POD_TOP_VALUE 5
#define POD_PARI "pari"
#define POD_DISPARI "dispari"
//Pari o dispari defines </>

//All game defines

//List of games
typedef enum {
    DADI,
    BIGLIE,
    MORRACINESE,
    MURRA,
    PARI_O_DISPARI,
    TRIS,
    IMPICCATO,
    PONTE_DI_VETRO,
} games;
//List of games </>

#if defined WIN32 || defined WIN64
#define sleep(x) Sleep((x) * 1000)
#elif defined __linux__ || defined __APPLE__
#define sleep(x) sleep(x)
#endif

#define DECIMAL_ 10
#define ASCII_CODE_1 49


#define MAXNAME_LENGTH 64
#define GROUP_MAX_SIZE 4

#define GARBAGE (-1)

#define NEWLINE "\n"
#define NEWLINE_FUNC printf("\n");

#define UI_MESSAGE_V1 ">>"
#define UI_MESSAGE_V2 ":"

#define ENDSTRING '\0'

#define SPACE_STRING " "

#define GAMES "DADI", "BIGLIE", "MORRACINESE", "MURRA", "PARI O DISPARI", "TRIS", "IMPICCATO", "PONTE DI VETRO"
#define SCREMATURA "SCREMATURA"
#define BLACKJACK_NAME "BLACKJACK"

#define PLAYER_NAME_MIN_LEN 4

#define FRONTMAN "Riccardo Scateni"
#define STOP_INSERT_PNAMES "stop"

#define MINPLAYER 16

#define UNIDENTIFIED '\r'

typedef struct {
    int id;
    char name[MAXNAME_LENGTH];
    int wins;
    int played;
    int finals;
    int tgp;
    int tgw;
} player;

typedef struct {
    char *name;
    int id;
} playerArray;

typedef struct Player_List {
    struct Player_List *prev;
    char name[MAXNAME_LENGTH];
    struct Player_List *next;
} playerList;

typedef enum {
    SURVIVOR_OF_SCREMATURA,
    CLASSIC_GAME,
    FINAL
} stats;

//All game defines </>


void gameTypeStamp(const char gameName[], games game);
void winner_stamp(const char name[]);
playerList* listCreator(playerArray pArray[]);
char* listReader(playerList *list, _Bool reset);
void listRemover(playerList **list, char player[]);
void freeList(playerList *list);

playerArray **arrayGenerator(int *array, player *players, int size, int dim, int numPlayers, int listIds[]);

void optionsForNewSavings(int *k, player **p);
void gameSetup(int numPlayers, player **p, int *numEntities, int *numPlayingPlayers, int **pIds, int **alivePlayers);
player **getPlayers(player **players);
int *getPlayersIds(int *playersIds);
int getNumOfActivePlayers(int playersNum);
int checkNumAlive(const int *array, int dim);

int gameManagement(playerArray *arrays, int **playingPlayers, games _game);
void increaseStats(playerArray *pArray, int size, stats type, int id);
void mainGames(int **pIds, player **p, int **alivePlayers, int N, int M, int k, int _fileElement);

#include "../portable_utils/utils.h"
#include "../portable_utils/colors.h"
#include "filemanagement.h"
#include "bots.h"
#include "scrematura.h"
#include "tris.h"
#include "paridispari.h"
#include "dadi.h"
#include "morracinese.h"
#include "biglie.h"
#include "murra.h"
#include "impiccato.h"
#include "ponte_vetro.h"
#include "blackjack.h"

//cString conditions list
#define CS_CON_GCOUNTLIMIT "(1|8)"
#define CS_CON_PNAMEALLOWEDCHARS "(A|Z)(a|z)(.._ )(0->)"
#define CS_CON_PMINTOMAXNUM "(16->)"
#define CS_CON_USERACCOUNTSPLAYING "(1->)"
#define CS_CON_SCMINTOMAXNUM "(0|999)"
#define CS_CON_FILENUM "(1->)"
#define CS_CON_ALLOWEDFILENAMECHARS "(A|Z)(a|z)(.._)"
#define CS_CON_ALLOWEDCHARSFORFILEOPTIONS "(..oldnewOLDNEW)"
#define CS_CON_SAVINGOPTION "(1|3)"
#define CS_CON_BIGLIEMINMAXINPUT "(1|19)"
#define CS_CON_BIGLIECHOICEALLOWEDCHARS "(..dispariDISPARI)"
#define CS_CON_BLACKJACKCHOICES "(A|Z)(a|z)"
#define CS_CON_MORRACINESECHOICES "(A|Z)(a|z)"
#define CS_CON_MURRARANGE "(1|5)"
#define CS_CON_MURRATOTALRANGE "(2|10)"
#define CS_CON_PODCHOICEALLOWEDCHARS "(..dispariDISPARI)"
#define CS_CON_PODRANGE "(0|5)"
#define CS_CON_TRISALLOWEDINS "(A|C)(a|c)(1|3)"
#define CS_CON_IMPICCATOMODE "(A|Z)(a|z)"
#define CS_CON_IMPICCATOGAME "(A|Z)(a|z)(..^!^.?;: ,\')"
#define CS_CON_PONTEDIVETROMOVES "(..<^>)"
//cString conditions list </>
//sString comparisons
#define SS_COM_FILEOPTIONS "old;new;"
#define SS_COM_PARIDISPARI "pari;dispari;"
#define SS_COM_BLACKJACK "resto;pesco;"
#define SS_COM_MORRACINESE "sasso;carta;forbici;"
#define SS_COM_IMPICCATOMODES "classic;adapted;"
//sString comparisons </>
//sString commands
#define SS_COMM_NOCREMSPACES "nocaps;raspaces;"
//sString commands </>

#endif //FINAL_PROJECT_STRUCTURES_H

