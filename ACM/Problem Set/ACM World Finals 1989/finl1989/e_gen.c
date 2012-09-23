/*
        Input generator

        Problem:        E
        Author:         Ondrej Karny
        Date:           1998-Apr-04
*/
/*
 * ACM International Collegiate Programming Contest Finals 1989
 *
 * Problem E - Robot Crash
 *
 * Input generator
 *
 * Format of the single input:
 *
 * x y angle speed for left one
 * x y angle speed for right one
 *
 * y must be 0<= <=10
 * angle is in degrees
 * left angle -85 <= <= 85
 * right angle -95 >= >= -180 || 95 <= <= 180
 *
 */


#include <stdio.h>
#include <math.h>
#include <stdlib.h>





#ifndef M_PI
#ifdef PI
#define M_PI PI
#else  /*ndef PI*/
#define M_PI 3.14159265358979323846
#endif /*ndef PI*/
#endif /*ndef M_PI*/


#define MAX_DELAY 0.5
#define MAX_Y 10.0

#define INFINITY 10.0e10
#define EPSILON 0.0001


#define RADIANS( angle ) (( angle ) * M_PI / 180.0 )


typedef struct _Gun {
    double x;
    double y;
    double angle;
    double speed;
    double xSpeed;
    double ySpeed;
    } Gun;

typedef struct _Segment {
    double x;
    double y;
    double dx;
    double dy;
    double start;
    double end;
    } Segment;


static Gun leftGun;
static Gun rightGun;
static int collision;
static double collisionX;
static double collisionY;


#define FIXME( a ) printf a



static int addSuccessive( const Segment *segment )
{
Segment *next;

next = (Segment *)(segment + 1);
/*the succesive segment is reversed*/
next->x = segment->x;
next->dx = segment->dx;
next->dy = - (segment->dy);
next->start = segment->end;
if ( segment->dy > 0.0 ) {
    if ( next->dy > -EPSILON ) return 1;
    /*this segment goes up, next is top-bottom*/
    next->y = MAX_Y - next->dy * next->start;
    next->end = ( 0.0 - next->y ) / next->dy;
    }
else {
    if ( next->dy < EPSILON ) return 1;
    /*this segment goes down, next is bottom-top*/
    next->y = 0.0 - next->dy * next->start;
    next->end = ( MAX_Y - next->y ) / next->dy;
    }
return 0;
}


static int addPrevious( const Segment *segment )
{
Segment *prev;

prev = (Segment *)(segment - 1);
/*the previous segment is reversed*/
prev->x = segment->x;
prev->dx = segment->dx;
prev->dy = - (segment->dy);
prev->end = segment->start;
if ( segment->dy > 0.0 ) {
    if ( prev->dy > -EPSILON ) return 1;
    /*this segment goes up, previous is top-bottom*/
    prev->y = 0.0 - prev->dy * prev->end;
    prev->start = ( MAX_Y - prev->y ) / prev->dy;
    }
else {
    if ( prev->dy < EPSILON ) return 1;
    /*this segment goes down, previous is bottom-top*/
    prev->y = MAX_Y - prev->dy * prev->end;
    prev->start = ( 0.0 - prev->y ) / prev->dy;
    }
return 0;
}


/*
 * Compute line segment for given gun and time.
 * Also compute previous and successive segments
 * if it does make sense.
 *
 */
