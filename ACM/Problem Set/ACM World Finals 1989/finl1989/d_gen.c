/*
        Input generator

        Problem:        D
        Author:         Ondrej Karny
        Date:           1998-Mar-31
*/
/*
 * ACM International Collegiate Programming Contest Finals 1989
 *
 * Problem D - Running Lights Visibility Calculator
 *
 * Input generator
 *
 * Format of the single input:
 *
 * name of the dataset 50 chars max
 * number of other ships
 *  own ship name, 25 chars max
 *  location x location y course in degrees speed
 *  other ship name, 25 chars max
 *  location x location y course in degrees speed
 *
 * max 4000 ships in single set
 *
 */


#include <stdio.h>
#include <math.h>
#include <assert.h>
#define ASSERT( condition ) assert( condition )


/*
 * This code taken from MSVC CRT Source.
 *
 */
unsigned int randSeed;

void srand( unsigned int seed )
{
randSeed = seed;
}

unsigned int rand( void )
{
randSeed = randSeed * 214013 + 2531011;
return ( randSeed >> 16 ) & 0x7fff;
}

#define MAX_RAND 32767


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
    case 0 :                /* heavy spaces - 25 chars */
        fprintf( stdout, "  " );
        subGenString( 8 );
        fprintf( stdout, "      " );
        subGenString( 7 );
        fprintf( stdout, "  " );
        break;
    case 1 :                /* some spaces - 25 chars */
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


static double generate( void )
{
return (double)rand() / (double)MAX_RAND;
}



#ifndef M_PI
#ifdef PI
#define M_PI PI
#else  /*ndef PI*/
#define M_PI 3.14159265358979323846
#endif /*ndef PI*/
#endif /*ndef M_PI*/

#define EPSILON 0.0001

#define RADIANS( angle ) (( angle ) * M_PI / 180.0 )
#define DEGREES( angle ) (( angle ) * 180.0 / M_PI )

#define MAX_DELTA    RADIANS( 2.0 )
#define MAX_DISTANCE 10.0

typedef struct _Ship {
    double x;
    double y;              /*location*/
    double course;         /*angle in radians*/
    double speed;          /*current speed*/
    double bearing;        /*bearing from the ship to me*/
    double distance;       /*initial distance*/
    } Ship;

static Ship mineShip;
static Ship moveShip;
static Ship otherShip;

/*
 * Let the ship 3 minutes move.
 *
 */
static void translateShip( Ship *ship )
{
ship->x += 0.05 * ship->speed * sin( ship->course );
ship->y += 0.05 * ship->speed * cos( ship->course );
}

/*
 * Compute relative bearing from one
 * ship to another. Store the bearing
 * in fromShip. Also compute distance
 * and store it.
 *
 */
static void bearingFromTo( Ship *fromShip, const Ship *toShip )
{
double dx;
double dy;
double bearing;

dx = toShip->x - fromShip->x;
dy = toShip->y - fromShip->y;
fromShip->distance = sqrt( dx * dx + dy * dy );
ASSERT(( fabs( dx ) > EPSILON ) || ( fabs( dy ) > EPSILON ));
bearing = atan2( dx, dy );
if ( bearing < 0.0 ) {
    bearing += 2 * M_PI;
    }
ASSERT( bearing >= 0.0 );
ASSERT( bearing < 2 * M_PI );
bearing -= fromShip->course;
if ( bearing < 0.0 ) {
    bearing += 2 * M_PI;
    }
else if ( bearing >= 2 * M_PI ) {
    bearing -= 2 * M_PI;
    }
ASSERT( bearing >= 0.0 );
ASSERT( bearing < 2 * M_PI );
fromShip->bearing = bearing;
}


double convert( double value )
{
char buffer[128];

sprintf( buffer, "%f", value );
sscanf( buffer, "%lf", &value );
return value;
}


