/*
        Problem:        H
        Description:    sort players
        Class:          database
        Subclass:       sorting
        Algorithm:      heapsort
        Author:         Ondrej Karny
        Date:           1998-Apr-01
*/
/*
 * ACM International Collegiate Programming Contest Finals 1989
 *
 * Problem H - PGA Tour Prize Money
 *
 * Example solution
 *
 * Notes:
 *
 * There are some fixes in original problem description. Most of them
 * concerns input and output formatting. Also enhanced input specification
 * to support multiple problems in one file. There were also some
 * inconsistencies in example output table with ties and places.
 *
 * Algorithm:
 *
 * for each tournament
 *  read number of players
 *  read purse
 *  read place assignments
 *  read all the players
 *  sort the players based on their 36-hole score
 *  make the first cut
 *  sort the plauers based on their 72-hole score
 *  assign money
 *  write out the results
 *
 * Uses only sorting and data management.
 *
 * Special accuracy note:
 *
 * I treat all the values during evaluation as integers, so no
 * imperfections should happen.
 *
 */

#include <stdio.h>

#include <string.h>

#ifdef DEBUG
#include <assert.h>
#define ASSERT( condition ) assert( condition )
#else
#define ASSERT( condition )
#endif

#define MAX_PLAYERS 144
#define MAX_PLACES  70
#define MAX_NAME    20
#define MAX_LINE    128

#define POSITION_ROUND_0 (22-1)
#define POSITION_ROUND_1 (26-1)
#define POSITION_ROUND_2 (30-1)
#define POSITION_ROUND_3 (34-1)


typedef struct _Player {
    unsigned int number;        /*number in input set*/
    char         name[MAX_NAME+1]; /*player's name*/
    int          amateur;       /*is the player amateur?*/
    int          score[4];      /*-1 means disqualified in the n-th round*/
    int          twoScore;      /*two rounds score*/
    int          fourScore;     /*four rounds score*/
    int          place;         /*finish place, negative if part of a tie*/
    unsigned int money;         /*money won*/
    } Player;

Player       players[MAX_PLAYERS];     /*database of players*/
unsigned int numberOfPlayers;

Player      *playerArray[MAX_PLAYERS]; /*used for sorting*/
unsigned int numberOfSortable;

unsigned int purse;             /*total tournament prize money*/
unsigned int share[MAX_PLACES]; /*share on purse*/

static int finalSort = 2;


/*
 * Compare two Player records. Returns
 * non-zero if the first one ('left') is
 * less in the aforementioned
 * order than the second one ('right').
 * In other words, operator <.
 * Uses the variable finalSort,
 * that will tell us whether we are
 * sorting for first cut only or for
 * finals.
 *
 */
static int playerIsLess( const Player *left, const Player *right )
{
int leftScore;
int rightScore;

if ( finalSort ) {
    ASSERT( finalSort == 1 );
    leftScore = left->fourScore;
    rightScore = right->fourScore;
    }
else {
    leftScore = left->twoScore;
    rightScore = right->twoScore;
    }
if ( leftScore == rightScore ) {
    if ( left->number < right->number ) {
        return 1;
        }
    else {
        ASSERT( left->number > right->number );
        return 0;
        }
    }
if ( leftScore == -1 ) {
    return 0;
    }
if ( rightScore == -1 ) {
    return 1;
    }
if ( leftScore < rightScore ) {
    return 1;
    }
else {
    ASSERT( leftScore > rightScore );
    return 0;
    }
}


/*
 * Heapsort the playerArray[], uses only
 * first numberOfSortable slots.
 *
 */
