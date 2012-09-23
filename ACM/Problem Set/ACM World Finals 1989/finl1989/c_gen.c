#include <stdlib.h>
#include <stdio.h>

/* OK_FIXED: lowered number of pairs - it worked for too long */
#define MAX_NUM		2500

/* OK_FIXED: lowered number of random pairs - it worked for too long */
#define RANDOM_NUMBERS	2300

#define RAND_SEED	5315969

static int randint( int a, int b )
{
	return a + (int)( (b - a + 1.0) * rand() / (RAND_MAX + 1.0) ) ;
}

int main( void )
{
	int i /* OK_FIXED: unused variable , j */ ;
	srand( RAND_SEED ) ;
	for( i = 1 ; i <= MAX_NUM ; i++ ) {
		printf( "1 %d\n", i ) ;
	}
	for( i = 1 ; i <= MAX_NUM ; i++ ) {
		printf( "%d %d\n", MAX_NUM, i ) ;
	}
	for( i = 1 ; i <= MAX_NUM ; i++ ) {
		printf( "%d %d\n", MAX_NUM - 1, i ) ;
	}
	for( i = 0 ; i <= MAX_NUM ; i++ ) {
		printf( "%d %d\n", i, MAX_NUM ) ;
	}
	for( i = 0 ; i <= MAX_NUM ; i++ ) {
		printf( "%d %d\n", i, MAX_NUM - 1 ) ;
	}
	for( i = 0 ; i <= RANDOM_NUMBERS ; i++ ) {
		printf( "%d %d\n", randint( 0, MAX_NUM ), randint( 1, MAX_NUM ) ) ;
	}
	return EXIT_SUCCESS ;
}
