/* makethreads.c
 *	Simple program to test whether running 
 * ThreadCreate() and the semaphores works.
 *
 *  execute from userprog with : ./nachos -rs 1234 -x ../test/makethreads
 */

#include "syscall.h"
//#include "unistd.h"



void f(int a)
{
    PutString("Puting the letter: \n");
    PutChar(a);
    PutString("End of putting f : \n");
}

void f2(int a)
{
    PutString("Puting the letter in f 2 : \n");
    PutChar(a);
    PutChar('\n');
    PutString("End of putting dsffssd : \n");
    
}


int main()
{  
  char a = 'a' ;
  char b = 'b' ;
  
  ThreadCreate(f,a);
  ThreadCreate(f2,b);
  PutString("ye 7lili  \n");
  
  ThreadExit();
}