int main( int argc, const char **argv )
{
int i;

/* trivial example */
fprintf( stdout,
    "Sample Test Data Set\n"
    "4\n"
    "Ownship\n"
    "0.0 0.0 90.0 10.0\n"
    "Windswept\n"
    "10.0 10.0 135.0 8.0\n"
    "Footloose\n"
    "-5.0 6.0 275.0 6.0\n"
    "Crasher\n"
    "0.0 9.0 135 14.14\n"
    "Aquavit\n"
    "-2.0 -2.0 294.0 15.0\n" );

/* zero otherships */
fprintf( stdout,
    " Zero other ships\n"
    "0\n"
    " mine ship\n"
    "135.0 134.0 120.0 17.0\n" );

/* 50 characters of garbage */
fprintf( stdout, "%s\n", fifty );
/* 4000 of other ships will test the implementation */
fprintf( stdout, "4000\n" );
/* 25 chars of garbage as own ship name */
fprintf( stdout, "%s\n", twentyFive );
fprintf( stdout, "0.0 0.0 0.0 1.0\n" );
for ( i = 0; i < 4000; i++ ) {
    /* names of other ships are simply garbage */
    fprintf( stdout, "%04d%s\n", i, twentyOne );
    /* all the ships are initially invisble, so no warning should be issued */
    fprintf( stdout, "10.0 0.0 270.0 %f\n", (double)i / 40.0 );
    }

/* 50 characters of garbage */
fprintf( stdout, "%s\n", fifty );
/* 4000 of other ships will test the implementation */
fprintf( stdout, "4000\n" );
/* 25 chars of garbage as own ship name */
fprintf( stdout, "%s\n", twentyFive );
fprintf( stdout, "0.0 0.0 0.0 1.0\n" );
for ( i = 0; i < 4000; i++ ) {
    /* names of other ships are simply garbage */
    fprintf( stdout, "%04d%s\n", i, twentyOne );
    /* the last ship goes parallel to mine - but the warning
       shouldn't be issued because the distance is the same,
       doesn't decrease. Other ships goes too fast to issue
       warning - tests the "distance decreases" condition */
    fprintf( stdout, "%f 0.0 0.0 %d\n",
            10.0 - ((double)i / 400.0 ), 4000 - i );
    }

/* and some random stuff here */
srand( 137 );

for ( i = 0; i < 3; i++ ) {
    unsigned int ships;
    double x,y,c,s;

    generateString( 50 );
    fprintf( stdout, "\n" );
    ships = ( rand() % 4000 ) + 1;
    fprintf( stdout, "%u\n", ships );
    fprintf( stdout, "%s\n", twentyFive );
    x = generate() * 20.0 - 10.0;
    y = generate() * 20.0 - 10.0;
    c = generate() * 360.0;
    s = generate() * 20.0;
    mineShip.x = convert( x );
    mineShip.y = convert( y );
    mineShip.course = RADIANS( convert( c ));
    mineShip.speed = convert( s );
    moveShip.x = convert( x );
    moveShip.y = convert( y );
    moveShip.course = RADIANS( convert( c ));
    moveShip.speed = convert( s );
    translateShip( &moveShip );
    fprintf( stdout, "%f %f %f %f\n", x, y, c, s );
    while ( ships-- ) {
        generateString( 25 );
        fprintf( stdout, "\n" );
        while ( 1 ) {
            x = generate() * 20.0 - 10.0;
            y = generate() * 20.0 - 10.0;
            c = generate() * 360.0;
            s = generate() * 20.0;
            otherShip.x = convert( x );
            otherShip.y = convert( y );
            otherShip.course = RADIANS( convert( c ));
            otherShip.speed = convert( s );
            bearingFromTo( &otherShip, &mineShip );

            {
            double result;
            double above;
            double beyond;

#define tworound( v ) floor(( v ) * 100.0 + 0.5 )

            result = DEGREES( otherShip.bearing );
            above = tworound( DEGREES( otherShip.bearing ) + EPSILON );
            beyond = tworound( DEGREES( otherShip.bearing ) - EPSILON );
            if ( tworound( result ) != above ) continue;
            if ( tworound( result ) != beyond ) continue;
            }   /*eob*/
            if ( otherShip.distance >= MAX_DISTANCE ) break;
            {
            double oldBearing;

            oldBearing = otherShip.bearing;
            translateShip( &otherShip );
            bearingFromTo( &otherShip, &moveShip );
            oldBearing = fabs( oldBearing - otherShip.bearing );
            if (( oldBearing < MAX_DELTA - EPSILON ) ||
                ( oldBearing > MAX_DELTA + EPSILON )) break;
            }   /*eob*/
            }
        fprintf( stdout, "%f %f %f %f\n", x, y, c, s );
        }
    }

return 0;
}

