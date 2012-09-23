#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/* OK_FIXED: added */
#include <string.h>

#define MAX_NUM		3000

int used[ MAX_NUM ] ;

int length ;

#define FRACTION_LEN	50

char fract[ FRACTION_LEN ] ;

int main( void )
{
	int a, b ;
	while( fscanf( stdin, "%d %d\n", &a, &b ) != EOF ) {
		int cyclepos, cyclelen ;
		assert( a >= 0 ) ;
		assert( b > 0 ) ;
		assert( a <= MAX_NUM ) ;
		assert( b <= MAX_NUM ) ;
		memset( used, 0, sizeof( used ) ) ;
		length = 0 ;
		printf( "%d/%d = %d.", a, b, a / b ) ;
		for( ; ; ) {
			a %= b ;
			if( used[ a ] ) {
				break ;
			}
			used[ a ] = length + 1 ;
			a *= 10 ;
			if( length < FRACTION_LEN ) {
				fract[ length ] = '0' + ( a / b ) ;
			}
			length++ ;
		}
		cyclepos = used[ a ] - 1 ;
		cyclelen = length - cyclepos ;
		printf( "%.*s(%.*s", cyclepos, fract, cyclelen, fract + cyclepos ) ;
		if( length > FRACTION_LEN ) {
			printf( "..." ) ;
		}
		printf( ")\n   %d = number of digits in repeating cycle\n", cyclelen ) ;
	}
	return EXIT_SUCCESS ;
}