static int segmentsFromTime( const Gun *gun, double time, Segment *segment )
{
double x;
double y;

/*segment defaults*/
segment->x = gun->x;
segment->y = gun->y;
segment->dx = gun->xSpeed;
segment->dy = gun->ySpeed;
/*compute position at given time*/
x = gun->x + time * gun->xSpeed;
y = gun->y + time * gun->ySpeed;
if ( y >= 0.0 ) {
    if ( y <= MAX_Y ) {
        /*no bounces at begin - simply goes from gun to the given point*/
        if ( y < EPSILON ) return 1;
        if ( y > MAX_Y - EPSILON ) return 1;
        segment->start = 0.0;
        if ( segment->dy != 0.0 ) {
            /*isn't horizontal, so the segment must end anywhere*/
            if ( fabs( segment->dy ) < EPSILON ) return 1;
            if ( fabs( segment->y - MAX_Y ) < EPSILON ) return 1;
            if ( segment->dy > 0.0 ) {
                segment->end = ( MAX_Y - segment->y ) / segment->dy;
                }
            else {
                segment->end = ( 0.0 - segment->y ) / segment->dy;
                }
            if ( segment->end > INFINITY ) return 1;
            if ( addSuccessive( segment )) return 1;
            /*there's no previous segmet*/
            }
        else {
            /*the segment is exactly horizontal, no previous, no next*/
            segment->end = INFINITY;
            }
        }
    else {
        double realY;

        /*there're some bounces between begin and the desired segment*/
        /*but the first bounce is on top edge, so the dy > 0.0*/
        if ( segment->dy < EPSILON ) return 1;
        realY = fmod( y, 2 * MAX_Y );
        if ( realY > MAX_Y ) {
            /*the segment is top-bottom segment*/
            segment->dy = - (segment->dy);
            realY = 2 * MAX_Y - realY;
            segment->y = realY - segment->dy * time;
            if ( fabs(( segment->y + segment->dy * time ) - realY ) >
                            EPSILON ) return 1;
            segment->start = ( MAX_Y - segment->y ) / segment->dy;
            segment->end = - segment->y / segment->dy;
            if ( segment->start > time ) return 1;
            if ( segment->end < time ) return 1;
            }
        else {
            /*the segment is bottom-top segment*/
#if 0
            /* this equation could be written more readibly */
            segment->y -= ( 2 * MAX_Y ) * (int)( y / ( 2 * MAX_Y ));
#else /* 0 */
            segment->y = realY - segment->dy * time;
#endif /* 0 */
            if ( fabs(( segment->y + segment->dy * time ) - realY ) >
                             EPSILON ) return 1;
            segment->start = - segment->y / segment->dy;
            segment->end = ( MAX_Y - segment->y ) / segment->dy;
            if ( segment->start > time ) return 1;
            if ( segment->end < time ) return 1;
            }
        if ( addPrevious( segment )) return 1;
        if ( addSuccessive( segment )) return 1;
        }
    }
else {
    double realY;

    /*there're some bounces between begin and the desired segment*/
    /*but I know the first bounce is on bottom edge, so the dy < 0.0*/
    if ( segment->dy > - EPSILON ) return 1;
    realY = fmod( -y, 2 * MAX_Y );
    if ( realY > MAX_Y ) {
        /*top-bottom segment*/
        realY = 2 * MAX_Y - realY;
        segment->y = realY - segment->dy * time;
        if ( fabs(( segment->y + segment->dy * time ) - realY ) >
                            EPSILON ) return 1;
        segment->start = ( MAX_Y - segment->y ) / segment->dy;
        segment->end = - segment->y / segment->dy;
        if ( segment->start > time ) return 1;
        if ( segment->end < time ) return 1;
        }
    else {
        /*bottom-top segment, flip the dy*/
        segment->dy = - (segment->dy);
        segment->y = realY - segment->dy * time;
        if ( fabs(( segment->y + segment->dy * time ) - realY ) >
                             EPSILON ) return 1;
        segment->start = - segment->y / segment->dy;
        segment->end = ( MAX_Y - segment->y ) / segment->dy;
        if ( segment->start > time ) return 1;
        if ( segment->end < time ) return 1;
        }
    if ( addSuccessive( segment )) return 1;
    if ( addPrevious( segment )) return 1;
    }
return 0;
}


/*
 * Try to intersect two line segments,
 * given their parametric equation and
 * parameter constraints. Return non-zero
 * if the segments intersect.
 *
 */
