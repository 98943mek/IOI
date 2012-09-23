#include<stdio.h>
#include <string.h>
#define MAX 1000

char loglan_line[MAX],sentence[MAX];
int i,len,ptr;
char token,ch;
int eof,eol;

void movmem(char *p,char *q,long r) {
 for (;r;r--) *(q++)=*(p++);
}

void getline() {
 gets(loglan_line);
 if (!(eof=(loglan_line[0]=='#'))) {
  len=strlen(loglan_line);
  eol=0;
  ptr=0;
  ch=' ';
  }
 else
  len=0;
 }

void getch() {
 if (ptr>=len) {
  getline();
  ch=' ';
  }
 else {
  ch=loglan_line[ptr++];
  if ((ch>='A')&&(ch<='Z')) ch=ch-'A'+'a';
  eol=(ch=='.');
  }
 }

void skip() {
 while (ch==' ') getch();
 }

int getnexttoken() {
 char firstchar;
 char word[MAX];
 int lword;
 skip(); word[0]=0; lword=0; firstchar=ch;
 if (eol) return 0;
 while ((ch>='a')&&(ch<='z')) {
  word[lword]=((ch=='a')||(ch=='e')||(ch=='i')||(ch=='o')||(ch=='u')) ? 'V' : 'C';
  word[++lword]=0;
  getch();
  }
 if ((word[0]=='V')&&(lword==1)) { token='A'; return 1; }
 if ((word[0]=='C')&&(word[1]=='V')&&(lword==2)) {
  switch(firstchar) {
  case 'g': token='M'; break;
  case 'b': token='B'; break;
  case 'd': token='D'; break;
  case 'l': token='L'; break;
  }
  return 1; }
 if ((word[0]=='C')&&(word[1]=='C')&&(word[2]=='V')&&(word[3]=='C')&&(word[4]=='V')&&(lword==5))
  { token='P'; return 1; }
 if ((word[0]=='C')&&(word[1]=='V')&&(word[2]=='C')&&(word[3]=='C')&&(word[4]=='V')&&(lword==5))
  { token='P'; return 1; }
 token='N'; return 1;
 }

int pos(char *substr,char *str) {
 int sslen=strlen(substr);
 int slen=strlen(str);
 int i;
 for (i=0; i<=slen-sslen; i++)
  if (!memcmp(&str[i],&substr[0],sslen))
   return i+1;
 return 0;
}

void shorten(char *nts,char *nt) {
 int l,l1,l2,l3;
 l3=strlen(nt);
 l=pos(nts,sentence);
 l1=strlen(nts);
 while (l) {
  l2=strlen(sentence);
  movmem(&sentence[l+l1-1],&sentence[l-1+l3],l2);
  movmem(&nt[0],&sentence[l-1],l3);
  sentence[l-1+l3+l2]=0;
  l=pos(nts,sentence);
  }
 }

void processline() {
 sentence[0]=0;
 while (!eol)
  if (getnexttoken())
   { sentence[strlen(sentence)+1]=0;
     sentence[strlen(sentence)]=token;
   }
 shorten("P","G"); shorten("GG","G");
 shorten("LG","Z"); shorten("N","Z");
 shorten("AG","R"); shorten("MG","V");
 shorten("G","R"); shorten("RR","R");
 shorten("ZVZ","T");
 shorten("ZV","T");
 shorten("DR","Q"); shorten("ZBR","Q");
 shorten("Q","S"); shorten("T","S");
 if ((sentence[0]=='S')&&(sentence[1]==0))
  printf("Good\n");
 else
  printf("Bad!\n");
 }

int main() {
 getline();
 while (!eof) { processline(); getline(); }
 return 0;
}
