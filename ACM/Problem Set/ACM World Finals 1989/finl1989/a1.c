/*

Standard solution to collating sequence problem.
Includes tests for valid input.

*/

#if 0
#define DBG
#endif

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define err(s)	do{fprintf(stderr,"Error: %s!\n",s);exit(EXIT_FAILURE);}while(0)
#define err1(s,n)	do{fprintf(stderr,"Error: " s "!\n",(n));exit(EXIT_FAILURE);}while(0)
#define err2(s,n1,n2)	do{fprintf(stderr,"Error: " s "!\n",(n1),(n2));exit(EXIT_FAILURE);}while(0)

#define MIN_CHAR	'A'
#define MAX_CHAR	'Z'
#define END_CHAR	'#'

#define STR_LEN		20

typedef char bool_t ;
#define TRUE 	(!0)
#define FALSE	(!1)

bool_t bigger[ 256 ] [ 256 ] ;

typedef char state_t ;
#define STATE_NOTSEEN	0
#define STATE_SEEN 	STATE_SUM
#define STATE_SUM	1
#define STATE_SUMMING	2
#define STATE_DONE	3

state_t state[ 256 ] ;

int order[ 256 ] ;

static int readinput( void )
{
	unsigned char buf[ STR_LEN + 1 ] ;
	unsigned char * ptr ;
	buf[0] = '\0' ;
	for( ; ; ) {
		int c ;
		do {
			c = fgetc( stdin ) ;
			if( c == EOF ) {
			    return 0 ;
			}
			if( c == END_CHAR ) {
				return 1 ;
			}
		} while( c < MIN_CHAR || c > MAX_CHAR ) ;
		ptr = buf ;
		do {
			if( ! *ptr ) {
				break ;
			}
			if( *ptr != c ) {
				bigger[ c ][ *ptr ] = TRUE ;
				break ;
			}
			ptr++ ;
			c = fgetc( stdin ) ;
			if( c == EOF ) {
				err( "unexpected EOF" ) ;
			}
		} while( c >= MIN_CHAR && c <= MAX_CHAR ) ;
		while( c >= MIN_CHAR && c <= MAX_CHAR ) {
			*ptr++ = c ;
			state[ c ] = STATE_SEEN ;
			c = fgetc( stdin ) ;
			if( c == EOF ) {
				err( "unexpected EOF" ) ;
			}
		}
		*ptr = '\0' ;
	}
}


#ifdef DBG
static void testprint( void )
{
	int i ;
	for( i = 0 ; i < 256 ; i++ ) {
		int j ;
		for( j = 0 ; j < 256 ; j++ ) {
			if( bigger[ i ][ j ] ) {
				printf( "%c > %c\n", i, j ) ;
			}
		}
	}
	for( i = MIN_CHAR ; i <= MAX_CHAR ; i++ ) {
		printf( "%c: %u\n", i, order[ i ] ) ;
	}
}
#endif

static void sum( int k )
{
	int i ;
	switch( state[ k ] ) {
		case STATE_NOTSEEN: {
			err1( "char %c not seen", k ) ;
			break ;
		}
		case STATE_SUMMING: {
			err1( "char %c in cycle", k ) ;
			break ;
		}
		case STATE_DONE: {
			return ;
		}
	}
	assert( state[ k ] == STATE_SUM ) ;
	assert( order[ k ] == 0 ) ;
	state[ k ] = STATE_SUMMING ;
	order[ k ] = 1 ;
	for( i = MIN_CHAR ; i <= MAX_CHAR ; i++ ) {
		if( bigger[ k ][ i ] ) {
			sum( i ) ;
			order[ k ] += order[ i ] ;
		}
	}
	state[ k ] = STATE_DONE ;
}

static void solve( void )
{
	int i ;
	for( i = MIN_CHAR ; i <= MAX_CHAR ; i++ ) {
		if( state[ i ] == STATE_SEEN ) {
			sum( i ) ;
		}
	}

}

static void printoutput( void )
{
	for( ; ; ) {
		int m = 1 << ( MAX_CHAR - MIN_CHAR + 1 ), i ;
		char c = '\0' ;
		for( i = MIN_CHAR ; i <= MAX_CHAR ; i++ ) {
			if( order[ i ] ) {
				if( order[ i ] < m ) {
					m = order[ i ] ;
					c = i ;
				}
				else if( order[ i ] == m ) {
					err2( "unresolved collision between %c and %c", c, i ) ;
				}
			}
		}
		if( ! c ) {
			break ;
		}
		fputc( c, stdout ) ;
		order[ c ] = 0 ;
	}
	fputc( '\n', stdout ) ;
}

int main( void )
{
	while( readinput() ) {
#ifdef DBG
    	testprint() ;
#endif
    	solve() ;
#ifdef DBG
    	testprint() ;
#endif
    	printoutput() ;
	    memset( bigger, 0, sizeof(bigger) ) ;
	    memset( state, 0, sizeof(state) ) ;
	    memset( order, 0, sizeof(order) ) ;
    }
	return EXIT_SUCCESS ;
}
