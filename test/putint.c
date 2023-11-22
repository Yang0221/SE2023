

#include "syscall.h"
#include <limits.h>

int	main()
{
    int a= 110;
    int b = 1365;
    int c = -16;
    int d = -255;
    int e = 0 ;
    PutString("test1 a  : ");
    PutInt(a);
    PutString("\n");
    
    PutString("test2 b: ");
    PutInt(b);
    PutString("\n");

    PutString("test3 c: ");
    PutInt(c);
    PutString("\n");

    PutString("test4 d: ");
    PutInt(d);
    PutString("\n");

    PutString("test5 e: ");
    PutInt(e);
    PutString("\n");

    PutString("test6 f: ");
    PutInt(123546481989448921321564984);
    PutString("\n");
    
    
    PutInt(INT_MAX);
       
    PutString("\n");
    
    PutInt(INT_MIN);
    
    PutString("\n");

    return 0;
}