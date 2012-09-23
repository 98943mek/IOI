/*
        Problem:        G
        Description:    sort locations
        Class:          database
        Subclass:       sorting
        Algorithm:      heapsort
        Author:         Ondrej Karny
        Date:           1998-Mar-30
*/
/*
 * ACM International Collegiate Programming Contest Finals 1989
 *
 * Problem G - Meals on Wheels Routing System
 *
 * Example solution
 *
 * Note:
 *
 * Fixes in original problem description include clarification of the
 * sort order, limits for all the objects involved and stating that
 * drivers ride through the route in order the customers were assigned,
 * not on shortest possible route. Also redone the output format.
 *
 * Algorithm:
 *
 * for each dataset do
 *  read name of the dataset
 *  read number of drivers
 *  read number of customers
 *  read the customer database - names and locations
 *  allocate customers to routes using following rules (descending priority)
 *   the polar angle of the location (0~ goes +x, 90~ goes +y)
 *   distance from the origin
 *   order in input set
 *  write out results
 *
 * Uses only sorting and some data management tools.
 *
 * Special accuracy note:
 *
 * Because the coordinates are integers and tangent values are
 * used only for sorting, there're no inaccuracies. No requirement
 * for rounding or epsilon sized deltas in results judging.
 *
 */

#include <stdio.h>

#ifdef DEBUG
#include <string.h>
#endif

#ifdef DEBUG
#include <assert.h>
#define ASSERT( condition ) assert( condition )
#else
#define ASSERT( condition )
#endif


#define MAX_CUSTOMERS 4000
#define MAX_ROUTES    4000
#define MAX_LINE      128
#define MAX_NAME      25
#define MAX_SETNAME   50

typedef struct _Customer {
    unsigned int number;   /*number in input set*/
    int x;
    int y;                 /*location*/
    int quadrant;          /*quadrant the locatio is in*/
    double tangent;        /*pseudo tangent value*/
    char name[MAX_NAME+2]; /*25 chars + optional enter + terminating zero*/
    } Customer;

Customer     customers[MAX_CUSTOMERS];     /*database of customers*/
Customer    *customerArray[MAX_CUSTOMERS]; /*used for sorting*/
unsigned int numberOfCustomers;


typedef struct _Route {
    unsigned int length;            /*total length of route*/
    unsigned int numberOfCustomers; /*number of the customers assigned*/
    Customer **customerList;        /*array of customers*/
    } Route;

Route         routes[MAX_ROUTES];    /*database of routes*/
Route       *routeArray[MAX_ROUTES]; /*used for sorting*/
unsigned int numberOfRoutes;



#define int_abs( x ) ( (x) >= 0 ? (x) : -(x) )


/*
 * Compare two Customer records. Returns
 * non-zero if the first one ('left') is
 * less in the aforementioned
 * order than the second one ('right').
 * In other words, operator <.
 *
 */
static int customerIsLess( const Customer *left, const Customer *right )
{
if ( left->quadrant < right->quadrant ) {
    return 1;
    }
if ( left->quadrant > right->quadrant ) {
    return 0;
    }
ASSERT( left->quadrant == right->quadrant );
if ( left->tangent < right->tangent ) {
    return 1;
    }
if ( left->tangent > right->tangent ) {
    return 0;
    }
ASSERT( left->tangent == right->tangent );
{
unsigned int leftLength;
unsigned int rightLength;

leftLength = int_abs( left->x ) + int_abs( left->y );
rightLength = int_abs( right->x ) + int_abs( right->y );
if ( leftLength < rightLength ) {
    return 1;
    }
if ( leftLength > rightLength ) {
    return 0;
    }
}   /*eob*/
ASSERT( left->x == right->x );
ASSERT( left->y == right->y );
if ( left->number < right->number ) {
    return 1;
    }
else {
    ASSERT( left->number > right->number );
    return 0;
    }
}


/*
 * Heapsort the customerArray[].
 *
 */
