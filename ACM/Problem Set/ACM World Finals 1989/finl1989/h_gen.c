/*
        Input generator

        Problem:        H
        Author:         Ondrej Karny
        Date:           1998-Apr-03
*/
/*
 * ACM International Collegiate Programming Contest Finals 1989
 *
 * Problem H - PGA Tour Prize Money
 *
 * Input generator
 *
 * Format of the single input:
 *
 * number of players in game (1-144, zero for end)
 * tour purse
 * 1st place share
 * ..
 * 70th place share
 * player entry - name, scores0-3
 *
 */


#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#define ASSERT( condition ) assert( condition )

unsigned int uintShares[70];
double doubleShares[70];

static void generateShares( unsigned int number, unsigned int sum )
{
int i;
double c;
unsigned int s;

/* now generate some deltas */
for ( i = 0; i < number; i++ ) {
    doubleShares[i] = (double)(rand() % 7000 ) / 7000.0;
    }
/* then convert from deltas into sequence */
c = 0.0;
for ( i = number; i; ) {
    i--;
    doubleShares[i] += c;
    c = doubleShares[i];
    }
/* now compute current sum */
c = 0.0;
for ( i = 0; i < number; i++ ) {
    c += doubleShares[i];
    }
/* normalize sequence into integers and sum to sum */
for ( i = 0; i < number; i++ ) {
    uintShares[i] = (unsigned int)(( doubleShares[i] / c ) * sum );
    }
s = 0;
for ( i = 0; i < number; i++ ) {
    s += uintShares[i];
    }
ASSERT( s >= sum - number );
ASSERT( s <= sum + number );
uintShares[0] += sum - s;
/* FIXME: ale co kdyz uintShares[0] < uintShares[1] ?? */
for ( i = 0; i < number; i++ ) {
    fprintf( stdout, "%u.%04u\n", uintShares[i] / 10000,
            uintShares[i] % 10000 );
    }
}





typedef struct _Player {
    char name[21];
    int  score[4];
    } Player;

Player players[144];
Player *order[144];
unsigned int numberOfPlayers;
unsigned int numberOfDisqualified;  /*FACT: in first two rounds*/



static void putPlayerName( unsigned int playerId, const char *string )
{
char *buffer;
int i;

ASSERT( playerId < 144 );
buffer = players[playerId].name;
for ( i = 0; i < 20; i++ ) {
    if ( *string ) {
        *buffer++ = *string++;
        }
    else {
        break;
        }
    }
while ( i < 21 ) {
    *buffer++ = '\0';
    i++;
    }
ASSERT( buffer == players[playerId].name + 21 );
}

char *stringPtr;

static void subGenString( unsigned int length )
{
while ( length ) {
    char ch;

    ch = (char)( rand() % 95 ) + 33;
    *stringPtr++ = ch;
    length--;
    }
}

static void generateString( unsigned int maxLength )
{
int length;

length = rand() % 5;
switch ( length ) {
    case 0 :                /* heavy spaces 2 + 7 + 2 + 7 + 2 = 20*/
        *stringPtr++ = ' ';
        *stringPtr++ = ' ';
        subGenString( 7 );
        *stringPtr++ = ' ';
        *stringPtr++ = ' ';
        subGenString( 7 );
        *stringPtr++ = ' ';
        *stringPtr++ = ' ';
        break;
    case 1 :                /* some spaces 1 + 9 + 1 + 9 = 20*/
        *stringPtr++ = ' ';
        subGenString( 9 );
        *stringPtr++ = ' ';
        subGenString( 9 );
        break;
    case 2 :                /* single char */
        subGenString( 1 );
        break;
    case 3 :                /* full size */
        subGenString( maxLength );
        break;
    case 4 :                /* random size */
        length = ( rand() % maxLength ) + 1;
        subGenString( length );
        break;
    }
}


static void putPlayerScore( unsigned int playerId,
            int sc1, int sc2, int sc3, int sc4 )
{
int *scores;

ASSERT( playerId < 144 );
scores = players[playerId].score;
*scores++ = sc1;
*scores++ = sc2;
*scores++ = sc3;
*scores++ = sc4;
}


