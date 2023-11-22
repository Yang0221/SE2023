#ifdef CHANGED

//----------------------------------------------------------------------//
//                       Université de Bordeaux                         //
//                             BECHER IHEB                              //
//                               YANG YI                                //
//----------------------------------------------------------------------//

#include "copyright.h"
#include "system.h"
#include "consoledriver.h"
#include "synch.h"
#include <limits.h>

static Semaphore *readAvail;
static Semaphore *writeDone;

static Lock * lockChar   = new Lock("myLockChar");
static Lock * lockString = new Lock("myLockString"); 

static void ReadAvailHandler(void *arg) { (void) arg; readAvail->V(); }
static void WriteDoneHandler(void *arg) { (void) arg; writeDone->V(); }
ConsoleDriver::ConsoleDriver(const char *in, const char *out)
{
readAvail = new Semaphore("read avail", 0);
writeDone = new Semaphore("write done", 0);
console = new Console (in, out, ReadAvailHandler, WriteDoneHandler, NULL);
}
ConsoleDriver::~ConsoleDriver()
{
delete console;
delete writeDone;
delete readAvail;
delete lockChar   ;
delete lockString ;
}
void ConsoleDriver::PutChar(int ch)
{   
    lockChar->Acquire();
    console->TX (ch);
    writeDone->P ();
    lockChar->Release();
}
int ConsoleDriver::GetChar()
{  
    lockChar->Acquire();
    readAvail->P();
    int ch = console->RX() ;
    lockChar->Release();
    return ch ;
}
void ConsoleDriver::PutString(const char s[])
{   
    lockString->Acquire();
    for(int i=0; s[i]!='\0'; i++)
    {
        PutChar(s[i]);
    }
    lockString->Release();
}
void ConsoleDriver::GetString(char *s, int n)
{
    int i = 0;
    int c = GetChar();
    s[i] = c;
    i++;

    lockString->Acquire();
    
    while(c!=EOF && c!='\n' && c!='\0' && i<n)
    {
        c = GetChar();
        s[i] = c;
        i++;
    }
    lockString->Release();
    // if(s[i-1]!='\0')
    // {
    //     s[i]='\0';
    // }   
}
void ConsoleDriver::PutInt(int n)
{
   // printf("n =%d\n",n);
    int nbrchiffre=0;
    int i=n;
    if(i<0)//Si c'est  negatif
    {
        nbrchiffre++;
    }
    do
    {
        i/=10;
        nbrchiffre++;
    }while (i != 0);
    char s[nbrchiffre+1]; 
    snprintf(s,nbrchiffre+1,"%d",n);
    PutString(s);
}


/* Get an int from consoledriver  */
void ConsoleDriver::GetInt(int *  n)
{
    int * nb = new int;
    char * str = new char[ 11 ]; // 10 le nombre de chiffre maximun en c 
    int i = 0 ;
    i++ ;
    /*get string */ 
    GetString ( str , 11 );
    sscanf ( str ,"%d", nb );
    
    machine->WriteMem ( * n ,sizeof(int), * nb );
    
    delete [] str;
    delete nb ;
}

#endif // CHANGED