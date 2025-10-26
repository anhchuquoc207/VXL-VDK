#ifndef INC_SCHED_H_
#define INC_SCHED_H_

typedef struct {
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
} struct_Task;

// Must be adjusted for each new project
#define SCH_MAX_TASKS 40
#define NO_TASK_ID 0



void SCH_Init(void);
void SCH_Update(void);
void SCH_Add_Task(void (* pFunction)(), unsigned int DELAY, unsigned int PERIOD);
void SCH_Dispatch_Tasks(void);
void SCH_Delete_Task(const uint8_t TASK_INDEX);
void SCH_Go_To_Sleep();
void SCH_Report_Status(void);



#endif /* INC_SCHED_H_ */
