//MASTERMIND JAMES ANTHONY DY QUIEN 999777308

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (void)
{
int guess,length,current,dummy,count;

printf("Enter the pattern length: ");
scanf("%d",&length);
printf("Enter the number of guesses: ");
scanf("%d",&guess);
printf("Input the key pattern with no spaces: ");
char key[length+1];
scanf("%c",&dummy);

 for(count=0;count<length;count++)
{
 scanf("%c",&current);
 key[count]=current;
}
key[count]='\0';

char answers[guess][(length+1)];
char results[guess][(length+1)];
int row,col;
for(row=0;row<guess;row++)
 for (col=0;col<=length;col++)
{
  answers[row][col]=0;
  results[row][col]=0;
  answers[row][length]='\0';
  results[row][length]='\0';
}


int guessnum=0,match=0,bcount,n;
double wcount=0.0,keycount,answerscount;
int j;
//HERE WE START A BIG LOOP TO COMPARE AND START SCORING THE GAME!!!!!
for(j=0;j<30;j++)
printf("\n");
while ((match!=1)&&(guessnum<guess))
{
printf("Input a guess pattern with no spaces:");

scanf("%c",&dummy);

//SCANNING FOR GUESS
 for(count=0;count<length;count++)
{
 scanf("%c",&current);
 answers[guessnum][count]=current;
}
 answers[guessnum][count]='\0';


//GET SCORES BY THEIR PLACE, THEN SORT THEM OUT
//B'S and dots
for(count=0,bcount=0;count<length;count++)
{
 if(answers[guessnum][count]==key[count])
  {
  results[guessnum][count]='b';
  bcount++;
  }
 else
  results[guessnum][count]='.';
}


for(wcount=0,count=0;count<length;count++)
{                                        
//this checks for the repeated values in each array, BUT NOT THE ONES ALREADY USED FOR B's
   keycount=0.0;
   answerscount=0.0;
  for(n=0;n<length;n++)  
   {
   if((key[n]==answers[guessnum][count])&&(results[guessnum][count]!='b')&&(results[guessnum][n]!='b'))
    keycount=keycount+1;   
   else
    ;
   }

  for(n=0;n<length;n++)
   {
   if((answers[guessnum][n]==answers[guessnum][count])&&(results[guessnum][count]!='b')&&(results[guessnum][n]!='b'))                               
    answerscount=answerscount+1.0;
   else
    ;
   }
  if(answerscount!=0)
   {
   if((keycount>=answerscount)&&(keycount!=0))
    wcount=wcount+1.0;
   else
     wcount=wcount+(keycount/answerscount); 
   }
  else
   ;
}

//TO SORT THEM OUT, FIND OUT HOW MANY OF EACH THERE ARE AND PLACE ACCORDINGLY 
for(count=0;count<bcount;count++)
  results[guessnum][count]='b';
  
for(;count<(bcount+wcount);count++)
  results[guessnum][count]='w';
  
for(;count<length;count++)
  results[guessnum][count]='.';
 
for(n=0;n<=guessnum;n++)
printf("%d: %s %s\n",(n+1),(results[n]),(answers[n]));

if(strcmp(answers[guessnum], key)==0)
{
 match=1;
 printf("You guessed the pattern!\n");
}
else
 ;
guessnum++;
}

if(match==0)
 printf("You did not guess the pattern!\n");
else
 ;
return 0;
}
