
#include "syscall.h"

int	main()
{   
    char c;

    PutString("type a char: \n");
    c = GetChar();
    PutString("**Received** : ");
    PutChar(c); 
    PutString("\n");

    Exit(0);
}