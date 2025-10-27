#include "sched.h"
uint8_t a_taskID[SCH_MAX_TASKS];
struct_Task* p_Head;

uint8_t get_available_ID() {
    for (uint8_t i = 0; i < SCH_MAX_TASKS; i++) {
        if (a_taskID[i] == 0) {
        	a_taskID[i] = 1;
            return i;
        }
    }
    return -1;
}

struct_Task* create_newTask(void (*pFunction)(), unsigned int DELAY, unsigned int PERIOD, unsigned int ID){
	struct_Task* newTask = (struct_Task*)malloc(sizeof(struct_Task));
	if (newTask){
		newTask->pTask = pFunction;
		// Convert delay from ms to scheduler ticks (10ms)
		newTask->Delay = DELAY / 10;
		newTask->Period = PERIOD;
		newTask->TaskID = ID;
		newTask->p_next = NULL;
	}
	return newTask;
}

void SCH_Init(void) {
	unsigned char i;

	for (i = 0; i < SCH_MAX_TASKS; i++){
		a_taskID[i] = 0;
	}
    p_Head = NULL; // Set list to empty
}

void SCH_Update(void) {
    if (p_Head && p_Head->pTask){
        if (p_Head->Delay > 0){
        	p_Head->Delay--;
        }
    }
}

void SCH_Add_Task(void (* pFunction)(), unsigned int DELAY, unsigned int PERIOD) {
	uint8_t ID = get_available_ID();

	if (ID == 255) return;

    struct_Task* newTask = create_newTask(pFunction, DELAY, PERIOD, ID);

    if (!newTask) return;

    if (p_Head == NULL){
        p_Head = newTask;
    }
    else {
        struct_Task* current = p_Head;
        struct_Task* prev = NULL;

        while (current && newTask->Delay >= current->Delay){
            newTask->Delay -= current->Delay;
            prev = current;
            current = current->p_next;
        }
        // Case A: Insert at head
        if (prev == NULL){
            newTask->p_next = p_Head;
            p_Head = newTask;
            // Update the delay of the *next* task
            current->Delay -= newTask->Delay;
        }
        // Case B: Insert at end
        else if (current == NULL){
            prev->p_next = newTask;
        }
        // Case C: Insert middle
        else {
            newTask->p_next = current;
            prev->p_next = newTask;
            // Update the delay of the *next* task
            current->Delay -= newTask->Delay;
        }
    }
}

struct_Task* SCH_Delete_Task(struct_Task* head) {
    if (head == NULL) return NULL;

    struct_Task* temp = head;
    head = head->p_next;
    free(temp);

    return head;
}

void SCH_Dispatch_Tasks(void) {
    while (p_Head && p_Head->Delay == 0) {

        void (*pTask_temp)() = p_Head->pTask;
        unsigned int Period_temp = p_Head->Period;

    	a_taskID[p_Head->TaskID] = 0;

    	(*pTask_temp)();

    	p_Head = SCH_Delete_Task(p_Head);

        if (Period_temp > 0) {
            SCH_Add_Task(pTask_temp, Period_temp, Period_temp);
        }
    }
}
