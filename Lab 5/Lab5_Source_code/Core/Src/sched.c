#include "sched.h"
#include "main.h"

// --- Global Variables ---
uint8_t a_taskID[SCH_MAX_TASKS];
struct_Task* p_Head;


uint8_t get_available_ID() {
    for (uint8_t i = 0; i < SCH_MAX_TASKS; i++) {
        if (a_taskID[i] == 0) {
        	a_taskID[i] = 1; // Mark as used
            return i;
        }
    }
    return -1; // No available IDs
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
		a_taskID[i] = 0; // Mark all IDs as free
	}
    p_Head = NULL; // Set the task list to empty
}


void SCH_Update(void) {
    // Only check and decrement the head of the list
    if (p_Head && p_Head->pTask){
        if (p_Head->Delay > 0){
        	p_Head->Delay--;
        }
    }
}


void SCH_Add_Task(void (* pFunction)(), unsigned int DELAY, unsigned int PERIOD) {
	uint8_t ID = get_available_ID();

	// Return if task list is full
	if (ID == 255) return;

    struct_Task* newTask = create_newTask(pFunction, DELAY, PERIOD, ID);

    // Return if malloc failed
    if (!newTask) return;

    // --- Sorted Insertion Logic ---

    // If the list is empty, just make this the head
    if (p_Head == NULL){
        p_Head = newTask;
    }
    // If the list is not empty, find the correct spot
    else {
        struct_Task* current = p_Head;
        struct_Task* prev = NULL;

        // 1. Walk the list and subtract delays
        while (current && newTask->Delay >= current->Delay){
            newTask->Delay -= current->Delay;
            prev = current;
            current = current->p_next;
        }

        // 2. Insert the task

        // Case A: Insert at the head
        if (prev == NULL){
            newTask->p_next = p_Head;
            p_Head = newTask;
            // Update the delay of the *next* task
            if (current) {
                current->Delay -= newTask->Delay;
            }
        }
        // Case B: Insert at the end
        else if (current == NULL){
            prev->p_next = newTask;
        }
        // Case C: Insert in the middle
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
    head = head->p_next; // Move head to the next node
    free(temp);          // Free the old head

    return head; // Return the new head
}


void SCH_Dispatch_Tasks(void) {
    // Use a 'while' loop to run ALL tasks that are due (Delay == 0)
    while (p_Head && p_Head->Delay == 0) {

        // 1. Store task info *before* deleting
        void (*pTask_temp)() = p_Head->pTask;
        unsigned int Period_temp = p_Head->Period;
        uint8_t ID_temp = p_Head->TaskID;

        // 2. Run the task
    	(*pTask_temp)();

        // 3. Delete the task (this advances p_Head to the next task)
    	p_Head = SCH_Delete_Task(p_Head);

        // 4. Re-add the task if it was periodic
        if (Period_temp > 0) {
            SCH_Add_Task(pTask_temp, Period_temp, Period_temp);
        } else {
        	// If it was a one-shot task, just free its ID
        	a_taskID[ID_temp] = 0;
        }
    }
}
