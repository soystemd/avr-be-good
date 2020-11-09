/*
 * main.c
 * a simple tunable co-operative
 * task scheduler. capable of
 * running all the tasks in one cycle.
 */

#include <avr/interrupt.h>
#define CFG_TASKS
#define CFG_IO
#define CFG_REG
#include "config.h"

int
main (void)
{
	registers_init ();
	sei ();

	while (1)
		tsk_task_runner ();
}

ISR (TIMER0_OVF_vect)
{
	TCNT0 = TCNT0_VALUE;
	tsk_task_time_manager ();
}