static int intersectSegments( const Segment *left,
            const Segment *right, double *x, double *y, double *t )
{
double u;
double v;
double det;

det = left->dy * right->dx - left->dx * right->dy;
if ( fabs( det ) < 1e-6 ) {
    det = 0.0;
    }
if (( det != 0.0 ) &&
       ( fabs( det ) < EPSILON )) return 2;
if ( det == 0.0 ) {
    det = left->dx * ( right->y - left->y ) -
          left->dy * ( right->x - left->x );
    if ( fabs( det ) < 1e-6 ) {
        det = 0.0;
        }
    if (( det != 0.0 ) &&
           ( fabs( det ) < EPSILON )) return 2;
    if ( det == 0.0 ) {
        double v;

        v = ( left->x - right->x ) / ( right->dx - left->dx );
        if ( fabs( v - left->start ) < EPSILON ) return 2;
        if ( fabs( v - left->end ) < EPSILON ) return 2;
        if ( fabs( v - right->start ) < EPSILON ) return 2;
        if ( fabs( v - right->end ) < EPSILON ) return 2;
        if (( v > left->start ) &&
            ( v < left->end ) &&
            ( v > right->start ) &&
            ( v < right->end )) {
            *t = 0.0;
            *x = left->x + left->dx * v;
            *y = left->y + left->dy * v;
            return 1;
            }
/*
        FIXME(( "They are colinear and I have to minimize the time delta " ));
*/
        return 2;
        }
    else {
        /*they're parallel*/
        return 0;
        }
    }
else {
    double dx;
    double dy;

    dx = ( right->y - left->y ) * right->dx -
         ( right->x - left->x ) * right->dy;
    dy = left->dx * ( right->y - left->y ) -
         left->dy * ( right->x - left->x );
    u = dx / det;
    v = dy / det;
    if ( fabs( u - left->start ) < EPSILON ) return 2;
    if ( fabs( u - left->end ) < EPSILON ) return 2;
    if ( fabs( v - right->start ) < EPSILON ) return 2;
    if ( fabs( v - right->end ) < EPSILON ) return 2;
    if (( u > left->start ) &&
        ( u < left->end ) &&
        ( v > right->start ) &&
        ( v < right->end )) {
        *t = fabs( u - v );
        *x = left->x + left->dx * u;
        *y = left->y + left->dy * u;
        return 1;
        }
    else {
        return 0;
        }
    }
}


static int evaluate( const Gun *left, const Gun *right )
{
double distance;
double hitTime;

distance = right->x - left->x;
if ( distance < EPSILON ) return 1;
hitTime = distance / ( left->xSpeed - right->xSpeed );
{
double leftPos;
double rightPos;

leftPos = left->x + hitTime * left->xSpeed;
rightPos = right->x + hitTime * right->xSpeed;
if ( fabs( leftPos - rightPos ) > EPSILON ) return 1;
}   /*eob*/
{
Segment leftSegments[3];
Segment rightSegments[3];
int loLeft;
int loRight;
int hiLeft;
int hiRight;

if ( segmentsFromTime( left, hitTime, leftSegments + 1 )) return 1;
if ( segmentsFromTime( right, hitTime, rightSegments + 1 )) return 1;
loLeft = 1;
hiLeft = 1;
loRight = 1;
hiRight = 1;
if ( fabs(( hitTime - leftSegments[1].start ) - MAX_DELAY ) < EPSILON ) return 1;
if ( fabs(( leftSegments[1].end - hitTime ) - MAX_DELAY ) < EPSILON ) return 1;
if ( fabs(( hitTime - rightSegments[1].start ) - MAX_DELAY ) < EPSILON ) return 1;
if ( fabs(( rightSegments[1].end - hitTime ) - MAX_DELAY ) < EPSILON ) return 1;
if (( leftSegments[1].start > hitTime - MAX_DELAY ) &&
    ( leftSegments[1].start > 0.0 )) {
    loLeft = 0;
    }
if (( rightSegments[1].start > hitTime - MAX_DELAY ) &&
    ( rightSegments[1].start > 0.0 )) {
    loRight = 0;
    }
if ( leftSegments[1].end < hitTime - MAX_DELAY ) {
    hiLeft = 2;
    }
if ( rightSegments[1].end < hitTime - MAX_DELAY ) {
    hiRight = 2;
    }
{
double x;
double y;
double time;
double bestTime;

bestTime = INFINITY;
while ( loLeft <= hiLeft ) {
    int right;

    for ( right = loRight; right <= hiRight; right++ ) {
        int inter;

        inter = intersectSegments( leftSegments + loLeft,
                    rightSegments + right, &x, &y, &time );
        if ( inter == 2 ) return 1;
        if ( inter ) {
            if ( fabs( time - MAX_DELAY ) < EPSILON ) return 1;
            if ( time <= MAX_DELAY ) {
                if (( fabs( bestTime - time ) < EPSILON ) &&
                       ( bestTime != time )) return 1;
                collision = 1;
                if ( time < bestTime ) {
                    bestTime = time;
                    collisionX = x;
                    collisionY = y;
                    }
                else if ( time == bestTime ) {
                    if ( x < collisionX ) {
                        collisionX = x;
                        collisionY = y;
                        }
                    }
                }
            }
        }
    loLeft++;
    }
}   /*eob*/
}   /*eob*/
return 0;
}




/*
 * Compute the move vector for the
 * given gun. Also compute first bounce
 * time and bounce period.
 *
 */
static int setupGun( Gun *gun )
{
double angle;

angle = RADIANS( gun->angle );
gun->xSpeed = gun->speed * cos( angle );
gun->ySpeed = gun->speed * sin( angle );
return 0;
}


