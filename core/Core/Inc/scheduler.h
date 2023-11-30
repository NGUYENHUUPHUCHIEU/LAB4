/*
 * scheduler.h
 *
 *  Created on: Nov 29, 2023
 *      Author: Acer
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_
#include <stdint.h>

#define SCH_MAX_TASKS	40
#define a			10

typedef struct {
	void ( * pTask)(void);
	uint32_t Delay;
	uint32_t Period;
	uint8_t RunMe;
	uint32_t TaskID;
} sTask;

typedef struct {
	sTask SCH_TASK_G[SCH_MAX_TASKS];
	int numTask;
} taskList;

taskList SCH_TASK_LIST;

void SCH_Init(void);
void SCH_Add_Task(void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD);
void SCH_Update(void);
void SCH_Dispatch_Tasks(void);
void SCH_Delete_Task(void);
void insert_to_list(sTask task);


#endif /* INC_SCHEDULER_H_ */
