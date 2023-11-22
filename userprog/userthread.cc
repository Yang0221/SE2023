#ifdef CHANGED

//----------------------------------------------------------------------//
//                       Université de Bordeaux                         //
//                             BECHER IHEB                              //
//                               YANG YI                                //
//----------------------------------------------------------------------//


#include "system.h"
#include "userthread.h"

#include "synch.h"
#include "syscall.h"
#include <unistd.h>



struct Serialisation{
	int my_function , arg;
    //Semaphore * access ;
};

static int nbthread = 0 ;
static Semaphore * mutexThread = new Semaphore("mutex for thread", 1);


static void StartUserThread(void* schmurtz)
{
   
   for (int i = 0; i < NumTotalRegs; i++)
    {
        machine->WriteRegister (i, 0); //clean the registers
    }
    
    machine->WriteRegister (PCReg, currentThread->getF()); //
    DEBUG('x', "pcreg : %d\n", machine->ReadRegister(PCReg));

    machine->WriteRegister (NextPCReg, machine->ReadRegister(PCReg) + 4);
    DEBUG('x', "nextpcreg : %d\n", machine->ReadRegister(NextPCReg));
 
    machine->WriteRegister(4, currentThread->getArg());
    DEBUG('x', " arg : %d\n", machine->ReadRegister(4));
  
    mutexThread->P();
    int stack = currentThread->space->AllocateUserStack();
    mutexThread->V();
   
    machine->WriteRegister (RetAddrReg, currentThread->getExitCode() );
    DEBUG ('x', "RetAddrReg %d \n", machine->ReadRegister(31));
   
    machine->WriteRegister (StackReg, stack );
    DEBUG ('x', "Initializing stack register to 0x%x\n", stack);

    //delete myfunc ;
    //printf("je suis là\n");
    machine->Run();    
}


/**
 * This function is called when the SC ThreadCreate is called. and it creat a 
 * new threa with thre function adress and argument passed in parametre 
 */
int do_ThreadCreate(int f, int arg,int exitCode)
{

    mutexThread->P(); //One thread creation at a time   
    if(nbthread>=MAX_THREAD_NUMBER){
        mutexThread->V();
        return -1 ;
    }        
    Thread *t = new Thread("mythred"); //create a new thread
    if(t==NULL ){
        mutexThread->V();
        return -1 ;
    }
    nbthread ++ ; 
    t->setFArgExitCode(f,arg,exitCode); 
    mutexThread->V();
    
    t->space = currentThread->space ;
    t->Start(StartUserThread, NULL);
    currentThread->Yield();
    return 0 ;
}

void do_ThreadExit()
{
    mutexThread->P();
    if(nbthread > 0) 
    {
        currentThread->space->DeallocateStack(); //deallocate thread memory 
        nbthread--;
        mutexThread->V();
    }
    else 
    {
        mutexThread->V();
        //currentThread->Finish();
        interrupt->Powerdown();
    }
    //currentThread->Yield();
    currentThread->Finish();
    
}
#endif // CHANGED