#ifdef CHANGED

//----------------------------------------------------------------------//
//                       Université de Bordeaux                         //
//                             BECHER IHEB                              //
//                               YANG YI                                //
//----------------------------------------------------------------------//


#ifndef CONSOLEDRIVER_H
#define CONSOLEDRIVER_H
#include "copyright.h"
#include "utility.h"
#include "console.h"
#include "system.h"
class ConsoleDriver:dontcopythis {
public:
// initialize the hardware console device
ConsoleDriver(const char *readFile, const char *writeFile);
~ConsoleDriver(); // clean up
void PutChar(int ch); // Behaves like putchar(3S)
int GetChar(); // Behaves like getchar(3S)
void PutString(const char *s); // Behaves like fputs(3S)
void PutInt(int n);
void GetString(char *s, int n); // Behaves like fgets(3S)
void GetInt(int* n);

private:
Console *console;
};
#endif // CONSOLEDRIVER_H
#endif // CHANGED