static void generateScore( unsigned int playerId, int half, int full )
{
int sc1;
int sc2;
int sc3;
int sc4;

sc1 = rand() % 200 + 36;
sc2 = rand() % 200 + 36;
sc3 = rand() % 200 + 36;
sc4 = rand() % 200 + 36;
if ( ( rand() % 5 ) == 0 ) {
    switch ( rand() % 4 ) {
        case 0 :
            sc1 = -1;
        case 1 :
            sc2 = -1;
            numberOfDisqualified++;
        case 2 :
            sc3 = -1;
        case 3 :
            sc4 = -1;
            break;
        }
    }
else {
    if ( sc1 + sc2 < half ) {
        int rest;

        rest = half - ( sc1 + sc2 );
        sc1 += rest >> 1;
        sc2 += rest >> 1;
        }
    if ( sc1 + sc2 + sc3 + sc4 <= full ) {
        int rest;

        rest = full - ( sc1 + sc2 + sc3 + sc4 );
        sc3 += rest >> 1;
        rest -= rest >> 1;
        sc4 += rest;
        }
    }
putPlayerScore( playerId, sc1, sc2, sc3, sc4 );
}



void outputPlayers( void )
{
int playerId;

for ( playerId = 0; playerId < numberOfPlayers; playerId++ ) {
    Player *player;
    char score[4*4];
    int i;

    player = order[playerId];
    score[0] = '\0';
    score[4] = '\0';
    score[8] = '\0';
    score[12] = '\0';
    for ( i = 0; i < 4; i++ ) {
        if ( player->score[i] > 0 ) {
            sprintf( score + i * 4, "%d", player->score[i] );
            }
        else {
            ASSERT( player->score[i] == -1 );
            strcpy( score + i * 4, "DQ" );
            break;
            }
        }
    fprintf( stdout, "%-20s %-3s %-3s %-3s %-3s\n",
            player->name, score + 0, score + 4, score + 8, score + 12 );
    }
}


void addAmateurs( void )
{
int playerId;

for ( playerId = 0; playerId < 144; playerId++ ) {
    if (( rand() % 30 ) == 7 ) {
        char *name;

        name = players[playerId].name + 20;
        while ( *name == '\0' ) {
            name--;
            }
        *name = '*';
        ASSERT( name >= players[playerId].name );
        ASSERT( name <  players[playerId].name + 20 );
        }
    }
}


void shufflePlayers( void )
{
int i;

for ( i = 0; i < 600; i++ ) {
    int from;
    int to;
    Player *temp;

    from = rand() % 143 + 1;
    to = rand() % 143 + 1;
    temp = order[from];
    order[from] = order[to];
    order[to] = temp;
    }
}




int main( int argc, const char **argv )
{
int i;

srand( 137 );

putPlayerName( 0, "WALLY WEDGE" );   /* always winner */
putPlayerName( 1, "SANDY LIE" );
putPlayerName( 2, "SID SHANKER*" );  /* always amateur */
putPlayerName( 3, "JIMMY ABLE" );
/* rest of names is random generated */
for ( i = 4; i < 144; i++ ) {
    stringPtr = players[i].name;
    generateString( 20 );
    }

putPlayerScore( 0, 70, 70, 70, 70 );
putPlayerScore( 1, 80, -1, -1, -1 );
putPlayerScore( 2, 100, 99, 62, 61 );
putPlayerScore( 3, 69, 73, 80, -1 );
numberOfDisqualified = 1;
/* rest of scores is random generated */
for ( i = 4; i < 144; i++ ) {
    generateScore( i, 140 + 30, 280 );
    }
numberOfPlayers = ( rand() % ( 75 - numberOfDisqualified )) +
                  ( 70 + numberOfDisqualified );

/* trivial example */
fprintf( stdout,
    "%u\n"
    "1000000.00\n"
    "18.0000\n"      /* 1 */
    "10.8000\n"
     "6.8000\n"
     "4.8000\n"
     "4.4000\n"      /* 5 */
     "4.2000\n"
     "4.1000\n"
     "4.0500\n"
     "4.0300\n"
     "4.0100\n",     /* 10 */
    numberOfPlayers );
generateShares( 70 - 10, 1000000 - 651900 );  /* rest is random */

for ( i = 0; i < 144; i++ ) {
    order[i] = players + i;
    }
outputPlayers();

for ( i = 0; i < 60; i++ ) {
    addAmateurs();
    numberOfPlayers = ( rand() % ( 75 - numberOfDisqualified )) +
                      ( 70 + numberOfDisqualified );
    fprintf( stdout, "%u\n"
                     "40000000.00\n",
                     numberOfPlayers );  /*and purse*/
    generateShares( 70, 100 * 10000 );
    shufflePlayers();
    outputPlayers();
    }
fprintf( stdout, "0\n" );   /* end of input */
return 0;
}

