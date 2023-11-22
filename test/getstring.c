
#include "syscall.h"

int	main()
{   
    char c[2048]; 
    PutString("type a string  press enter: \n");
    GetString(c, 1024);
    PutString("**Received** : ");PutString(c); PutString(" \n");
    
    Exit(0);
}