//The MasterMind Assistant James Anthony Dy Quien 999777308

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void PrintPattern(char solution[],char key[],int col,int size,int black,int dot,int a,int b,int c,int d,int e,int f,int bcount,int dcount)
{
 int i,a2,b2,c2,d2,e2,f2;
 char row;

 if (col>=size)
{
  if(dcount==dot&&bcount==black)
 {
   for(i=0;i<size;i++)
    printf("%c",solution[i]);
   printf("\n");
   
   return;
 }
 else
  return;
}
//Once it reaches a solution it will print it


for (row='a';row<='f';row++)
 {
  solution[col]=row;
  bool safe=true;
//checking every "row" for a possible answer in the metaphorical table of all possible answers

  for(i=0,a2=0,b2=0,c2=0,d2=0,e2=0,f2=0,dcount=0,bcount=0;i<=col;i++)
   {
    if(solution[i]==key[i])
     bcount++;
    if(solution[i]=='a')
     a2++;
    if(solution[i]=='b')
     b2++;
    if(solution[i]=='c')
     c2++;
    if(solution[i]=='d')
     d2++;
    if(solution[i]=='e')
     e2++;
    if(solution[i]=='f')
     f2++;
    if((solution[i]=='a'&&a2>a)||((solution[i]=='b')&&(b2>b))||((solution[i]=='c')&&(c2>c))||((solution[i]=='d')&&(d2>d))||((solution[i]=='e')&&(e2>e))||((solution[i]=='f')&&(f2>f)))
     dcount++;
    }

   if ((bcount>black)||(dcount>dot))
    safe=false;
//checking all past columns to see if everything fits the algorithm

if(safe==true)

    PrintPattern(solution,key,col+1,size,black,dot,a,b,c,d,e,f,bcount,dcount);
//recurses for the next column else it goes for the next letter value
 }
}

int main (void)
{

int count,dummy,current,length,blacks,whites,dots,a=0,b=0,c=0,d=0,e=0,f=0;

printf("Enter the pattern length:");
scanf("%d",&length);
printf("Input the guess pattern:");
scanf("%d",&dummy);

char key[(length+1)];
char solution[(length+1)];
solution[(length+1)]='\0';

 for(count=0;count<length;count++)
{
 scanf("%c",&current);

 key[count]=current;

if(key[count]=='a')
 a++;
if(key[count]=='b')
 b++;
if(key[count]=='c')
 c++;
if(key[count]=='d')
 d++;
if(key[count]=='e')
 e++;
if(key[count]=='f')
 f++;
}
 key[count]='\0';

printf("Enter the number of black pegs in the feedback:");
scanf("%d",&blacks);
printf("Enter the number of white pegs in the feedback:");
scanf("%d",&whites);

dots=length-(blacks+whites);
char *dom=solution;
char *range=key;

dom=(char*)malloc(length+1);
range=(char*)malloc(length+1);

printf("The possible key patterns are:\n");
PrintPattern(solution,key,0,length,blacks,dots,a,b,c,d,e,f,0,0);

free(dom);
free(range);
return 0;
}
