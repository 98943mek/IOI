/* POZOR: TOTO NEFUNGUJE, lastmsg[] funguje v trivialnich pripadech, ale ne
   vzdy! g.out v tarballu je momentalne spravne. */
#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>

#define MAXPEO 100000
#define MAXRCPT 10000
#define LINELEN 100
#define MESSGLEN 10000

#define indent printf("     ")

char *table[MAXPEO];
char lastmsg[MAXPEO];

int hashfn( const char *n )
{
int a, i;
a=0;
for( i=0; n[i]!=0; ++i ) a+=(int)(n[i])*(1+(int)(n[i+1]));
 a %= MAXPEO;
 if (a < 0)
   a += MAXPEO;
 return a%MAXPEO;
}

void zarad( const char *n )
{
int kam;
kam=hashfn( n );
while( table[kam]!=0 ) kam=(kam+1)%MAXPEO;
table[kam]=strdup( n );
}

const char *getcomp( const char *n )
{
const char *a;
a=n;
while( *a!='@' ) ++a;
return ++a;
}

int vezmi( const char *n )
{
int kam;
kam=hashfn( n );
while( table[kam]!=NULL && strcmp(table[kam],n)!=0 ) kam=(kam+1)%MAXPEO;
if( table[kam]==NULL ) return -1;
return kam;
}

int main(void)
{
char comp[50], name[50], pom[100], from[50], to[50];
char rcpt[MAXRCPT][50];
const char *rcptcomp[MAXRCPT];
const char *fcomp;
char messg[MESSGLEN][LINELEN];
int n, i, j, k, rcptnum, lines, rgood, mnum, komu;
scanf( "%s", pom );
while( pom[0]!='*' )
  {
  scanf( "%s %d", comp, &n );
  for( i=0; i<n; ++i )
    {
    scanf( "%s", name );
    sprintf( pom, "%s@%s", name, comp );
    zarad( pom );
    }
  scanf( "%s", pom );
  }
mnum=1;  
scanf( "%s", from );
while( from[0]!='*' )
  {
  int c;
  fcomp=getcomp( from );
  rcptnum=0;
  scanf( "%s", to);
  while( to[0]!='*' )
    {
    strcpy( rcpt[rcptnum], to );
    rcptcomp[rcptnum]=getcomp( rcpt[rcptnum] );
    ++rcptnum;
    scanf( "%s", to );
    } 
  lines=0;
  while ((c = getchar()) != EOF && (c == ' ' || c == '\t'))
    ;
  if (c != '\n')
    ungetc(c, stdin);
  gets( messg[0] );
  while( messg[lines++][0]!='*' )
    {
    gets( messg[lines] );
    }
  --lines;  
  for( i=0; i<rcptnum; ++i )
    {
    if( rcpt[i][0]!=0 )
      {
      printf( "Connection between %s and %s\n", fcomp, rcptcomp[i] );
      indent;printf( "HELO %s\n", fcomp );
      indent;printf( "250\n" );
      indent;printf( "MAIL FROM:<%s>\n", from );
      indent;printf( "250\n" );
      rgood=0;
      for( j=i; j<rcptnum; ++j )
        {
        if( rcpt[j][0]!=0 && strcmp( rcptcomp[j], rcptcomp[i] )==0 )
          {
          komu=vezmi( rcpt[j] );
          if( komu==-1 )
            {
            indent;printf( "RCPT TO:<%s>\n", rcpt[j] );
            indent;printf( "550\n" );
            /*********** spoleham, ze neni moc pripadu nesmysle adresy ****/
            for( k=j+1; k<rcptnum; ++k )
              if( strcmp( rcpt[j], rcpt[k] )==0 ) rcpt[k][0]=0;  
            }
          else
            {
            if( lastmsg[komu]!=mnum )
              {
              indent;printf( "RCPT TO:<%s>\n", rcpt[j] );
              indent;printf( "250\n" );
              lastmsg[komu]=mnum;
              ++rgood;
              }
            }
          rcpt[j][0]=0;    
          }
        }
      if( rgood>0 )
        {
        indent;printf( "DATA\n" );
        indent;printf( "354\n" );
        for( k=0; k<lines; ++k ) 
          {
          indent;puts( messg[k] );
          }
        indent;printf( ".\n");
	indent;printf( "250\n" );  
        }  
      indent;printf( "QUIT\n" );
      indent;printf( "221\n" );    
      }
    }  
  scanf( "%s", from );
  ++mnum;
  }  
return 0;
}
