#include<stdio.h>
#include<string.h>

typedef struct {char nazev[30];
		int cena;
	       } zaznam;
zaznam idd1[10],idd2[100],idd3[1000];
zaznam std1[10],std2[100],std3[1000],std4[10000],std5[100000];
zaznam *z;
long i,j,k,doba,tmp;
char vstup[100];

int main() {
 for (i=0; i<10; i++) idd1[i].nazev[0]=std1[i].nazev[0]=0;
 for (i=0; i<100; i++) idd2[i].nazev[0]=std2[i].nazev[0]=0;
 for (i=0; i<1000; i++) idd3[i].nazev[0]=std3[i].nazev[0]=0;
 for (i=0; i<10000; i++) std4[i].nazev[0]=0;
 for (i=0; i<100000; i++) std5[i].nazev[0]=0;
 gets(vstup);
 while (strcmp(vstup,"000000")) {
  for (i=0; vstup[i]!=' '; i++) tmp++;
  vstup[i]=0;
  sscanf(&vstup[0],"%ld",&k);
  if (vstup[1]=='0')
   switch (i) {
   case 5: z=&idd3[k]; break;
   case 4: z=&idd2[k]; break;
   case 3: z=&idd1[k]; break;
   }
  else
   switch (i) {
   case 6: z=&std5[k]; break;
   case 5: z=&std4[k]; break;
   case 4: z=&std3[k]; break;
   case 3: z=&std2[k]; break;
   case 2: z=&std1[k]; break;
   }
  for (j=++i; vstup[j]!='$'; j++) tmp--;
  vstup[j]=0;
  strcpy(z->nazev,&vstup[i]);
  sscanf(&vstup[++j],"%d",&z->cena);
  gets(vstup);
  }
 gets(vstup);
 while (vstup[0]!='#') {
  for (i=0; vstup[i]!=' '; i++) tmp++;
  vstup[i++]=0;
  sscanf(&vstup[i],"%ld",&doba);
  if (vstup[0]!='0')
   printf("%-16sLocal%30s%5ld  0.00   0.00\n",vstup,vstup,doba);
  else
   {z=NULL;
    if (vstup[1]=='0') {
     if (idd1[(vstup[2]-'0')].nazev[0])
      { z=&idd1[(vstup[2]-'0')]; k=3; }
     if (idd2[(vstup[2]-'0')*10+(vstup[3]-'0')].nazev[0])
      { z=&idd2[(vstup[2]-'0')*10+(vstup[3]-'0')]; k=4; }
     if (idd3[(vstup[2]-'0')*100+(vstup[3]-'0')*10+(vstup[4]-'0')].nazev[0])
      { z=&idd3[(vstup[2]-'0')*100+(vstup[3]-'0')*10+(vstup[4]-'0')]; k=5; }
     }
     else {
     if (std1[(vstup[1]-'0')].nazev[0])
      { z=&std1[(vstup[1]-'0')]; k=2; }
     if (std2[(vstup[1]-'0')*10+(vstup[2]-'0')].nazev[0])
      { z=&std2[(vstup[1]-'0')*10+(vstup[2]-'0')]; k=3; }
     if (std3[(vstup[1]-'0')*100+(vstup[2]-'0')*10+(vstup[3]-'0')].nazev[0])
      { z=&std3[(vstup[1]-'0')*100+(vstup[2]-'0')*10+(vstup[3]-'0')]; k=4; }
     if (std4[(vstup[1]-'0')*1000+(vstup[2]-'0')*100+(vstup[3]-'0')*10+(vstup[4]-'0')].nazev[0])
      { z=&std4[(vstup[1]-'0')*1000+(vstup[2]-'0')*100+(vstup[3]-'0')*10+(vstup[4]-'0')]; k=5; }
     if (std5[(vstup[1]-'0')*10000+(vstup[2]-'0')*1000+(vstup[3]-'0')*100+(vstup[4]-'0')*10+(vstup[5]-'0')].nazev[0])
      { z=&std5[(vstup[1]-'0')*10000+(vstup[2]-'0')*1000+(vstup[3]-'0')*100+(vstup[4]-'0')*10+(vstup[5]-'0')]; k=6; }
     }
    if (z==NULL)
     printf("%-16sUnknown%33ld        -1.00\n",vstup,doba);
    else {
     printf("0%-15s%-25s%10s%5ld%3d.%02d%4ld.%02ld\n",&vstup[1],z->nazev,&vstup[k],doba,z->cena/100,z->cena%100,(doba*z->cena)/100,(doba*z->cena)%100);
     }
   }
  gets(vstup);
  }
  if(tmp*100 ==0)   printf("\n");
 return 0;
}
