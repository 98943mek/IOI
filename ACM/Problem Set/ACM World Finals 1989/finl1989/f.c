#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <assert.h>
#include <string.h> 

#if 0
#define DBG
#endif

#ifndef min
#define min(a,b)    (((a)<(b))?(a):(b))
#endif

#define hope(c) assert(c)

#define round(n)    floor((n)+.5)

#define CITYNAME_LEN    19

#define MAX_FLIGHTS     20      /* per schedule list */
#define MAX_CITIES      (2*MAX_FLIGHTS)

/* how many flights it can take to get somewhere */
#define MAX_FLIGHTLEN   min(MAX_FLIGHTS,MAX_CITIES)

#define MINS_PER_DAY    (24*60)

typedef struct city city_t ;

typedef struct flight {
    struct flight * next ;      /* list of from-city flights */
    city_t * from, * to ;
    int departure, arrival ;
    int cost ;
} flight_t ;

struct city {
    char name[CITYNAME_LEN+1] ;
    flight_t * flight ;
    int used ;
} ;

flight_t flight[MAX_FLIGHTS] ;
int flights ;

city_t city[MAX_CITIES] ;
int cities ;

typedef struct cache {
    int time ;
    int cost ;
    int flightidx, flightlen ;
} cache_t ;

cache_t timecache[ MAX_CITIES ][ MAX_CITIES ] ;
cache_t costcache[ MAX_CITIES ][ MAX_CITIES ] ;

flight_t * flightstack[ MAX_FLIGHTLEN ] ;

flight_t * flightpool[ 2 * MAX_CITIES * MAX_CITIES * MAX_FLIGHTLEN ] ;
int flightlast ;

static city_t * getcity( const char * name )
{
    city_t * p = city ;
    int i ;
    for( i = cities ; i-- ; p++ ) {
        if( strcmp( p->name, name ) == 0 ) {
            return p ;    
        }
    }
    hope( cities < MAX_CITIES ) ;
    hope( p == city + cities ) ;
    cities++ ;
    strcpy( p->name, name ) ;
    p->flight = NULL ;
    p->used = 0 ;
    return p ;
}

static void addflight( city_t * f, city_t * t, int d, int a, int c )
{
    flight_t * n ;
    hope( f ) ;
    hope( t ) ;
    hope( d < a ) ;
    hope( c > 0 ) ;
    hope( c < 100000 ) ;
    hope( flights < MAX_FLIGHTS ) ;
    n = flight + flights++ ;
    n->from = f ;
    n->to = t ;
    n->departure = d ;
    n->arrival = a ;
    n->cost = c ;
    n->next = f->flight ;
    f->flight = n ;
}

static city_t * readcity( FILE * fi )
{
    static char buf[CITYNAME_LEN+2] ;
    int c ;
    hope( fi ) ;
    c = fgetc( fi ) ;
    if( c == '#' ) {
        do {
            c = fgetc( fi ) ;
        } while( c != '\n' && c != EOF ) ;
        return NULL ;
    }
    else {
        char * p ;
        buf[0] = c ;
        fread( buf + 1, 1, sizeof(buf) - 2, fi ) ;
        p = buf + CITYNAME_LEN ;
        while( *p == ' ' ) {
            *p-- = '\0' ;
        }
        hope( p >= buf ) ;
        return getcity( buf ) ;
    }
}

static int readtime( FILE * fi )
{
    int h, m ;
    char c ;
    int t ;
    hope( fi ) ;
    t = fscanf( fi, "%d:%d%c ", &h, &m, &c ) ;
    hope( t == 3 ) ;
    hope( h < 12 || ( h == 12 && m == 0 ) ) ;
    hope( m < 60 ) ;
    t = h * 60 + m ;
    hope( t <= MINS_PER_DAY / 2 ) ;
    switch( c ) {
        case 'A': {
            break ;
        }
        case 'P': {
            t += MINS_PER_DAY / 2 ;
            break ;
        }
        case 'M': {
            hope( t == MINS_PER_DAY / 2 ) ;
            t = 0 ;
            break ;
        }
        case 'N': {
            hope( t == MINS_PER_DAY / 2 ) ;
            break ;
        }
        default: {
            hope( ! "this can't happen" ) ;
            break ;
        }    
    }
    hope( t < MINS_PER_DAY ) ;
    return t ;
}

static void printtime( FILE * fo, int t )
{
    int h ;
    hope( fo ) ;
    if( t > MINS_PER_DAY ) {
        int d ;
        d = t / MINS_PER_DAY ;
        hope( d ) ;
        t %= MINS_PER_DAY ;
        fprintf( fo, "%d day%s ", d, d == 1 ? "" : "s" ) ;
    }    
    h = t / 60 ;
    t %= 60 ;
    fprintf( fo, "%d:%02d", h, t ) ;
}

static int readcost( FILE * fi )
{
    float f ;
    int c ;
    hope( fi ) ;
    /* note the space below which skips all following whitespace, including enter */
    fscanf( fi, "%f ", &f ) ;
    c = round( f * 100 ) ;
    return c ;
}

static void printcost( FILE * fo, int c )
{
    int d ;
    hope( fo ) ;
    d = c / 100 ;
    c %= 100 ;
    fprintf( fo, "%d.%02d", d, c ) ;
}

