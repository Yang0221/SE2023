#ifdef CHANGED

//----------------------------------------------------------------------//
//                       Université de Bordeaux                         //
//                             BECHER IHEB                              //
//                               YANG YI                                //
//----------------------------------------------------------------------//
#include "system.h"
#include "consoledriver.h"
#include "userCode.h"




int copyStringFromMachine (int from , char * to , unsigned size )
{
    int i = 0;
    int ch ;
    int r=0 ;
    while(( i < (int) size )&&( r=machine->ReadMem( from + i , 1 , &ch ))&& (char) ch !='\0') {
    //printf("i am here ");
    if(i>0)
        if(to[i-1]=='\0'){
            break ;
        }
    *(to+i)= (char)ch;
	i++;
    }
    to[i]='\0';
    return i ;
}

int copyStringToMachine(char* from, int to, unsigned size)
{
    DEBUG('u', "copystringtomachine \n");
    unsigned i=0;
    while(from[i]!=EOF && from[i]!='\0' && from[i]!='\n' && i<size)
    {
        machine->WriteMem(to+i, 1, from[i]);
        i++;
    }
    machine->WriteMem(to+i,1,'\0');
    return i;
}
#endif // CHANGED