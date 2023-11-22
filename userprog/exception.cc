// exception.cc 
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.  
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "userCode.h"
#include "userthread.h"
#include "forkexec.h"
#include "synch.h"

static Semaphore *mutex_process= new Semaphore("mutex process", 1);

//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC ()
{
    int pc = machine->ReadRegister (PCReg);
    machine->WriteRegister (PrevPCReg, pc);
    pc = machine->ReadRegister (NextPCReg);
    machine->WriteRegister (PCReg, pc);
    pc += 4;
    machine->WriteRegister (NextPCReg, pc);
}


//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions 
//      are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler (ExceptionType which)
{
    int type = machine->ReadRegister (2);
    int address = machine->registers[BadVAddrReg];
	//printf("r1 : %d|r2:%d|r3:%d|r4:%d|r5%d",machine->ReadRegister (1),machine->ReadRegister (2),machine->ReadRegister (3),machine->ReadRegister (4),machine->ReadRegister (5));

    switch (which)
      {
	case SyscallException:
          {
	    switch (type)
	      {
		case SC_Halt:
		  {
			mutex_process->P();
		    DEBUG ('s', "Shutdown, initiated by user program.\n");
			if( machine->processCounter>1){
				machine->processCounter--;
				mutex_process->V();
				printf("affdsds\n");
				currentThread-> Finish ();
				currentThread->Yield();
			}
		    interrupt->Powerdown ();
		    break;
		  }
		#ifdef CHANGED	
		case SC_Exit:
		{
						
			int code = machine->ReadRegister(4);
			DEBUG('s',"Exit Called with paramètre (%d) \n",code);
			if(code==-1){
				currentThread->finishAllChild();
				machine->processCounter--;
			}
			bool  valid = true ;
			while (valid)
			{
				mutex_process->P();
			if( machine->processCounter > 1){
				machine->processCounter--;
				mutex_process->V();
				}else{
				interrupt->Powerdown();
			}
				currentThread->Finish();
			}
			break;
		}	
		case SC_PutChar:
		{
			DEBUG('s',"PutChar\n");
			consoledriver->PutChar(machine->ReadRegister(4));
			break;
		}
		case SC_PutString :
		{
			DEBUG('s',"PutString\n");
			char buff[MAX_STRING_SIZE]={'\0'};

			int c = machine->ReadRegister(4);
			int res=0;
			int from=c ;
			do
			{
				buff[0]='\0';
				res = copyStringFromMachine(from, buff, MAX_STRING_SIZE);
				consoledriver->PutString(buff);
				from += res ;
			}while(res <= (MAX_STRING_SIZE) && res>0);
			break;
		}
		case SC_GetChar:
		{
			DEBUG('s',"GetChar\n");
			machine->WriteRegister(2, consoledriver->GetChar());
			DEBUG('s',"GetChar : %c\n",machine->ReadRegister(2));
			break;
		}
		case SC_GetString :
		{
            DEBUG('s',"GetString\n");
			int max_n = 1024; 
			int to = machine->ReadRegister(4);
			int n = machine->ReadRegister(5);
			if(n>max_n)
			{
				n=max_n;
			}
			char from[n];

			consoledriver->GetString(from, n);
			int i=copyStringToMachine(from, to, n);
			DEBUG('s',"i=%d\n",i);

			//machine->WriteRegister(2, to);
			break;
		}
		case SC_PutInt :
		{
			DEBUG('s',"PutInt\n");

			consoledriver->PutInt(machine->ReadRegister(4));
			break;
		}
		case SC_GetInt :
		{
			DEBUG('s',"GetInt\n");
			int * n= new int ;
			* n = machine->ReadRegister(4);
			consoledriver->GetInt( n );
			delete n ;
			break;
		}
			case SC_ThreadCreate:
		{
			DEBUG('s',"ThreadCreate\n");
			int f = machine->ReadRegister(4);
			int arg = machine->ReadRegister(5);
			int exitCode = machine->ReadRegister(6);
			do_ThreadCreate(f,arg,exitCode);
			break;
		}
		case SC_ThreadExit:
		{
			DEBUG('s',"ThreadExit\n");
			
			do_ThreadExit();

			break;
		}
		case SC_ForkExec :
		{
			DEBUG('s',"PutString\n");
			char buff[MAX_STRING_SIZE]={'\0'};

			int c = machine->ReadRegister(4);
			int from=c ;
			buff[0]='\0';
			copyStringFromMachine(from, buff, MAX_STRING_SIZE);
			do_ForkExec(buff);
			break ;
		}

		#endif //CHANGED
		default:
		  {
		    printf("Unimplemented system call %d\n", type);
		    ASSERT(FALSE);
		  }
	      }

	    // Do not forget to increment the pc before returning!
	    UpdatePC ();
	    break;
	  }

	case PageFaultException:
	  if (!address) {
	    printf("NULL dereference at PC %x!\n", machine->registers[PCReg]);
	    ASSERT (FALSE);
	  } else {
	    printf ("Page Fault at address %x at PC %x\n", address, machine->registers[PCReg]);
	    ASSERT (FALSE);	// For now
	  }
	  break;

	case ReadOnlyException:
	  printf ("Read-Only at address %x at PC %x\n", address, machine->registers[PCReg]);
	  ASSERT (FALSE);	// For now
	  break;

	case BusErrorException:
	  printf ("Invalid physical address at address %x at PC %x\n", address, machine->registers[PCReg]);
	  ASSERT (FALSE);	// For now
	  break;

	case AddressErrorException:
	  printf ("Invalid address %x at PC %x\n", address, machine->registers[PCReg]);
	  ASSERT (FALSE);	// For now
	  break;

	case OverflowException:
	  printf ("Overflow at PC %x\n", machine->registers[PCReg]);
	  ASSERT (FALSE);	// For now
	  break;

	case IllegalInstrException:
	  printf ("Illegal instruction at PC %x\n", machine->registers[PCReg]);
	  ASSERT (FALSE);	// For now
	  break;

	default:
	  printf ("Unexpected user mode exception %d %d %x at PC %x\n", which, type, address, machine->registers[PCReg]);
	  ASSERT (FALSE);
	  break;
      }
}
