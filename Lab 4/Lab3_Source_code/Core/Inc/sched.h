#ifndef INC_SCHED_H_
#define INC_SCHED_H_

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct struct_Task{
    void (*pTask)(void);
    uint32_t Delay;
    uint32_t Period;
    uint8_t RunMe;
    uint32_t TaskID;
    struct struct_Task* p_next;
} struct_Task;

#define SCH_MAX_TASKS 40


void SCH_Init(void);
void SCH_Update(void);
void SCH_Add_Task(void (* pFunction)(), unsigned int DELAY, unsigned int PERIOD);
void SCH_Dispatch_Tasks(void);
struct_Task* SCH_Delete_Task(struct_Task* head);
struct_Task* create_newTask(void (*pFunction)(), unsigned int DELAY, unsigned int PERIOD, unsigned int ID);
uint8_t get_available_ID(void);



#endif /* INC_SCHED_H_ */
