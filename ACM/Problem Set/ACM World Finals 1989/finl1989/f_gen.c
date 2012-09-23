#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/* OK_FIXED: added */
#include <string.h>


#define hope assert

#define MAX_SCHEDULES   200
#define MAX_REQUESTS    8
#define MAX_FLIGHTS     20      /* per schedule */
#define MAX_CITIES      (2*MAX_FLIGHTS)

#define MAX_COST        (100000-1)  /* in cents */
/* this means travel can't take more than 10 days
   and it can't cost more than 10000.00,
   while each flight costs less than 1000.00
   and takes no more 24 hours
*/
#define MAX_SCHED       10

/*
   This is the special ambiguous travel
   avoidance limit.
   ( (1<<16)/100 < 1000 && (1<<10)/60 < 24 )
*/
#define MAX_TIME_LIMIT  11
#define MAX_COST_LIMIT  17
   
#define CITYNAMELEN     19
#define CITYSTR         "%-19s" /* cityname len */
#define CITYNUM         "%-19d" /* cityname len */

#define MINS_PER_DAY    (60*24)

#define RAND_SEED	5615939

typedef char cityname_t[ CITYNAMELEN + 1 ] ;

cityname_t cityname[ MAX_CITIES ] ;

char accessible[ MAX_CITIES ][ MAX_CITIES ] ;

static int randint( int a, int b )
{
	return a + (int)( (b - a + 1.0) * rand() / (RAND_MAX + 1.0) ) ;
}

static void initnames( void )
{
    int n ;
    for( n = 0 ; n < MAX_CITIES ; n++ ) {
        char c ;
        int i ;
        char * s = (char *) ( cityname + n ) ;
        c = n >= 26 ? 'A' + n - 26 : 'a' + n ;
        *s++ = c ;
        hope( n < 2 * 26 ) ;
        for( i = CITYNAMELEN - 1 ; i-- ; ) {
            if( rand() < RAND_MAX / ( CITYNAMELEN / 2 ) ) {
                c = ' ' ;
            }
            else {
                c = randint( 'a', 'z' ) ;
                if( rand() > RAND_MAX / 2 ) {
                    c += 'A' - 'a' ;
                }
            }
            *s++ = c ;
        }
        *s = '\0' ;
    }
}

static void makeaccessible( void )
{
    int n, i, j, k ;
    for( n = 0 ; n < MAX_SCHED ; n++ ) {
        for( i = 0 ; i < MAX_SCHED ; i++ ) {
            for( j = 0 ; j < MAX_SCHED ; j++ ) {
                for( k = 0 ; k < MAX_SCHED ; k++ ) {
                    if( accessible[ i ][ j ] ) {
                        break ;
                    }
                    accessible[ i ][ j ] = accessible[ i ][ k ] && accessible[ k ][ j ] ;
                }
            }
        }
    }
}

static void printtime( int t )
{
    int h = t / 60 ;
    printf( "%02d:%02d%c", h % 12, t % 60 , h >= 12 ? 'P' : 'A' ) ;
}

static void printcost( int c )
{
    printf( "%3d.%02d", c / 100, c % 100 ) ;
}

static void printflights( void )
{
    int n, c, i, m, l ;
    memset( accessible, 0, sizeof( accessible ) ) ;
    m = rand() > RAND_MAX / 2 ;
    l = m ? MAX_TIME_LIMIT : MAX_COST_LIMIT ;
    n = randint( 1, l ) ;
    c = randint( 2, MAX_SCHED ) ;
    for( i = n ; i-- ; ) {
        int f, t ;
        int dep, arr, cost ;
        int a ;
        f = randint( 0, c - 1 ) ;
        do {
            t = randint( 0, c - 1 ) ;
        } while( t == f ) ;
        a = i ;             /* so there is no ambiguity */
        if( m ) {
            dep = 1 ;
            arr = dep + ( 1 << a ) ;
            switch( randint( 0, 5 ) ) {
                case 0: {
                    cost = 1 ;
                    break ;
                }
                case 1: {
                    cost = MAX_COST ;
                    break ;
                }
                default: {
                    cost = randint( 1, MAX_COST ) ;
                    break ;
                }
            }
        }
        else {
            cost = 1 << a ;
            switch( randint( 0, 5 ) ) {
                case 0: {
                    dep = randint( 0, 30 ) ;
                    break ;
                }
                case 1: {
                    dep = randint( MINS_PER_DAY / 2, MINS_PER_DAY / 2 + 30 ) ;
                    break ;
                }
                default: {
                    dep = randint( 0, MINS_PER_DAY - 1 ) ;
                    break ;
                }
            }   
            switch( randint( 0, 5 ) ) {
                case 0: {
                    arr = dep + randint( 1, 30 ) ;
                    break ;
                }
                case 1: {
                    arr = dep + randint( 3*60, 5*60 ) ;
                    break ;
                }
                default: {
                    arr = randint( 0, MINS_PER_DAY - 1 ) ;
                    break ;
                }
            }   
        }
        arr %= MINS_PER_DAY ;
        hope( dep < MINS_PER_DAY ) ;
        hope( arr < MINS_PER_DAY ) ;
        hope( cost ) ;
        hope( cost <= MAX_COST ) ;
        hope( f != t ) ;
        printf( CITYSTR " " CITYSTR " ", cityname[ f ], cityname[ t ] ) ;
        printtime( dep ) ;
        fputc( ' ', stdout ) ;
        printtime( arr ) ;
        fputc( ' ', stdout ) ;
        printcost( cost ) ;
        fputc( '\n', stdout ) ;
        accessible[ f ][ t ] = 1 ;
    }
    fputs( "#\n", stdout ) ;        
}

