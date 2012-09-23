/*

Specialized solution to collating sequence problem.
Includes no tests for valid input.

*/

#if 0
#define DBG
#endif

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/* OK_FIXED: added */
#include <string.h>


#define MIN_CHAR	'A'
#define MAX_CHAR	'Z'
#define CHAR_SPAN	( MAX_CHAR - MIN_CHAR + 1 )

#define END_CHAR	'#'

#define STR_LEN		20

long bits[ CHAR_SPAN ] ;

char output[ 1 + CHAR_SPAN + 1 ] ;

#define bigger_set(a,b)		( bits[ (a) - MIN_CHAR ] |= ( 1 << ( (b) - MIN_CHAR ) ) )
#define bigger_tst(a,b)		( bits[ (a) - MIN_CHAR ] &  ( 1 << ( (b) - MIN_CHAR ) ) )

static int readinput( void )
{
	unsigned char buf[ STR_LEN + 1 ] ;
	unsigned char * ptr ;
	buf[0] = '\0' ;
	for( ; ; ) {
		int c ;
		do {
			c = fgetc( stdin ) ;
			switch( c ) {
			    case EOF: {
			        return 0 ;
			    }
			    case END_CHAR: {
			        return 1 ;
			    }
			}
		} while( c < MIN_CHAR || c > MAX_CHAR ) ;
		ptr = buf ;
		do {
			if( ! *ptr ) {
				break ;
			}
			if( *ptr != c ) {
				bigger_set( c, *ptr ) ;
				break ;
			}
			ptr++ ;
			c = fgetc( stdin ) ;
		} while( c >= MIN_CHAR && c <= MAX_CHAR ) ;
		while( c >= MIN_CHAR && c <= MAX_CHAR ) {
			*ptr++ = c ;
			bigger_set( c, c ) ;
			c = fgetc( stdin ) ;
		}
		*ptr = '\0' ;
	}
}

#ifdef DBG
static void testprint( void )
{
	int i ;
	for( i = MIN_CHAR ; i <= MAX_CHAR ; i++ ) {
		int j ;
		printf( "%c: ", i ) ;
		for( j = MIN_CHAR ; j <= MAX_CHAR ; j++ ) {
			fputc( bigger_tst(i,j) ? '1' : '0', stdout ) ;
		}
		fputc( '\n', stdout ) ;
	}
}
#endif

static void sum( int k, long b )
{
	int i ;
	for( i = 0 ; b ; i++, b >>= 1 ) {
		if( b & 1 ) {
			sum( i, bits[ i ] & ~bits[ k ] ) ;
			bits[ k ] |= bits[ i ] ;
		}
	}
}

static void solve( void )
{
	int i ;
	for( i = 0 ; i < CHAR_SPAN ; i++ ) {
		long b ;
		int n ;
		sum( i, bits[ i ] ) ;
		for( b = bits[ i ], n = 0 ; b ; b >>= 1 ) {
			if( b & 1 ) {
				n++ ;
			}
		}
		output[ n ] = i + MIN_CHAR ;
	} ;
}

static void printoutput( void )
{
	int i = 1 ;
	while( output[ i ] ) {
		fputc( output[ i ], stdout ) ;
		i++ ;
	}
	fputc( '\n', stdout ) ;
}

int main( void )
{
	while( readinput() ) {
        #if DBG
    	testprint() ;
        #endif
	    solve() ;
        #if DBG
    	testprint() ;
        #endif
	    printoutput() ;
	    memset( bits, 0, sizeof(bits) ) ;
	    memset( output, 0, sizeof(output) ) ;
	}
	return EXIT_SUCCESS ;
}
