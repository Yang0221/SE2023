

#include "syscall.h"

int	main()
{
    int a;
    PutString("Enter an integer  \n");
    GetInt(&a);
    PutString("*** number Received *** : ");
    PutInt(a); 
    PutString("\n");
    return 0;
}