#include "syscall.h"


void print(char c, int n)
{
	int i;
		for (i = 0; i < n; i++) {
		PutChar(c + i);
		}
		PutChar('\n');
}
int main(){
    
    print('A',5);
	print('a',5);
    
    return 0 ;
}
