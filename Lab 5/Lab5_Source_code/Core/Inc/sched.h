/*
 * sched.h
 *
 *  Created on: Nov 10, 2024
 *      Author: admin
 */

#ifndef INC_SCHED_H_
#define INC_SCHED_H_

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct struct_Task{
    // Pointer to the task (must be a 'void(void)' function)
    void (*pTask)(void);

    // Delay (ticks) until the function will (next) be run
    uint32_t Delay;

    // Interval (ticks) between subsequent runs
    uint32_t Period;

    // Incremented (by scheduler) when task is due to execute
    uint8_t RunMe;

    // This is a hint to solve the question below
    uint32_t TaskID;
    struct struct_Task* p_next;
} struct_Task;



// Must be adjusted for each new project
#define SCH_MAX_TASKS 40
#define NO_TASK_ID 0


void SCH_Init(void);
void SCH_Update(void);
void SCH_Add_Task(void (* pFunction)(), unsigned int DELAY, unsigned int PERIOD);
void SCH_Dispatch_Tasks(void);
struct_Task* SCH_Delete_Task(struct_Task* head);
struct_Task* create_newTask(void (*pFunction)(), unsigned int DELAY, unsigned int PERIOD, unsigned int ID);
uint8_t get_available_ID(void);



#endif /* INC_SCHED_H_ */