static void printflight( FILE * fo, flight_t * f )
{
    city_t * from, * to ;
    hope( fo ) ;
    hope( f ) ;
    from = f->from ;
    hope( from ) ;
    to = f->to ;
    hope( to ) ;
    fprintf( fo, "%s->%s,", from->name, to->name ) ;
    printtime( fo, f->departure ) ;
    fputc( '-', fo ) ;
    printtime( fo, f->arrival % MINS_PER_DAY ) ;
    fputc( ',', fo ) ;
    printcost( fo, f->cost ) ;
    fputc( '\n', fo ) ;
}

static void readschedule( FILE * fi )
{
    city_t * from, * to ;
    hope( fi ) ;
    for( ; ; ) {
        int d, a, c ;
        from = readcity( fi ) ;
        if( ! from ) {
            break ;
        }
        to = readcity( fi ) ;
        hope( to ) ;
        d = readtime( fi ) ;
        a = readtime( fi ) ;
        if( d >= a ) {
            a += MINS_PER_DAY ;
        }
        c = readcost( fi ) ;
        addflight( from, to, d, a, c ) ;
        #ifdef DBG
        printflight( stdout, flight + flights - 1 ) ;
        #endif
    }
}

static int isoptimal( cache_t * c, int timereq, int time, int cost )
{
    int b ;
    hope( c ) ;
    if( timereq ) {
        b = time < c->time || ( time == c->time && cost < c->cost ) ;
    }
    else {
        b = cost < c->cost || ( cost == c->cost && time < c->time ) ;
    }
    return b ;
}

static void solverequest( cache_t * cache, city_t * from, city_t * to, int timereq,
                        int depth, flight_t * fl, int time, int cost )
{
    hope( from ) ;
    hope( to ) ;
    hope( ! depth || fl ) ;
    if( ! isoptimal( cache, timereq, time, cost ) ) {
        return ;
    }
    if( to == from ) {
        int i ;
        flight_t ** f ;
        hope( depth ) ;
        cache->flightlen = depth ;
        cache->time = time ;
        cache->cost = cost ;
        f = flightpool + cache->flightidx ;
        for( i = 0 ; i < depth ; i++, f++ ) {
            *f = flightstack[ i ] ;
        }
    }
    else {
        flight_t * p = from->flight ;
        from->used++ ;
        for( ; p ; p = p->next ) {
            city_t * t ;
            int ntime, ncost ;
            hope( p->from == from ) ;
            hope( p->to ) ;
            t = p->to ;
            if( t->used ) {
                continue ;
            }
            ntime = time ;
            ncost = cost ;
            /* add waiting on airport */
            if( fl ) {
                int d = p->departure - fl->arrival ;
                if( d < 0 ) {
                    d += MINS_PER_DAY ;
                }
                ntime += d ;
            }
            ntime += p->arrival - p->departure ;
            ncost += p->cost ;
            flightstack[ depth ] = p ;
            solverequest( cache, p->to, to, timereq, depth + 1, p, ntime, ncost ) ;
        }
        from->used-- ;
    }
}

static void solverequests( FILE * fi, FILE * fo )
{
    city_t * from, * to ;
    hope( fi ) ;
    for( ; ; ) {
        flight_t ** f ;
        static char buf[5] ;
        int r ;
        int fromi, toi ;
        cache_t * cache ;
        from = readcity( fi ) ;
        if( ! from ) {
            break ;
        }
        to = readcity( fi ) ;
        hope( to ) ;
        hope( from != to ) ;
        fromi = from - city ;
        toi = to - city ;
        hope( city + fromi == from ) ;
        hope( city + toi == to ) ;
        fscanf( fi, " %4s ",  buf ) ;
        r = 0 ;
        if( strcmp( buf, "TIME" ) == 0 ) {
            r = 1 ;
            cache = &timecache[ fromi ][ toi ] ;
        }
        else {
            hope( strcmp( buf, "COST" ) == 0 ) ;
            cache = &costcache[ fromi ][ toi ] ;
        }
        hope( cache ) ;
        if( ! cache->time ) {
            cache->flightidx = flightlast ;
            cache->flightlen = 0 ;
            cache->cost = cache->time = INT_MAX ;
            solverequest( cache, from, to, r, 0, NULL, 0, 0 ) ;
            hope( cache->flightlen ) ;
            flightlast += cache->flightlen ;
        }
        fprintf( fo, "%s->%s,", from->name, to->name ) ;
        printtime( fo, cache->time ) ;
        fputc( ',', fo ) ;
        printcost( fo, cache->cost ) ;
        fputc( '\n', fo ) ;
        r = cache->flightlen ;
        f = flightpool + cache->flightidx ;
        while( r-- ) {
            printflight( fo, *f++ ) ;
        }
        fputc( '\n', fo ) ;
    }
}

int main( void )
{
    int n ;
    fscanf( stdin, "%d\n", &n ) ;
    hope( n ) ;
    while( n-- ) {
        int i, j ;
        cities = 0 ;
        flights = 0 ;
        flightlast = 0 ;
        for( i = 0 ; i < MAX_CITIES ; i++ ) {
            for( j = 0 ; j < MAX_CITIES ; j++ ) {
                timecache[ i ][ j ].time = 0 ;
                costcache[ i ][ j ].time = 0 ;
            }
        }
        readschedule( stdin ) ;
        solverequests( stdin, stdout ) ;
    }

/* OK_FIXED: added */
return 0;
}
