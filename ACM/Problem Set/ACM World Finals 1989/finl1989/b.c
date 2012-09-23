#if 0
#define DBG
#endif

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/* OK_FIXED: added */
#include <string.h>

#define MAX_DOTS 9

typedef char bool ;

bool hlines[ MAX_DOTS ][ MAX_DOTS - 1 ] ;	/* rows x columns */
bool vlines[ MAX_DOTS - 1 ][ MAX_DOTS ] ;

int hspan[ MAX_DOTS ][ MAX_DOTS - 1 ] ;
int vspan[ MAX_DOTS - 1 ][ MAX_DOTS ] ;

int size ;

int squares[ MAX_DOTS ] ;

#ifndef min
#define min(a,b)	(((a)<(b))?(a):(b))
#endif

#ifdef DBG
static void testprint( void )
{
	int i, j ;
	for( i = 0 ; i < size - 1 ; i++ ) {
		for( j = 0 ; j < size - 1 ; j++ ) {
			fputs( hlines[ i ][ j ] ? "---" : "   ", stdout ) ;
		}
		fputc( '\n', stdout ) ;
		for( j = 0 ; j < MAX_DOTS ; j++ ) {
			fputs( vlines[ i ][ j ] ? "|  " : "   ", stdout ) ;
		}
		fputc( '\n', stdout ) ;
	}
	for( j = 0 ; j < size - 1 ; j++ ) {
		fputs( hlines[ i ][ j ] ? "---" : "   ", stdout ) ;
	}
	fputs( "\n\n", stdout ) ;
}

static void testprint2( void )
{
	int i, j ;
	for( i = 0 ; i < size ; i++ ) {
		for( j = 0 ; j < size - 1 ; j++ ) {
			printf( "%d ", hspan[ i ][ j ] ) ;
		}
		fputc( '\n', stdout ) ;
	}
	for( i = 0 ; i < size - 1 ; i++ ) {
		for( j = 0 ; j < size ; j++ ) {
			printf( "%d ", vspan[ i ][ j ] ) ;
		}
		fputc( '\n', stdout ) ;
	}
}
#endif

static void solve( void )
{
	int i, j, k ;
	for( i = size ; i-- ; ) {
		int hl, vl ;
		hl = vl = 0 ;
		for( j = size - 1 ; j--; ) {
			if( hlines[ i ][ j ] ) {
				hl++ ;
			}
			else {
				hl = 0 ;
			}
			hspan[ i ][ j ] = hl ;
			if( vlines[ j ][ i ] ) {
				vl++ ;
			}
			else {
				vl = 0 ;
			}
			vspan[ j ][ i ] = vl ;
		}
	}
	for( i = 0 ; i < size - 1 ; i++ ) {
		for( j = 0 ; j < size - 1 ; j++ ) {
			for( k = min( vspan[ i ][ j ], hspan[ i ][ j ] ) ; k ; k-- ) {
				if( vspan[ i ][ j + k ] >= k && hspan[ i + k ][ j ] >= k ) {
					squares[ k - 1 ]++ ;
                    #ifdef DBG
					printf( "(%d,%d,%d)\n", i, j, k ) ;
                    #endif
				}
			}
		}
	}
}

static void printoutput( int n )
{
	int i, f ;
	f = 0 ;
	if( n > 1 ) {
		fputc( '\n', stdout ) ;
	}
	printf( "Problem #%d\n\n", n ) ;
	for( i = 0 ; i < size ; i++ ) {
		if( squares[ i ] ) {
			printf( "%d square (s) of size %d\n", squares[ i ], i + 1 ) ;
			squares[ i ] = 0 ;
			f = 1 ;
		}
	}
	if( ! f ) {
		printf( "No completed squares can be found.\n" ) ;
	}
	printf( "\n**********************************\n" /* OK_FIXED: , n */ ) ;
}

int main( void )
{
	int problem ;
	for( problem = 1 ; ; problem++ ) {
		int edges ;
		if( fscanf( stdin, "%d\n", &size ) == EOF ) {
			break ;
		}
		assert( size >= 2 ) ;
		assert( size <= MAX_DOTS ) ;
		memset( hlines, 0, sizeof( hlines ) ) ;
		memset( vlines, 0, sizeof( vlines ) ) ;
		fscanf( stdin, "%d\n", &edges ) ;
		while( edges-- ) {
			char c ;
			int i, j ;
			fscanf( stdin, " %c %d %d\n", &c, &i, &j ) ;
			i-- ;
			j-- ;
			assert( i >= 0 ) ;
			assert( j >= 0 ) ;
			if( c == 'H' ) {
				assert( i < size  ) ;
				assert( j < size - 1 ) ;
				assert( ! hlines[ i ][ j ] ) ;
				hlines[ i ][ j ] = 1 ;
			}
			else {
				assert( c == 'V' ) ;
				assert( i < size ) ;
				assert( j < size - 1 ) ;
				assert( ! vlines[ j ][ i ] ) ;
				vlines[ j ][ i ] = 1 ;
			}
		}
        #ifdef DBG
		testprint() ;
        #endif
		solve() ;
        #ifdef DBG
		testprint2() ;
        #endif
		printoutput( problem ) ;
	}
	return EXIT_SUCCESS ;
}
