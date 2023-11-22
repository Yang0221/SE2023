#include "syscall.h"

int	main()
{  
   ForkExec("../test/programme1");
   ForkExec("../test/putchar");
   ForkExec("../test/programme0");
   int a = 5 ;
   a++ ;
   Exit(-1);
   return  0 ;
}

