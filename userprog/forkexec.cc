#ifdef CHANGED
//----------------------------------------------------------------------//
//                       Université de Bordeaux                         //
//                             BECHER IHEB                              //
//                               YANG YI                                //
//----------------------------------------------------------------------//
#include "forkexec.h"
#include "thread.h"
#include "addrspace.h"
#include "synch.h"
#include "system.h"
#include "console.h"




static void StartProcess(void* schmurtz){
    currentThread->space->InitRegisters();
    currentThread->space->RestoreState();    
    machine->Run();
    currentThread->Yield();
}
int do_ForkExec(const char * s ){
    OpenFile *  executable = fileSystem->Open(s) ;
    AddrSpace * processAddressSpace = new AddrSpace(executable) ;
    
    Thread * t = new Thread(s);
    currentThread->child->Append(t);
    t->space=processAddressSpace ;
    
    t->Start(StartProcess, NULL);
    machine->processCounter++ ; 
    
    delete executable ;
    currentThread->Yield();
    return 0 ;
}

#endif