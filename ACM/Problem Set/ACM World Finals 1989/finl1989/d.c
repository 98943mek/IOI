/*
        Problem:        D
        Description:    compute translation vector
        Class:          geometric
        Subclass:       trigonometric
        Algorithm:      direct
        Author:         Ondrej Karny
        Date:           1998-Mar-30
*/
/*
 * ACM International Collegiate Programming Contest Finals 1989
 *
 * Problem D - Running Lights Visibility Calculator
 *
 * Example solution
 *
 * Note:
 *
 * Fixes in original problem description include clarification of the
 * lights visibility state and order, output format and input format
 * limits.
 *
 * Algorithm:
 *
 * for each scenario
 *  read mine ship data
 *  move the mine ship 3 minutes later
 *  output table heading
 *  for all ships
 *   read the ship from input
 *   compute visibility state
 *   compute lights order
 *   compute location after 3 minutes
 *   store in warning if necessary
 *   output table line
 *  for all warnings
 *   issue warning
 *
 * Uses only trivial functions.
 *
 * Note about precision:
 *
 * To maintain maximum accuracy, all computations are done
 * in doubles, with minimum operator usage. The angles
 * are stored in radians.
 *
 */

#include <stdio.h>

#include <string.h>

#include <math.h>

#ifndef M_PI
#ifdef PI
#define M_PI PI
#else  /*ndef PI*/
#define M_PI 3.14159265358979323846
#endif /*ndef PI*/
#endif /*ndef M_PI*/


#ifdef DEBUG
#include <assert.h>
#define ASSERT( condition ) assert( condition )
#else
#define ASSERT( condition )
#endif

#define EPSILON 0.0001

#define RADIANS( angle ) (( angle ) * M_PI / 180.0 )
#define DEGREES( angle ) (( angle ) * 180.0 / M_PI )

#define MAX_DISTANCE 10.0
#define MAX_SHIPS    4000
#define MAX_NAME     25
#define MAX_SETNAME  50
#define MAX_LINE     128
#define MAX_LONGLINE 1024

#define MAX_DELTA    RADIANS( 2.0 )

#define MIN_HALVE    0.0
#define MAX_RED      RADIANS( 2.5 )
#define MIN_STERN    RADIANS( 110.0 )
#define MAX_GREEN    RADIANS( 115.0 )
#define MAX_HALVE    M_PI
#define MIN_RED      RADIANS( 245.0 )
#define MAX_STERN    RADIANS( 250.0 )
#define MIN_GREEN    RADIANS( 357.5 )


#define NAME_STERN "Stern"
#define NAME_MAST  "Masthead"
#define NAME_RED   "Red"
#define NAME_GREEN "Green"


typedef struct _Ship {
    double x;
    double y;              /*location*/
    double course;         /*angle in radians*/
    double speed;          /*current speed*/
    double bearing;        /*bearing from the ship to me*/
    double distance;       /*initial distance*/
    char name[MAX_NAME+2]; /*25 chars + optional enter + terminating zero*/
    } Ship;


typedef struct _Warning {
    char name[MAX_NAME+1]; /*25 chars + terminating zero*/
    double distance;       /*end of interval distance*/
    } Warning;


static Ship mineShip;
static Ship moveShip;   /*mine ship 3 minutes later*/

static Warning warnings[MAX_SHIPS];  /*all the possible warnings*/
static unsigned int numberOfWarnings;


/*
 * Compute lights visibility and
 * return string description.
 *
 */
static const char *computeLights( const Ship *otherShip )
{
if ( otherShip->distance < MAX_DISTANCE ) {
    double bearing;

    bearing = otherShip->bearing;
    if ( bearing <= MAX_HALVE ) {
        if ( bearing >= MAX_GREEN ) {
            return NAME_STERN " " NAME_MAST;
            }
        if ( bearing > MIN_STERN ) {
            return NAME_STERN " " NAME_MAST " " NAME_GREEN;
            }
        if ( bearing >= MAX_RED ) {
            return NAME_MAST " " NAME_GREEN;
            }
        return NAME_GREEN " " NAME_MAST " " NAME_RED;
        }
    else {
        if ( bearing <= MIN_RED ) {
            return NAME_MAST " " NAME_STERN;
            }
        if ( bearing < MAX_STERN ) {
            return NAME_RED " " NAME_MAST " " NAME_STERN;
            }
        if ( bearing <= MIN_GREEN ) {
            return NAME_RED " " NAME_MAST;
            }
        return NAME_GREEN " " NAME_MAST " " NAME_RED;
        }
    }
else {
    return "Lights not visible";
    }
}


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


/*
 * Compute all results for single other ship
 * and print it to output. If warnings
 * needs to be issued, store necessary
 * data in array of warnings.
 *
 */
