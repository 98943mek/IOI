/*
        Input generator

        Problem:        G
        Author:         Ondrej Karny
        Date:           1998-Mar-30
*/
/*
 * ACM International Collegiate Programming Contest Finals 1989
 *
 * Problem G - Meals on Wheels Routing System
 *
 * Input generator
 *
 * Format of the single input:
 *
 * name of the dataset 50 chars max
 * number of routes number of customers
 *  customer's name 25 chars max
 *  location x location y
 *
 * max 4000 customers in single set
 */


#include <stdio.h>
#include <math.h>
#include <stdlib.h>

const char fifty[51] = "%s%s%d%f%f%d%h%g%das%das%d%*f%%%3df$dad$%f%f%f%f%s";
const char twentyFive[26] = "%*f%f**%g%s%d%i%d%*d%e%f%";
const char twentyOne[22] = "%*f%f**%g%s%d%i%d%*d%";

static void subGenString( unsigned int length )
{
while ( length ) {
    char ch;

    ch = (char)( rand() % 96 ) + 32;
    fprintf( stdout, "%c", ch );
    length--;
    }
}

static void generateString( unsigned int maxLength )
{
int length;

length = rand() % 5;
switch ( length ) {
    case 0 :                /* heavy spaces 2 + 8 + 6 + 7 + 2 = 25! */
        fprintf( stdout, "  " );
        subGenString( 8 );
        fprintf( stdout, "      " );
        subGenString( 7 );
        fprintf( stdout, "  " );
        break;
    case 1 :                /* some spaces 1 + 11 + 1 + 11 + 1 = 25! */
        fprintf( stdout, " " );
        subGenString( 11 );
        fprintf( stdout, " " );
        subGenString( 11 );
        fprintf( stdout, " " );
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


int main( int argc, const char **argv )
{
int i;

/* trivial example */
fprintf( stdout,
    "Sample Route List\n"
    "4 10\n"
    "able\n"
    "1 2\n"
    "baker\n"
    "-3 6\n"
    "charlie\n"
    "-4 -5\n"
    "donald\n"
    "4 -7\n"
    "eloise\n"
    "3 4\n"
    "frank\n"
    "2 2\n"
    "gertrude\n"
    "5 9\n"
    "horace\n"
    "-2 -5\n"
    "inez\n"
    "5 -3\n"
    "james\n"
    "0 1\n" );

/* 50 characters of garbage, will catch printf( dataSetName ) error */
fprintf( stdout, "%s\n", fifty );
/* 4000 entries will catch underdimensed arrays */
fprintf( stdout, "4000 4000\n" );
for ( i = 0; i < 4000; i++ ) {
    /* 21 characters of garbage, will catch printf( customerName ) error */
    fprintf( stdout, "%04d%s\n", i, twentyOne );
    /* very large numbers as locations will catch improper length evaluation*/
    /* all locations the same will test the sorting order */
    fprintf( stdout, "10000 10000\n" );
    }

fprintf( stdout, "%s\n", fifty );
fprintf( stdout, "4000 4000\n" );
for ( i = 4000; i; ) {
    i--;
    fprintf( stdout, "%s%04d\n", twentyOne, i );
    /* paraboloid, will catch proper polar angle of origin as well
       as (reverse) sorting */
    fprintf( stdout, "%d %d\n", i, ( i * i ) >> 6 );
    }

/* spaces in dataset name will catch parsing errors */
fprintf( stdout, "  %s  %s\n", twentyOne, twentyFive );
/* non-equal division of customers to routes */
fprintf( stdout, "3997 4000\n" );
for ( i = 0; i < 4000; i++ ) {
    /* spaces in customer names will catch error in parsing */
    fprintf( stdout, "%s%4d\n", twentyOne, i );
    /* spiral will stress sorting a bit more */
    fprintf( stdout, "%d %d\n", (int)( sin( (double)i ) * i ),
                                (int)( cos( (double)i ) * i ));
    }

/* now generate some random inputs */
srand( 137 );

for ( i = 0; i < 10; i++ ) {
    unsigned int routes;
    unsigned int customers;

    generateString( 50 );
    fprintf( stdout, "\n" );
    customers = ( rand() % 4000 ) + 1;
    routes = ( rand() % customers ) + 1;
    fprintf( stdout, "%u %u\n", routes, customers );
    while ( customers ) {
        int x;
        int y;

        generateString( 25 );
        fprintf( stdout, "\n" );
        x = ( rand() % 10000 ) - 5000;
        y = ( rand() % 10000 ) - 5000;
        fprintf( stdout, "%d %d\n", x, y );        
        customers--;
        }
    }

return 0;
}