static void sortPlayers( void )
{
unsigned int index;

for ( index = 1; index < numberOfSortable; index++ ) {
    unsigned int bubbleIndex;
    Player *bubbleElement;

    bubbleIndex = index;
    bubbleElement = playerArray[bubbleIndex];
    while ( bubbleIndex > 0 ) {
        unsigned int parentIndex;

        parentIndex = ( bubbleIndex - 1 ) >> 1;
        if ( !playerIsLess( bubbleElement,
                    playerArray[parentIndex] )) {
            playerArray[bubbleIndex] = playerArray[parentIndex];
            bubbleIndex = parentIndex;
            }
        else {
            break;
            }
        }
    playerArray[bubbleIndex] = bubbleElement;
    }
#ifdef DEBUG
for ( index = 1; index < numberOfSortable; index++ ) {
    ASSERT( playerIsLess( playerArray[index],
                 playerArray[(index-1)>>1] ));
    }
#endif /*def DEBUG*/
for ( index = numberOfSortable - 1; index; ) {
    Player *bubbleElement;
    unsigned int bubbleIndex;
    unsigned int sonIndex;

    bubbleElement = playerArray[index];
    playerArray[index] = playerArray[0];
    bubbleIndex = 0;
    index--;
    while ( sonIndex = ( 2 * bubbleIndex ) + 1, sonIndex <= index ) {
        if ( sonIndex < index ) {
            if ( playerIsLess( playerArray[sonIndex],
                playerArray[sonIndex+1] )) {
                sonIndex++;
                }
            }
        if ( !playerIsLess( playerArray[sonIndex], bubbleElement )) {
            playerArray[bubbleIndex] = playerArray[sonIndex];
            bubbleIndex = sonIndex;
            }
        else {
            break;
            }
        }
    playerArray[bubbleIndex] = bubbleElement;
    }
#ifdef DEBUG
for ( index = 0; index < numberOfSortable - 1; index++ ) {
    ASSERT( playerIsLess( playerArray[index],
                playerArray[index+1] ));
    }
#endif /*def DEBUG*/
}


/*
 * For all players compute their final
 * position. Also resolve ties.
 *
 */
static void computePlaces( void )
{
unsigned int playerId;
Player *previous;

previous = playerArray[0];
previous->place = 1;
playerId = 1;
while ( playerId < numberOfSortable ) {
    Player *player;

    player = playerArray[playerId];
    playerId++;
    if ( previous->fourScore == player->fourScore ) {
        int place;

        place = previous->place;
        if ( place > 0 ) {
            place = -place;
            previous->place = place;
            }
        player->place = place;
        }
    else {
        player->place = (int)playerId;
        }
    previous = player;
    }
}


/*
 * Compute part of the tournament's purse.
 * Does use special tricks to maintain
 * maximum accuracy possible.
 * The idea is that it should return
 *  purse * percentage / ( 100 * 10000 )
 *
 */
static unsigned int computeMoney( unsigned int percentage )
{
double total;

total = (double)purse * (double)(percentage);
total /= (double)( 100 * 10000 );
return (unsigned int)total;
}


/*
 * Sort the playerArray[numberOfSortable].
 * The make the first cut.
 * Update the numberOfSortable.
 * Sort the playerArray again, based
 * on the fourScore this time.
 * Assign the money.
 * Output results.
 *
 */
