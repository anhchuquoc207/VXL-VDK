#include "main.h"
#include "sched.h"

// Task array with a maximum size defined
struct_Task SCH_tasks_G[SCH_MAX_TASKS];
//unsigned char Error_code_G;

void SCH_Init(void) {
    unsigned char i;

    // Loop through the task array and delete each task
    for (i = 0; i < SCH_MAX_TASKS; i++) {
        SCH_Delete_Task(i);
    }

    // Reset the global error variable
    // SCH_Delete_Task() will generate an error code if the task array is empty
    //Error_code_G = 0;

    // Initialize the timer and watchdog
    //Timer_Init();
    //Watchdog_Init();
}

void SCH_Update(void) {
    unsigned char Index;

    // NOTE: Calculations are in *TICKS* (not milliseconds)
    for (Index = 0; Index < SCH_MAX_TASKS; Index++) {
        // Check if there is a task at this location
        if (SCH_tasks_G[Index].pTask) {
            if (SCH_tasks_G[Index].Delay == 0) {
                // The task is due to run
                // Increment the 'RunMe' flag
                SCH_tasks_G[Index].RunMe = 1;

                if (SCH_tasks_G[Index].Period) {
                    // Schedule periodic tasks to run again
                    SCH_tasks_G[Index].Delay = SCH_tasks_G[Index].Period;
                }
            } else {
                // Not yet ready to run, just decrement the delay
                SCH_tasks_G[Index].Delay--;
            }
        }
    }
}


void SCH_Add_Task(void (* pFunction)(), unsigned int DELAY, unsigned int PERIOD) {
    unsigned char Index = 0;

    while ((SCH_tasks_G[Index].pTask != 0) && (Index < SCH_MAX_TASKS)) {
        Index++;
    }

    if (Index == SCH_MAX_TASKS) {

        return;
    }

    SCH_tasks_G[Index].pTask = pFunction;
    SCH_tasks_G[Index].Delay = DELAY / 10;
    SCH_tasks_G[Index].Period = PERIOD / 10;
    SCH_tasks_G[Index].RunMe = 0;

}

void SCH_Dispatch_Tasks(void) {
    unsigned char Index;

    for (Index = 0; Index < SCH_MAX_TASKS; Index++) {
        if (SCH_tasks_G[Index].RunMe > 0) {
            (*SCH_tasks_G[Index].pTask)();
            SCH_tasks_G[Index].RunMe = 0 ;

            if (SCH_tasks_G[Index].Period == 0) {
                SCH_Delete_Task(Index);
            }
        }
    }
}

void SCH_Delete_Task(const uint8_t TASK_INDEX) {

    // Check if there is no task at the specified index
    if (SCH_tasks_G[TASK_INDEX].pTask == 0) {
        // No task at this location, set the global error variable
        //Error_code_G = ERROR_SCH_CANNOT_DELETE_TASK;

        // Also return an error code
        //Return_code = RETURN_ERROR;
    } else {
        // Task found and marked for deletion
        //Return_code = RETURN_NORMAL;
    }

    SCH_tasks_G[TASK_INDEX].pTask = 0x0000;
    SCH_tasks_G[TASK_INDEX].Delay = 0;
    SCH_tasks_G[TASK_INDEX].Period = 0;
    SCH_tasks_G[TASK_INDEX].RunMe = 0;

}

void SCH_Go_To_Sleep(){

}

unsigned char Last_error_code_G = 0;
unsigned int Error_tick_count_G = 0;
unsigned char Error_port;

void SCH_Report_Status(void) {
    #ifdef SCH_REPORT_ERRORS
    // ONLY APPLIES IF WE ARE REPORTING ERRORS

    // Check for a new error code
    if (Error_code_G != Last_error_code_G) {
        // Negative logic on LEDs assumed
        Error_port = 255 - Error_code_G;
        Last_error_code_G = Error_code_G;

        if (Error_code_G != 0) {
            Error_tick_count_G = 60000; // Start error timer
        } else {
            Error_tick_count_G = 0; // No error, reset the timer
        }
    } else {
        // No new error, handle tick count
        if (Error_tick_count_G != 0) {
            if (--Error_tick_count_G == 0) {
                Error_code_G = 0; // Reset error code after timer expires
            }
        }
    }
    #endif
}
