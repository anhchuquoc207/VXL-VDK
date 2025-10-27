/*
 * sched.c
 *
 *  Created on: Nov 10, 2024
 *      Author: admin
 */

#include "sched.h"
#include "main.h"
// Task array with a maximum size defined
//struct_Task SCH_tasks_G[SCH_MAX_TASKS];
uint8_t a_taskID[SCH_MAX_TASKS];
struct_Task* p_Head;
struct_Task* p_Update;

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
		newTask->Delay = DELAY / 10;
		newTask->Period = PERIOD;
		newTask->RunMe = 0;
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
    p_Head = NULL;
    p_Update = NULL;

}

void SCH_Update(void) {
    if (p_Update && p_Update->pTask){
        if (p_Update->Delay <= 0){
            p_Update->RunMe += 1;
        }
        else {
        	p_Update->Delay--;
        }
    }
}

void SCH_Add_Task(void (* pFunction)(), unsigned int DELAY, unsigned int PERIOD) {
	uint8_t ID = get_available_ID();
    	struct_Task* newTask = create_newTask(pFunction, DELAY, PERIOD, ID);
    	if (!newTask) return;

    	if (p_Head == NULL){
    		p_Head = newTask;
    		p_Update = newTask;
    	}
    	else {
    		struct_Task* current = p_Head;
    		struct_Task* prev = NULL;

    		while (current && newTask->Delay >= current->Delay){
    			newTask->Delay -= current->Delay;
    			prev = current;
    			current = current->p_next;
    		}

    		if (prev == NULL){
    			newTask->p_next = p_Head;
    			p_Head = newTask;
    			current->Delay -= newTask->Delay;
    		}
    		else if (current == NULL){
    			prev->p_next = newTask;
    		}
    		else {
    			newTask->p_next = current;
    			prev->p_next = newTask;
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
    if (p_Update && p_Update->RunMe > 0){
    	(*p_Update->pTask)();
    	//HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
    	a_taskID[p_Update->TaskID] = 0;
    	if(p_Update->Period > 0) SCH_Add_Task(p_Update->pTask, p_Update->Period, p_Update->Period);
    	p_Head = SCH_Delete_Task(p_Head);
    	p_Update = p_Head;

    }
}









