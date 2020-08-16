/* Controller */

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* hardware platform libraries */
#include <mbed.h>

/* asteroids */
#include "model.h"
#include "asteroids.h"

/* Joystick 5-way switch
*/
typedef enum {JLT = 0, JRT, JUP, JDN, JCR} btnId_t;
enum position { left,down,right,up,centre };
DigitalIn joystick[] = {P5_0, P5_1, P5_4, P5_2, P5_3};
bool jsPrsdAndRlsd(btnId_t b);

/* Definition of Joystick press capture function
 * b is one of JLEFT, JRIGHT, JUP, JDOWN - from enum, 0, 1, 2, 3
 * Returns true if this Joystick pressed then released, false otherwise.
 *
 * If the value of the button's pin is 0 then the button is being pressed,
 * just remember this in savedState.
 * If the value of the button's pin is 1 then the button is released, so
 * if the savedState of the button is 0, then the result is true, otherwise
 * the result is false. */
bool jsPrsdAndRlsd(btnId_t b) {
	bool result = false;
	uint32_t state;
	static uint32_t savedState[4] = {1,1,1,1};
        //initially all 1s: nothing pressed
	state = joystick[b].read();
  if ((savedState[b] == 0) && (state == 1)) {
		result = true;
	}
	savedState[b] = state;
	return result;
}

bool missile_fired = false;

void controls(void)
{
	
		//missile_fired = true;
	
	if (jsPrsdAndRlsd(JUP))
	{
		//up was pressed
		//LEFT MAKES THIS HAPPEN
		player.v.x = -25.0;
	}
	else if (jsPrsdAndRlsd(JRT))
	{
		//right was pressed
		//DOWN MAKES THIS HAPPEN
		player.v.y = 25.0;
	}
	else if (jsPrsdAndRlsd(JLT))
	{
		//left was pressed.
		//RIGHT MAKES THIS HAPPEN
		player.v.x = 25.0;
	}
	else if (jsPrsdAndRlsd(JDN))
	{
		//down was pressed
		//UP MAKES THIS HAPPEN
		player.v.y = -25.0;
	}
	else if (jsPrsdAndRlsd(JCR))
	{
		//Pressing centre fires a missile
		missile_fired = true;
	}
}