int test( double lx, double ly, double la, double ls,
          double rx, double ry, double ra, double rs )
{
leftGun.x = lx;
leftGun.y = ly;
leftGun.angle = la;
leftGun.speed = ls;
rightGun.x = rx;
rightGun.y = ry;
rightGun.angle = ra;
rightGun.speed = rs;
collision = 0;
    if ( leftGun.x > rightGun.x ) {
        /* they simply can't collide */
        }
    else {
        if ( leftGun.x == rightGun.x ) {
            if ( leftGun.y == rightGun.y ) {
                collision = 1;
                collisionX = leftGun.x;
                collisionY = leftGun.y;
                }
            else {
                /* they simply can't collide */
                }
            }
        else {
            /* shoot them against each other and test collision */
            if ( setupGun( &leftGun )) return 0;
            if ( setupGun( &rightGun )) return 0;
            if ( evaluate( &leftGun, &rightGun )) return 0;
            }
        }
    if ( collision ) {
        double result;
        double above;
        double beyond;

#define tworound( value ) floor(( value ) * 100.0 + 0.5 )

        result = collisionX;
        above = tworound( collisionX + EPSILON );
        beyond = tworound( collisionX - EPSILON );
        if ( tworound( result ) != above ) return 0;
        if ( tworound( result ) != beyond ) return 0;
        result = collisionY;
        above = tworound( collisionY + EPSILON );
        beyond = tworound( collisionY - EPSILON );
        if ( tworound( result ) != above ) return 0;
        if ( tworound( result ) != beyond ) return 0;
        }
    else {
        ;
        }
return 1;
}






double convert( double value )
{
char buffer[128];
double result;

sprintf( buffer, "%f", value );
sscanf( buffer, "%lf", &result );
return result;
}


double generateBug( void )
{
return ((double)( rand() % 65536 )) / 655536.0;
}


double generate( void )
{
return ((double)( rand() % 65536 )) / 65536.0;
}

int main( int argc, const char **argv )
{
int i;

/* trivial example */
fprintf( stdout,
    "0 4 0 3.3\n"
    "40 5 125 5\n"

    "1 6 -5 10\n"
    "5 2 95 20\n"

    "2 5 45 5\n"
    "42 5 -135 5\n"

    "0 6 20 3\n"
    "0 5 180 4\n" );

/* /FIXME: add some random (and not so random) stuff */

for ( i = 0; i < 45; i++ ) {
    double lx,rx;
    double ly,ry;
    double la,ra;
    double ls,rs;

    while ( 1 ) {
        lx = generateBug() * 5000.0 - 4000.0;
        ly = generateBug() * 10.0;
        la = generateBug() * 170.0 - 85.0;
        ls = generateBug() * 1000.0;
        rx = generateBug() * 5000.0 - 1000.0;
        ry = generateBug() * 10.0;
        ra = generateBug() * 170 + 95.0;
        if ( ra > 180.0 ) {
            ra = -95.0 - ( ra - 180.0 );
            }
        rs = generateBug() * 1000.0;
        if ( test( convert( lx ), convert( ly ), convert( la ), convert( ls ),
                   convert( rx ), convert( ry ), convert( ra ), convert( rs ))) {
            break;
            }
        }
    fprintf( stdout, "%f %f %f %f\n",
            lx, ly, la, ls );
    fprintf( stdout, "%f %f %f %f\n",
            rx, ry, ra, rs );
    }


for ( i = 0; i < 5000; i++ ) {
    double lx,rx;
    double ly,ry;
    double la,ra;
    double ls,rs;

    while ( 1 ) {
        lx = generate() * 5000.0 - 4000.0;
        ly = generate() * 10.0;
        la = generate() * 170.0 - 85.0;
        ls = generate() * 1000.0 + 0.0001;
        rx = generate() * 5000.0 - 1000.0;
        ry = generate() * 10.0;
        ra = generate() * 170 + 95.0;
        if ( ra > 180.0 ) {
            ra = -95.0 - ( ra - 180.0 );
            }
        rs = generate() * 1000.0 + 0.0001;
        if ( test( convert( lx ), convert( ly ), convert( la ), convert( ls ),
                   convert( rx ), convert( ry ), convert( ra ), convert( rs ))) {
            break;
            }
        }
    fprintf( stdout, "%f %f %f %f\n",
            lx, ly, la, ls );
    fprintf( stdout, "%f %f %f %f\n",
            rx, ry, ra, rs );
    }
return 0;
}

