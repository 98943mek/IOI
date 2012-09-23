#include <stdlib.h>
#include <stdio.h>

#define MAX_DOTS	9

#define RANDOM_BOARDS	5000

#define RAND_SEED	5616969

static int randint( int a, int b )
{
	return a + (int)( (b - a + 1.0) * rand() / (RAND_MAX + 1.0) ) ;
}

int main( void )
{
	int k, d, i, j ;
	char buf[ 2 * MAX_DOTS * MAX_DOTS * ( 2 + 3 + 1 + 3 + 1 ) ] ;
	srand( RAND_SEED ) ;
	for( d = MAX_DOTS ; d >= 2 ; d-- ) {
		printf( "%d\n%d\n", d, 2 * ( d - 1 ) * d ) ;
		for( i = 1 ; i <= d ; i++ ) {
			for( j = 1 ; j <= d - 1 ; j++ ) {
				printf( "V %d %d\nH %d %d\n", i, j, i, j ) ;
			}
		}
	}
	for( k = RANDOM_BOARDS ; k-- ; ) {
		int count = 0 ;
		int p = rand() ;
		char * ptr = buf ;
		*ptr = '\0' ;
		d = randint( 2, MAX_DOTS ) ;
		for( i = 1 ; i <= d ; i++ ) {
			for( j = 1 ; j <= d - 1 ; j++ ) {
				if( rand() > p ) {
					ptr += sprintf( ptr, "V %d %d\n", i, j ) ;
					count++ ;
				}
				if( rand() > p ) {
					ptr += sprintf( ptr, "H %d %d\n", i, j ) ;
					count++ ;
				}
			}
		}
		printf( "%d\n%d\n", d, count ) ;
		fputs( buf, stdout ) ;
	}
	return EXIT_SUCCESS ;
}
