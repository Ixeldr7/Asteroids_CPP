/* Asteroids
    Semester 2 -- Small Embedded Systems
    Dr Alun Moon
*/

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

/* hardware platform libraries */


/* Main game elements */
#include "model.h"
#include "view.h"
#include "controller.h"

/* Game state */
float elapsed_time; 
int   score;
int   lives;
bool gameOver;
struct ship player;
struct shield player_shield;

float Dt = 0.01f;

Ticker model, view, controller;

bool paused = true;
/* The single user button needs to have the PullUp resistor enabled */
DigitalIn userbutton(P2_10,PullUp);
int main()
{

    init_DBuffer();
    initialise_missile_list();
	initialise_asteroid_list();		
	player = initialise_player();
		
    view.attach( draw, 0.025);
    model.attach( physics, Dt);
    controller.attach( controls, 0.1);
    
    lives = 5;
		score = 0;		
    
    paused = false;
    
    while(true) {
				score = score +1;
			
				wait_ms(200);
        
        while(lives > 0)
        {
			elapsed_time = elapsed_time + 1;
			score = elapsed_time;
            wait_ms(200);
        }       
        
    }
}