static void printrequest( int f, int t )
{
    printf( CITYSTR " " CITYSTR " ", cityname[ f ], cityname[ t ] ) ;
    fputs( rand() > RAND_MAX / 2 ? "TIME\n" : "COST\n", stdout ) ;
}

static void printrequests( void )
{
    int n = MAX_REQUESTS ;
    int f, t ;
    makeaccessible() ;
    for( n = MAX_REQUESTS ; n-- ; ) {
        for( f = 0 ; f < MAX_SCHED ; f++ ) {
            for( t = 0 ; t < MAX_SCHED ; t++ ) {
                if( f == t || ! accessible[ f ][ t ] ) {
                    continue ;
                }
                printrequest( f, t ) ;
            }
        }
    }
    for( ; ; ) {
        f = randint( 0, MAX_SCHED - 1 ) ;
        t = randint( 0, MAX_SCHED - 1 ) ;
        if( f == t || ! accessible[ f ][ t ] ) {
            continue ;
        }
        printrequest( f, t ) ;
        if( rand() <= RAND_MAX / 100 ) {
            break ;
        }
    }
    fputs( "#\n", stdout ) ;
}

int main( void )
{
    int i, n = MAX_SCHEDULES ;
    
	srand( RAND_SEED ) ;
	printf( "%d\n", n ) ;
	
	printf(
        "Center City         Homeville            5:20A  6:55A  12.50\n"
        "Center City         Greenville           5:45A  9:15A  35.00\n"
        "Homeville           Greenville           7:45A  9:35A  20.00\n"
        "Archer City         Homeville            5:00A  6:00P 612.50\n"
        "#\n"
        "Center City         Greenville          COST\n"
        "Archer City         Greenville          TIME\n"
        "#\n"
        "A                   B                   00:00A 00:02A  10.00\n"
        "A                   B                   00:10A 00:11A  10.00\n"
        "B                   C                   00:05A 00:07A  10.00\n"
        "B                   C                   00:18A 00:19A  10.00\n"
        "#\n"
        "A                   B                   TIME\n"
        "B                   C                   TIME\n"
        "A                   C                   TIME\n"
        "A                   B                   COST\n"
        "B                   C                   COST\n"
        "A                   C                   COST\n"
        "#\n"
    ) ;
    n -= 2 ;

    initnames() ;    
    for( i = 0 ; i < MAX_FLIGHTS ; i++ ) {
        printf( CITYSTR " " CITYSTR " 00:00A 12:00M   1.00\n", cityname[i], cityname[i + MAX_FLIGHTS] ) ;
    }
    printf( "#\n" ) ;
    for( i = 0 ; i < MAX_FLIGHTS ; i++ ) {
        printf( CITYSTR " " CITYSTR " TIME\n", cityname[i], cityname[i + MAX_FLIGHTS] ) ;
        printf( CITYSTR " " CITYSTR " COST\n", cityname[i], cityname[i + MAX_FLIGHTS] ) ;
    }
    printf( "#\n" ) ;
    n-- ;

    initnames() ;    
    for( i = 0 ; i < MAX_FLIGHTS ; i++ ) {
        printf( CITYSTR " " CITYSTR " 00:00P 12:00N   1.00\n", cityname[i], cityname[i + MAX_FLIGHTS] ) ;
    }
    printf( "#\n" ) ;
    for( i = 0 ; i < MAX_FLIGHTS ; i++ ) {
        printf( CITYSTR " " CITYSTR " TIME\n", cityname[i], cityname[i + MAX_FLIGHTS] ) ;
        printf( CITYSTR " " CITYSTR " COST\n", cityname[i], cityname[i + MAX_FLIGHTS] ) ;
    }
    printf( "#\n" ) ;
    n-- ;
    
    while( n-- ) {
        initnames() ;
        printflights() ;
        printrequests() ;
    }

	return EXIT_SUCCESS ;
}