static void sortCustomers( void )
{
unsigned int index;

for ( index = 1; index < numberOfCustomers; index++ ) {
    unsigned int bubbleIndex;
    Customer *bubbleElement;

    bubbleIndex = index;
    bubbleElement = customerArray[bubbleIndex];
    while ( bubbleIndex > 0 ) {
        unsigned int parentIndex;

        parentIndex = ( bubbleIndex - 1 ) >> 1;
        if ( !customerIsLess( bubbleElement,
                    customerArray[parentIndex] )) {
            customerArray[bubbleIndex] = customerArray[parentIndex];
            bubbleIndex = parentIndex;
            }
        else {
            break;
            }
        }
    customerArray[bubbleIndex] = bubbleElement;
    }
#ifdef DEBUG
for ( index = 1; index < numberOfCustomers; index++ ) {
    ASSERT( customerIsLess( customerArray[index],
                 customerArray[(index-1)>>1] ));
    }
#endif /*def DEBUG*/
for ( index = numberOfCustomers - 1; index; ) {
    Customer *bubbleElement;
    unsigned int bubbleIndex;
    unsigned int sonIndex;

    bubbleElement = customerArray[index];
    customerArray[index] = customerArray[0];
    bubbleIndex = 0;
    index--;
    while ( sonIndex = ( 2 * bubbleIndex ) + 1, sonIndex <= index ) {
        if ( sonIndex < index ) {
            if ( customerIsLess( customerArray[sonIndex],
                customerArray[sonIndex+1] )) {
                sonIndex++;
                }
            }
        if ( !customerIsLess( customerArray[sonIndex], bubbleElement )) {
            customerArray[bubbleIndex] = customerArray[sonIndex];
            bubbleIndex = sonIndex;
            }
        else {
            break;
            }
        }
    customerArray[bubbleIndex] = bubbleElement;
    }
#ifdef DEBUG
for ( index = 0; index < numberOfCustomers - 1; index++ ) {
    ASSERT( customerIsLess( customerArray[index],
                customerArray[index+1] ));
    }
#endif /*def DEBUG*/
}


/*
 * Assign the customers (already sorted)
 * to the routes. Also measures the route
 * lengths.
 *
 */
static void buildRoutes( void )
{
Customer **customerList;
unsigned int restCustomers;
unsigned int routeId;

customerList = customerArray;
restCustomers = numberOfCustomers;
for ( routeId = 0; routeId < numberOfRoutes; routeId++ ) {
    unsigned int assignedCustomers;

    assignedCustomers = ( restCustomers + ( numberOfRoutes - routeId - 1 )) /
                ( numberOfRoutes - routeId );
    restCustomers -= assignedCustomers;
    routes[routeId].numberOfCustomers = assignedCustomers;
    routes[routeId].customerList = customerList;
    {
    int x;
    int y;

    x = (*customerList)->x;
    y = (*customerList)->y;
    customerList++;
    routes[routeId].length = int_abs( x ) + int_abs( y );
    while ( --assignedCustomers ) {
        int newX;
        int newY;

        newX = (*customerList)->x;
        newY = (*customerList)->y;
        customerList++;
        routes[routeId].length += int_abs( newX - x ) +
                      int_abs( newY - y );
        x = newX;
        y = newY;
        }
    routes[routeId].length += int_abs( x ) + int_abs( y );
    }    /*eob*/
    routeArray[routeId] = routes + routeId;
    }
ASSERT( restCustomers == 0 );
ASSERT( customerList == customerArray + numberOfCustomers );
}


/*
 * Write out the route data, nicely formatted.
 *
 */
