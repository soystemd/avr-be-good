#include "button.h"
#include "config.h"
#include "tasker.h"
#include "motor.h"
#include <stdint.h>

// Static function prototypes:
static void
button_tap_event (void);

static void
button_hold_event (void);



static _Bool debounced = 0; // debounced = 1 means button is pressed

void
debounce (void)
{
	static uint8_t consecutive_changed_state_cycles = 0;
	_Bool raw = !GET(BUTTON); // raw = 1 means button is pressed

	if (raw != debounced)
	{
		consecutive_changed_state_cycles++;
		if (consecutive_changed_state_cycles >= 4)
			debounced = raw;
	}

	else
	{
		consecutive_changed_state_cycles = 0;
	}
}


void
button_event_handler (void)
{
	static uint8_t consecutive_button_down_cycles = 0;

	if (!debounced) // if button is depressed
	{
		if (consecutive_button_down_cycles >= 2 &&
		    consecutive_button_down_cycles <= 8)
			button_tap_event ();

		consecutive_button_down_cycles = 0;
	}

	else if (consecutive_button_down_cycles == 20)
	{
		button_hold_event ();

		/*
		    set cycles to 21 so the function will
		    do nothing after button_hold_event
		    is initiated, until button is depressed.
		*/
		consecutive_button_down_cycles = 21;
	}

	else if (consecutive_button_down_cycles == 21)
	{
		// do nothing;
	}

	else
	{
		consecutive_button_down_cycles++;
	}
}


static void
button_tap_event (void)
{
	task_set_state (motor_toggle_speed_control, RUNNABLE);
}


static void
button_hold_event (void)
{
	task_set_state (motor_toggle_on_off, RUNNABLE);
}