void evaluate( void )
{
finalSort = 0;
sortPlayers();
ASSERT( numberOfSortable >= MAX_PLACES );
{
unsigned int lastPlayer;
int lastScore;

lastPlayer = MAX_PLACES - 1;
lastScore = playerArray[lastPlayer]->twoScore;
ASSERT( lastScore >= 0 );
lastPlayer++;
while (( lastPlayer < numberOfSortable ) &&
       ( playerArray[lastPlayer]->twoScore == lastScore )) {
    lastPlayer++;
    }
ASSERT( lastPlayer <= numberOfSortable );
ASSERT( playerArray[lastPlayer-1]->twoScore == lastScore );
ASSERT( lastPlayer >= MAX_PLACES );
numberOfSortable = lastPlayer;
}   /*eob*/
finalSort = 1;
sortPlayers();
#ifdef DEBUG
ASSERT( playerArray[0]->fourScore != -1 );
if ( numberOfSortable > 1 ) {
    if ( playerArray[1]->fourScore != -1 ) {
        ASSERT( playerArray[0]->fourScore < playerArray[1]->fourScore );
        }
    }
#endif /*def DEBUG*/
computePlaces();
{
unsigned int moneyId;
unsigned int playerId;

playerId = 0;
moneyId = 0;
while ( playerId < numberOfSortable ) {
    if ( playerArray[playerId]->place >= 0 ) {
        Player *player;

        player = playerArray[playerId];
        ASSERT( player->place );
        if ( !(player->amateur)) {
            player->money = computeMoney( share[moneyId++] );
            }
        playerId++;
        }
    else {
        Player **tieBegin;
        unsigned int tieSize;
        unsigned int tieShare;
        unsigned int tieProfis;
        {
        int place;

        place = playerArray[playerId]->place;
        tieBegin = playerArray + playerId;
        tieSize = 1;
        tieProfis = ( playerArray[playerId]->amateur ? 0 : 1 );
        playerId++;
        while ( playerId < numberOfSortable ) {
            if ( playerArray[playerId]->place != place ) {
                break;
                }
            tieProfis += ( playerArray[playerId]->amateur ? 0 : 1 );
            tieSize++;
            playerId++;
            }
        }   /*eob*/
        if ( tieProfis ) {
            {
            unsigned int tieId;

            tieShare = 0;
            for ( tieId = tieProfis; tieId; tieId-- ) {
                tieShare += share[moneyId++];
                }
            }   /*eob*/
            {
            unsigned int money;

            money = computeMoney( tieShare );
            money = money / tieProfis;
            while ( tieSize ) {
                (*tieBegin)->money = money;     /* /TRICK: even amateurs...*/
                tieBegin++;
                tieSize--;
                }
            }   /*eob*/
            }
        }
    }
}   /*eob*/
}


/*
 * Print nicely formatted table - the final
 * results, including all the scores
 * and money won. Uses playerArray[]
 * as well as numberOfSortable.
 *
 */
static void outputResult( void )
{
unsigned int playerId;

fprintf( stdout,
    "Player Name          Place RD1  RD2  RD3  RD4  TOTAL    Money Won\n"
    "-----------------------------------------------------------------\n" );
for ( playerId = 0; playerId < numberOfSortable; playerId++ ) {
    const Player *player;
    char stringPlace[5];
    char stringScore0[5];
    char stringScore1[5];
    char stringScore2[5];
    char stringScore3[5];
    char stringScoreTotal[6];
    char stringMoney[13];

    player = playerArray[playerId];
    ASSERT( player->score[0] != -1 );
    sprintf( stringScore0, "%d", player->score[0] );
    ASSERT( player->score[1] != -1 );
    sprintf( stringScore1, "%d", player->score[1] );
    stringScore2[0] = '\0';
    stringScore3[0] = '\0';
    if ( player->score[2] != -1 ) {
        sprintf( stringScore2, "%d", player->score[2] );
        if ( player->score[3] != -1 ) {
            sprintf( stringScore3, "%d", player->score[3] );
            }
        else {
            ASSERT( player->score[3] == -1 );
            }
        }
    if ( player->fourScore == -1 ) {
        stringPlace[0] = '\0';
        strcpy( stringScoreTotal, "DQ" );
        stringMoney[0] = '\0';
        }
    else {
        if ( player->place > 0 ) {
            sprintf( stringPlace, "%d", player->place );
            }
        else {
            ASSERT( player->place < 0 );
            sprintf( stringPlace, "%dT", -(player->place) );
            }
        sprintf( stringScoreTotal, "%d", player->fourScore );
        }
    if ( player->amateur ) {
        stringMoney[0] = '\0';
        }
    else {
        if ( player->fourScore != -1 ) {
            sprintf( stringMoney, "%d.%02d", player->money / 100,
                        player->money % 100 );
            }
        }
    fprintf( stdout, "%-20s %-5s %-4s %-4s %-4s %-4s %-5s %12s\n",
            player->name, stringPlace, stringScore0, stringScore1,
            stringScore2, stringScore3, stringScoreTotal,
            stringMoney );
    }
fprintf( stdout, "\n" );
}



static char buffer[MAX_LINE];

/*
 * Read single line from input
 * into buffer array.
 *
 */
