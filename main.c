/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
* This example project uses cooperative preemptive scheduling. The RTX_Conf_CM.c
* file has "OS_ROBIN 0". The threads will have the same priority but they
* will use a semaphore to switch context.
* functions used: osSemaphoreWait, osSemaphoreRelease, osThreadYield
* Needed to increase stack size so that the printf would work correctly
* (no stack overflow).
* RTX_Conf_CM.c Default thread and Main thread stack size set to
* 2000.
* startup_stm32f407xx.s (Startup) change stack to 2000 (no change)
* Needed to include Run Time Environment->Compiler->I/O->STDOUT (select ITM)
* for printf.
 changed #define PLL_M      8 in system_stm32f4xx.c
* This works! 
look at apnt_230.pdf
Follow step #5 (PLL_M), #14 Configure SWV
Make sure 168MHz set Xtal and Core Clock for debug trace

changed RTOS kernal timer clock freq. to 168 MHz
*---------------------------------------------------------------------------*/


#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "cmsis_os.h"
#include "stdio.h"
#include "stm32f4xx.h"



void Thread_1 (void const *arg);                           // function prototype for Thread_1
osThreadDef (Thread_1, osPriorityNormal, 1, 0);            // define Thread_1

void Thread_2 (void const *arg);                           // function prototype for Thread_2
osThreadDef (Thread_2, osPriorityNormal, 1, 0);            // define Thread_2

// This is the semaphore
osSemaphoreId sid_Thread_Semaphore;                             // semaphore id
osSemaphoreDef (SampleSemaphore);                               // semaphore object


int main (void) {
	osThreadId id;
  osKernelInitialize ();                    // initialize CMSIS-RTOS

printf("test\n");
	
	// create the semaphore
	// start with a count of 1
	sid_Thread_Semaphore = osSemaphoreCreate (osSemaphore(SampleSemaphore), 1);
  if (!sid_Thread_Semaphore) {
    ; // Semaphore object not created, handle failure
  }
  
  id = osThreadCreate (osThread (Thread_1), NULL);         // create the thread
  if (id == NULL) {                                        // handle thread creation
    // Failed to create a thread
  };

  id = osThreadCreate (osThread (Thread_2), NULL);         // create the thread
  if (id == NULL) {                                        // handle thread creation
    // Failed to create a thread
  };
	
  osKernelStart ();                         // start thread execution 
}


void Thread_1(void const *arg){
	int i=0;
	while(1){
		osSemaphoreWait (sid_Thread_Semaphore, osWaitForever);
		i = i+1;
		printf("Thread_1\n");
	}
}

void Thread_2(void const *arg){
	int i=0;
	while(1){
		osSemaphoreRelease(sid_Thread_Semaphore);
		i = i+1;
		printf("Thread_2\n");
		osThreadYield();
	}
}
