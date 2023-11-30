/*
 * scheduler.c
 *
 *  Created on: Nov 29, 2023
 *      Author: Acer
 */
#include "main.h"
#include "scheduler.h"

void SCH_Init() {
	SCH_TASK_LIST.numTask = 0;
	for(int index = 0; index < SCH_MAX_TASKS; index++) {
		SCH_TASK_LIST.SCH_TASK_G[index].Delay = 0;
		SCH_TASK_LIST.SCH_TASK_G[index].Period = 0;
		SCH_TASK_LIST.SCH_TASK_G[index].RunMe = 0;
		SCH_TASK_LIST.SCH_TASK_G[index].pTask = 0;
	}
}

void insert_to_list(sTask task) {
	
	if (SCH_TASK_LIST.numTask == 0) {
		SCH_TASK_LIST.SCH_TASK_G[0] = task;
		SCH_TASK_LIST.numTask += 1;
		return;
	} else {
		int b = 0;
		int c = SCH_TASK_LIST.SCH_TASK_G[0].Delay; 
		int d = SCH_TASK_LIST.SCH_TASK_G[0].Delay;  
		
		while (b < SCH_TASK_LIST.numTask && task.Delay > d) {
			b++;
			if (b < SCH_TASK_LIST.numTask) {
				c = d;
				d += SCH_TASK_LIST.SCH_TASK_G[b].Delay;
			}
		}
	
		if (b == SCH_TASK_LIST.numTask) {
			task.Delay -= d;
			SCH_TASK_LIST.SCH_TASK_G[b] = task;
			SCH_TASK_LIST.numTask += 1;
			return;
		}
		
		else {
			for (int i = SCH_TASK_LIST.numTask; i > b; i--) {
				SCH_TASK_LIST.SCH_TASK_G[i] = SCH_TASK_LIST.SCH_TASK_G[i - 1];
			}
		
			if (b == 0) {
				SCH_TASK_LIST.SCH_TASK_G[b] = task;
				d = 0;
				c = task.Delay;
			}
			
			else {
				task.Delay -= c;
				d = c;
				c += task.Delay;
				SCH_TASK_LIST.SCH_TASK_G[b] = task;
			}
			
			d += SCH_TASK_LIST.SCH_TASK_G[b + 1].Delay;
			SCH_TASK_LIST.SCH_TASK_G[b + 1].Delay = d - c;
			SCH_TASK_LIST.numTask += 1;
		}
	}
}
void SCH_Add_Task(void (*pFunction)(), uint32_t delay, uint32_t period) {
	// check if numTask is overflow
	if (SCH_TASK_LIST.numTask >= SCH_MAX_TASKS) {
		return;
	}
	// create new task and insert
	sTask temp;
	temp.pTask = pFunction;
	temp.Delay = delay / a;
	temp.Period = period /a;
	temp.RunMe = 0;
	insert_to_list(temp);
}

void SCH_Update() {
	// check if the list is empty
	if (!SCH_TASK_LIST.SCH_TASK_G[0].pTask) {
		return;
	}
	else {
		if (SCH_TASK_LIST.SCH_TASK_G[0].Delay == 0) {
			// The task is due to run
			// Inc. the "RunMe" flag
			SCH_TASK_LIST.SCH_TASK_G[0].RunMe += 1;
			if (SCH_TASK_LIST.SCH_TASK_G[0].Period) {
				// Schedule periodic tasks to run again
				SCH_TASK_LIST.SCH_TASK_G[0].Delay = SCH_TASK_LIST.SCH_TASK_G[0].Period;
			}
		}
		// Not yet ready to run: just decrement the delay
		else SCH_TASK_LIST.SCH_TASK_G[0].Delay -= 1;
	}
}

void SCH_Delete_Task() {
	int index = 0;
	// "flag" check to add task into queue to run again if it has period
	int flag = 0;
	sTask temp;
	// check one-shot task
	if (SCH_TASK_LIST.SCH_TASK_G[index].Period) {
		flag = 1;
		temp = SCH_TASK_LIST.SCH_TASK_G[index];
	}
	// shift left all tasks
	for (; index < SCH_TASK_LIST.numTask - 1; index++) {
		SCH_TASK_LIST.SCH_TASK_G[index] = SCH_TASK_LIST.SCH_TASK_G[index + 1];
	}
	// delete task rear after shift left
	SCH_TASK_LIST.SCH_TASK_G[index].pTask = 0x0000;
	SCH_TASK_LIST.SCH_TASK_G[index].Delay = 0;
	SCH_TASK_LIST.SCH_TASK_G[index].Period = 0;
	SCH_TASK_LIST.SCH_TASK_G[index].RunMe = 0;
	SCH_TASK_LIST.numTask -= 1;
	// check flag to add back into TASK_G
	if (flag == 1) {
		insert_to_list(temp);
	}
}

void SCH_Dispatch_Tasks() {
	// Dispatches (runs) the next task (if one is ready)
	for(int index = 0; index < SCH_MAX_TASKS; index++) {
		if (SCH_TASK_LIST.SCH_TASK_G[index].RunMe > 0) {
			// Run the task
			(*SCH_TASK_LIST.SCH_TASK_G[index].pTask)();
			// Reset / reduce RunMe flag
			SCH_TASK_LIST.SCH_TASK_G[index].RunMe -= 1;
			// schedule to delete task
			SCH_Delete_Task();
		}
	}
}

