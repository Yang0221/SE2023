// synch.cc 
//      Routines for synchronizing threads.  Three kinds of
//      synchronization routines are defined here: semaphores, locks 
//      and condition variables (the implementation of the last two
//      are left to the reader).
//
// Any implementation of a synchronization routine needs some
// primitive atomic operation.  We assume Nachos is running on
// a uniprocessor, and thus atomicity can be provided by
// turning off interrupts.  While interrupts are disabled, no
// context switch can occur, and thus the current thread is guaranteed
// to hold the CPU throughout, until interrupts are reenabled.
//
// Because some of these routines might be called with interrupts
// already disabled (Semaphore::V for one), instead of turning
// on interrupts at the end of the atomic operation, we always simply
// re-set the interrupt state back to its original value (whether
// that be disabled or enabled).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "synch.h"
#include "system.h"

//----------------------------------------------------------------------
// Semaphore::Semaphore
//      Initialize a semaphore, so that it can be used for synchronization.
//
//      "debugName" is an arbitrary name, useful for debugging.
//      "initialValue" is the initial value of the semaphore.
//----------------------------------------------------------------------

Semaphore::Semaphore (const char *debugName, int initialValue)
{
    name = debugName;
    value = initialValue;
    queue = new List;
}

//----------------------------------------------------------------------
// Semaphore::Semaphore
//      De-allocate semaphore, when no longer needed.  Assume no one
//      is still waiting on the semaphore!
//----------------------------------------------------------------------

Semaphore::~Semaphore ()
{
    delete queue;
    queue = NULL;
    value = -1;
}

//----------------------------------------------------------------------
// Semaphore::P
//      Wait until semaphore value > 0, then decrement.  Checking the
//      value and decrementing must be done atomically, so we
//      need to disable interrupts before checking the value.
//
//      Note that Thread::Sleep assumes that interrupts are disabled
//      when it is called.
//----------------------------------------------------------------------

void
Semaphore::P ()
{
    IntStatus oldLevel = interrupt->SetLevel (IntOff);	// disable interrupts

    ASSERT(value >= 0);

    while (value == 0)
      {				// semaphore not available
	  queue->Append ((void *) currentThread);	// so go to sleep
	  currentThread->Sleep ();
      }
    value--;			// semaphore available, 
    // consume its value

    (void) interrupt->SetLevel (oldLevel);	// re-enable interrupts
}

//----------------------------------------------------------------------
// Semaphore::V
//      Increment semaphore value, waking up a waiter if necessary.
//      As with P(), this operation must be atomic, so we need to disable
//      interrupts.  Scheduler::ReadyToRun() assumes that threads
//      are disabled when it is called.
//----------------------------------------------------------------------

void
Semaphore::V ()
{
    Thread *thread;
    IntStatus oldLevel = interrupt->SetLevel (IntOff);

    ASSERT(value >= 0);

    thread = (Thread *) queue->Remove ();
    if (thread != NULL)		// make thread ready, consuming the V immediately
	scheduler->ReadyToRun (thread);
    value++;
    (void) interrupt->SetLevel (oldLevel);
}

// Dummy functions -- so we can compile our later assignments 
// Note -- without a correct implementation of Condition::Wait(), 
// the test case in the network assignment won't work!
#ifdef 
Lock::Lock (const char *debugName)
{
    (void) debugName;
    /* TODO */
    name = debugName;
    owner = NULL;
    queue = new List;
    //ASSERT(FALSE);
}

Lock::~Lock ()
{   
    delete queue;
}
void
Lock::Acquire ()
{
    /* TODO */
    //ASSERT(FALSE);
    IntStatus oldLevel = interrupt->SetLevel (IntOff);	// disable interrupts

    while (owner != NULL)
      {				
      queue->Append ((void *) currentThread);	// so go to sleep
      currentThread->Sleep ();
      }

    owner = currentThread;

    (void) interrupt->SetLevel (oldLevel) ;

}
bool 
Lock::isHeldByCurrentThread() 
{
    return owner == currentThread ;
}
void
Lock::Release ()
{
    /* TODO */
    //ASSERT(FALSE);
    ASSERT(isHeldByCurrentThread());
    
    Thread *thread;
    IntStatus oldLevel = interrupt->SetLevel (IntOff);

    owner = NULL;

    thread = (Thread *) queue->Remove ();
    if (thread != NULL)		
	scheduler->ReadyToRun (thread);
    
    (void) interrupt->SetLevel (oldLevel);
}

Condition::Condition (const char *debugName)
{
    (void) debugName;
    /* TODO */
    //ASSERT(FALSE);
    name = debugName;
    mutex = NULL;
    condqueue = new List;
}

Condition::~Condition ()
{
    delete condqueue;
}
void
Condition::Wait (Lock * conditionLock)
{
    (void) conditionLock;
    /* TODO */
    ASSERT(conditionLock->isHeldByCurrentThread());
    Semaphore * wait = new Semaphore(name, 0);
    condqueue->Append(wait); // list of semaphore that will be in charge of waiting on the condition
    conditionLock->Release(); //release the lock
    wait->P(); // sleep on  and wake up to a signal or a broadcast
    conditionLock->Acquire();  // take lock after waking up
    delete wait;
}

void
Condition::Signal (Lock * conditionLock)
{
    (void) conditionLock;
    /* TODO */
    //ASSERT(FALSE);
    ASSERT(conditionLock->isHeldByCurrentThread());
    Semaphore *  nextThreadSem ;

    if(!condqueue->IsEmpty()){
        nextThreadSem = (Semaphore * ) condqueue->Remove() ;
        nextThreadSem->V() ; 
    }

}


void
Condition::Broadcast (Lock * conditionLock)
{
    (void) conditionLock;
    /* TODO */
    //ASSERT(FALSE);
    while(!condqueue->IsEmpty())
    {
        Signal(conditionLock);
    }
}
#endif // 