static void getLine( void )
{
char *result;

result = fgets( buffer, MAX_LINE, stdin );
ASSERT( result );
}


/*
 * Read the first segment of input -
 * tournament purse and place-share percentages.
 *
 */
void readMoney( void )
{
int result;
unsigned int dollars;
unsigned int cents;
#ifdef DEBUG
unsigned int totalShare;
#endif
int placeId;

getLine();
result = sscanf( buffer, "%u.%u", &dollars, &cents );
ASSERT( result == 2 );
purse = dollars * 100 + cents;
#ifdef DEBUG
totalShare = 0;
#endif
for ( placeId = 0; placeId < MAX_PLACES; placeId++ ) {
    getLine();
    result = sscanf( buffer, "%u.%u", &dollars, &cents );
    share[placeId] = dollars * 10000 + cents;
#ifdef DEBUG
    totalShare += share[placeId];
#endif
    }
ASSERT( totalShare == 100 * 10000 );
}


/*
 * Read the second segment of input -
 * players, their scores and amateur
 * status.
 * Also computes their two and four rounds
 * cumulative score. All the non-disqualified
 * players are put into sort-array.
 *
 */
void readPlayers( void )
{
int result;
unsigned int playerId;
Player *player;

numberOfSortable = 0;
player = players;
for ( playerId = 0; playerId < numberOfPlayers; player++, playerId++ ) {
    getLine();
    memcpy( player->name, buffer, 20 );
    player->name[MAX_NAME] = '\0';
    player->number = playerId;
    {
    unsigned int asterisk;


    player->amateur = 0;
    for ( asterisk = MAX_NAME; asterisk; ) {
        asterisk--;
        if ( player->name[asterisk] == '*' ) {
            player->amateur = 1;
            break;
            }
        if ( player->name[asterisk] != ' ' ) {
            break;
            }
        }
    }   /*eob*/
    player->score[0] = -1;
    player->score[1] = -1;
    player->score[2] = -1;
    player->score[3] = -1;
    player->twoScore = -1;
    player->fourScore = -1;
    result = sscanf( buffer + POSITION_ROUND_0, "%d", player->score + 0 );
    if ( result == 1 ) {
        result = sscanf( buffer + POSITION_ROUND_1, "%d", player->score + 1 );
        if ( result == 1 ) {
            playerArray[numberOfSortable++] = player;
            player->twoScore = player->score[0] + player->score[1];
            result = sscanf( buffer + POSITION_ROUND_2, "%d",
                             player->score + 2 );
            if ( result == 1 ) {
                result = sscanf( buffer + POSITION_ROUND_3, "%d",
                                 player->score + 3 );
                if ( result == 1 ) {
                    player->fourScore = player->twoScore +
                                player->score[2] + player->score[3];
                    }
                else {
                    ASSERT( buffer[POSITION_ROUND_3+0] == 'D' );
                    ASSERT( buffer[POSITION_ROUND_3+1] == 'Q' );
                    }
                }
            else {
                ASSERT( buffer[POSITION_ROUND_2+0] == 'D' );
                ASSERT( buffer[POSITION_ROUND_2+1] == 'Q' );
                }
            }
        else {
            ASSERT( buffer[POSITION_ROUND_1+0] == 'D' );
            ASSERT( buffer[POSITION_ROUND_1+1] == 'Q' );
            }
        }
    else {
        ASSERT( buffer[POSITION_ROUND_0+0] == 'D' );
        ASSERT( buffer[POSITION_ROUND_0+1] == 'Q' );
        }
    }
ASSERT( numberOfSortable >= MAX_PLACES );
}



int main( int argc, const char **argv )
{
while ( 1 ) {
    int result;

    getLine();
    result = sscanf( buffer, "%u", &numberOfPlayers );
    ASSERT( result == 1 );
    if ( numberOfPlayers == 0 ) {
        break;
        }
    ASSERT( numberOfPlayers <= MAX_PLAYERS );
    readMoney();
    readPlayers();
    evaluate();
    outputResult();
    }
return 0;
}