static void outputRoutes( void )
{
unsigned int routeId;
unsigned int totalLength;
#ifdef DEBUG
unsigned int routeCustomers;
unsigned int totalCustomers;
#endif /*def DEBUG*/

totalLength = 0;
#ifdef DEBUG
routeCustomers = routeArray[0]->numberOfCustomers;
totalCustomers = 0;
#endif /*def DEBUG*/
for ( routeId = 0; routeId < numberOfRoutes; routeId++ ) {
    const Route *route;

    route = routeArray[routeId];
    fprintf( stdout, "Route ==> %u\n", routeId + 1 );
    {
    Customer **customerList;
    unsigned int customerId;

    customerList = route->customerList;
    ASSERT(( route->numberOfCustomers == routeCustomers ) ||
           ( route->numberOfCustomers == routeCustomers - 1 ));
    for ( customerId = route->numberOfCustomers; customerId; customerId-- ) {
        fprintf( stdout, "Customer: %s\n", (*customerList)->name );
        customerList++;
#ifdef DEBUG
        totalCustomers++;
#endif /*def DEBUG*/
        }
    }    /*eob*/
    fprintf( stdout, "Route Length ==> %u\n\n", route->length );
    totalLength += route->length;
    }
ASSERT( totalCustomers == numberOfCustomers );
fprintf( stdout, "Total Route Length ==> %u\n", totalLength );
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
 * Read the customer's dataset.
 * Read name and location, compute
 * quadrant the customer lives in
 * and also tangent value of the polar
 * angle.
 *
 */
static void readCustomers( void )
{
unsigned int customerId;
char buffer[MAX_LINE];

for ( customerId = 0; customerId < numberOfCustomers; customerId++ ) {
    int x;
    int y;
    int result;

    customers[customerId].number = customerId;
    result = getLine( customers[customerId].name, MAX_NAME + 2 );
    ASSERT( result );
    ASSERT( strlen( customers[customerId].name ) <= MAX_NAME );
    result = getLine( buffer, MAX_LINE );
    ASSERT( result );
    result = sscanf( buffer, "%d %d", &x, &y );
    ASSERT( result == 2 );
    customers[customerId].x = x;
    customers[customerId].y = y;
    {
    int quadrant;
    double tangent;

    if (( x == 0 ) &&  ( y == 0 )) {
        quadrant = 0;
        tangent = 0.0;
        }
    else if (( x > 0 ) && ( y >= 0 )) {
        quadrant = 0;
        tangent = (double)y / x;
        }
    else if (( x <= 0 ) && ( y > 0 )) {
        quadrant = 1;
        tangent = (double)-x / y;
        }
    else if (( x < 0 ) && ( y <= 0 )) {
        quadrant = 2;
        tangent = (double)y / x;
        }
    else {
        ASSERT(( x >= 0 ) && ( y < 0 ));
        quadrant = 3;
        tangent = (double)x / -y;
        }
    customers[customerId].quadrant = quadrant;
    customers[customerId].tangent = tangent;
    }    /*eob*/
    customerArray[customerId] = customers + customerId;
    }
}


int main( int argc, const char **argv )
{
while ( 1 ) {
    int result;
    char setName[MAX_SETNAME+14];        /*name of the data set*/
    char buffer[MAX_LINE];

    result = getLine( setName, MAX_SETNAME + 14 );
    if ( result == 0 ) {
        break;
        }
    ASSERT( strlen( setName ) <= MAX_SETNAME );
    result = getLine( buffer, MAX_LINE );
    ASSERT( result );
    result = sscanf( buffer, "%u %u", &numberOfRoutes,
                      &numberOfCustomers );
    ASSERT( result == 2 );
    ASSERT( numberOfCustomers );
    ASSERT( numberOfCustomers <= MAX_CUSTOMERS );
    ASSERT( numberOfRoutes );
    ASSERT( numberOfRoutes <= numberOfCustomers );
    readCustomers();
    sortCustomers();
    buildRoutes();
    fprintf( stdout, "%s\n", setName );
    fprintf( stdout, "Number of Customers: %u Number of Routes: %u\n\n",
            numberOfCustomers, numberOfRoutes );
    outputRoutes();
    fprintf( stdout, "*****" "*****" "*****" "*****"
                     "*****" "*****" "*****" "*****\n" );
    }
return 0;
}