static void evaluateShip( Ship *otherShip )
{
const char *visibility;

bearingFromTo( otherShip, &mineShip );
visibility = computeLights( otherShip );
fprintf( stdout, "%-25s %-7.2f %-8.2f %s\n",
        otherShip->name, DEGREES( otherShip->bearing ),
        otherShip->distance, visibility );
#ifdef DEBUG
{
double result;
double above;
double beyond;

#define tworound( v ) floor(( v ) * 100.0 + 0.5 )

result = DEGREES( otherShip->bearing );
above = tworound( DEGREES( otherShip->bearing ) + EPSILON );
beyond = tworound( DEGREES( otherShip->bearing ) - EPSILON );
ASSERT( tworound( result ) == above );
ASSERT( tworound( result ) == beyond );
}   /*eob*/
#endif /*def DEBUG*/
if ( otherShip->distance < MAX_DISTANCE ) {
    double oldBearing;
    double oldDistance;

    oldBearing = otherShip->bearing;
    oldDistance = otherShip->distance;
    translateShip( otherShip );
    bearingFromTo( otherShip, &moveShip );
    oldBearing = fabs( oldBearing - otherShip->bearing );
    ASSERT(( oldBearing < MAX_DELTA - EPSILON ) ||
           ( oldBearing > MAX_DELTA + EPSILON ));
    if (( otherShip->distance < oldDistance ) &&
        ( oldBearing <= MAX_DELTA )) {
        ASSERT( numberOfWarnings < MAX_SHIPS );
        warnings[numberOfWarnings].distance = otherShip->distance;
        strcpy( warnings[numberOfWarnings].name, otherShip->name );
        ASSERT( strlen( warnings[numberOfWarnings].name ) <= MAX_NAME );
        numberOfWarnings++;
        }
    }
}


/*
 * Print out the nicely formatted warning.
 *
 */
static void outputWarning( const Warning *warning )
{
fprintf( stdout, "** Collision warning --> %s %.2f\n",
    warning->name, warning->distance );
}


/*
 * Get single line from the stdin and
 * strip the enter character.
 * Return zero on end of file.
 *
 */
static int getLine( char *buffer, unsigned int bufferSize )
{
char *result;

result = fgets( buffer, bufferSize, stdin );
if ( result == NULL ) {    /*error or end of file*/
    return 0;
    }
while ( *result ) {        /*go to the end*/
    result++;
    }
result--;
ASSERT( *result == '\n' );
*result = '\0';            /*strip the trailing enter character*/
return 1;
}


/*
 * Read the ship record from input set.
 *
 */
static void readShip( Ship *ship )
{
char buffer[MAX_LONGLINE];
int result;

result = getLine( ship->name, MAX_NAME + 2 );
ASSERT( result );
ASSERT( strlen( ship->name ) <= MAX_NAME );
result = getLine( buffer, MAX_LONGLINE );
ASSERT( result );
result = sscanf( buffer, "%lf %lf %lf %lf", &(ship->x),
                    &(ship->y), &(ship->course), &(ship->speed));
ASSERT( result == 4 );
ASSERT( ship->course >= 0.0 );
ASSERT( ship->course < 360.0 );
ship->course = RADIANS( ship->course );
}




int main( int argc, const char **argv )
{
for ( ; ; ) {
    int result;
    char scenarioName[MAX_SETNAME+14];  /*name of the scenarion*/
    char buffer[MAX_LINE];
    unsigned int otherShips;

    result = getLine( scenarioName, MAX_SETNAME + 14 );
    if ( result == 0 ) {
        break;
        }
    ASSERT( strlen( scenarioName ) <= MAX_SETNAME );
    result = getLine( buffer, MAX_LINE );
    ASSERT( result );
    result = sscanf( buffer, "%u", &otherShips );
    ASSERT( result == 1 );
    ASSERT( otherShips <= MAX_SHIPS );
    readShip( &mineShip );
    moveShip.x = mineShip.x;
    moveShip.y = mineShip.y;
    moveShip.course = mineShip.course;
    moveShip.speed = mineShip.speed;
    translateShip( &moveShip );
    fprintf( stdout, "Scenario: %s\n", scenarioName );
    fprintf( stdout, "\n"
    "Boat ID                   Bearing Distance Lights (left to right)\n"
                     "-----" "-----" "-----" "-----"
                     "-----" "-----" "-----" "-----"
                     "-----" "-----" "-----" "-----"
                     "-----\n" );
    numberOfWarnings = 0;
    while ( otherShips ) {
        Ship otherShip;

        readShip( &otherShip );
        evaluateShip( &otherShip );
        otherShips--;
        }
    {
    unsigned int warnId;

    for ( warnId = 0; warnId < numberOfWarnings; warnId++ ) {
        outputWarning( warnings + warnId );
        }
    }   /*eob*/
    fprintf( stdout, "*****" "*****" "*****" "*****"
                     "*****" "*****" "*****" "*****"
                     "*****" "*****" "*****" "*****"
                     "*****\n" );
    }
return 0;
}


