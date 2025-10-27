#include "sched.h"

// --- Global Variables ---
uint8_t a_taskID[SCH_MAX_TASKS];
struct_Task* p_Head;

/**
  * @brief  Finds the first available ID in the task array.
  * @retval The available ID (0 to SCH_MAX_TASKS-1) or -1 (255) if no ID is free.
  */
uint8_t get_available_ID() {
    for (uint8_t i = 0; i < SCH_MAX_TASKS; i++) {
        if (a_taskID[i] == 0) {
        	a_taskID[i] = 1; // Mark as used
            return i;
        }
    }
    return -1; // No available IDs
}

/**
  * @brief  Creates a new task node.
  * @param  pFunction: Pointer to the task function.
  * @param  DELAY: Delay in milliseconds.
  * @param  PERIOD: Period in milliseconds.
  * @param  ID: The Task ID.
  * @retval Pointer to the new task, or NULL if malloc fails.
  */
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

/**
  * @brief  Initializes the scheduler.
  * @retval None
  */
void SCH_Init(void) {
	unsigned char i;

	for (i = 0; i < SCH_MAX_TASKS; i++){
		a_taskID[i] = 0; // Mark all IDs as free
	}
    p_Head = NULL; // Set the task list to empty
}

/**
  * @brief  The scheduler update function. Called every 10ms tick.
  * @note   This is an O(1) operation.
  * @retval None
  */
void SCH_Update(void) {
    // Only check and decrement the head of the list
    if (p_Head && p_Head->pTask){
        if (p_Head->Delay > 0){
        	p_Head->Delay--;
        }
    }
}

/**
  * @brief  Adds a task to the scheduler's sorted list.
  * @note   This is an O(n) operation (sorted insertion).
  * @param  pFunction: Pointer to the task function.
  * @param  DELAY: Delay in milliseconds.
  * @param  PERIOD: Period in milliseconds.
  * @retval None
  */
void SCH_Add_Task(void (* pFunction)(), unsigned int DELAY, unsigned int PERIOD) {
	uint8_t ID = get_available_ID();

	// Return if task list is full
	if (ID == 255) return;

    struct_Task* newTask = create_newTask(pFunction, DELAY, PERIOD, ID);

    // Return if malloc failed
    if (!newTask) return;

    //Sorted Insertion Logic

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
            current->Delay -= newTask->Delay;
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

/**
  * @brief  Deletes a task from the head of the list and frees its memory.
  * @param  head: Pointer to the head of the list.
  * @retval Pointer to the new head of the list.
  */
struct_Task* SCH_Delete_Task(struct_Task* head) {
    if (head == NULL) return NULL;

    struct_Task* temp = head;
    head = head->p_next; // Move head to the next node
    free(temp);          // Free the old head

    return head; // Return the new head
}

/**
  * @brief  Runs any tasks that are due.
  * @note   This is an O(1) operation (worst case O(k) if 'k' tasks are due on the same tick).
  * @retval None
  */
void SCH_Dispatch_Tasks(void) {
    // Use a 'while' loop to run ALL tasks that are due (Delay == 0)
    while (p_Head && p_Head->Delay == 0) {

        // 1. Store task info *before* deleting
        void (*pTask_temp)() = p_Head->pTask;
        unsigned int Period_temp = p_Head->Period;

        // 2. Free its ID
    	a_taskID[p_Head->TaskID] = 0;

        // 3. Run the task
    	(*pTask_temp)();

        // 4. Delete the task (this advances p_Head to the next task)
    	p_Head = SCH_Delete_Task(p_Head);

        // 5. Re-add the task if it was periodic
        // This will call SCH_Add_Task() to re-insert it in the correct sorted position.
        if (Period_temp > 0) {
            SCH_Add_Task(pTask_temp, Period_temp, Period_temp);
        }
    }
}
