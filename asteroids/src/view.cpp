/* Asteroids view
*/

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

/* hardware platform libraries */
#include <display.h>
#include <mbed.h>

#include "asteroids.h"
#include "model.h"
#include "utils.h"

Display *graphics = Display::theDisplay();

const colour_t background = rgb(0,51,102); /* Midnight Blue */


const coordinate_t shape[] = {
    {10,0}, {-5,5}, {-5,-5}
};

/* double buffering functions */
void init_DBuffer(void)
{   /* initialise the LCD driver to use second frame in buffer */
    uint16_t *bufferbase = graphics->getFb();
    uint16_t *nextbuffer = bufferbase+ (480*272);
    LPC_LCD->UPBASE = (uint32_t)nextbuffer;
}

void swap_DBuffer(void)
{   /* swaps frames used by the LCD driver and the graphics object */
    uint16_t *buffer = graphics->getFb();
    graphics->setFb( (uint16_t*) LPC_LCD->UPBASE);
    LPC_LCD->UPBASE = (uint32_t)buffer;
}

void draw_sidebar(int score, float elapsed_time, int lives)
{
	//displays the lives remaining at the top left of the screen
	graphics->setCursor(2,2);
	graphics->printf("Lives Remaining: %d", lives);
	//displaus the game title at the top centre of the screen
	graphics->setCursor(199,2);
	 graphics->printf("Time Elapsed: %f", elapsed_time);
	//displays the score at the top right of the screen
	graphics->setCursor(400,2);
	graphics->printf("Score: %d", score);
    //forms a bar seperating the information from the play area
	graphics->fillRect(0, 10, 480, 1, WHITE);
	
		/*draws test values
		graphics->setCursor(60,90);
		graphics->printf("Player Position X: %f, ", player.p.x);
		graphics->printf("Player Position Y: %f, ", player.p.y);
		graphics->printf("Player Velocity X: %f, ", player.v.x);
		graphics->printf("Player Velocity Y: %f, ", player.v.y);
		graphics->printf("Missile fired: %d", missileX);
		graphics->printf("Asteroid fired: %d", asteroidX);
		graphics->printf("Prune: %d", pruneX);
		graphics->printf("Collision: %d", collisionX);
		graphics->printf("Out of Bounds: %D", outOfBoundsX);*/
	
}

void drawmissiles(struct missile *list)
{
	while(list)
	{
		if(list){
		graphics->fillCircle(list->p.x, list->p.y, 1, RED);
		list = list->next;
	}
}
}

void drawrocks(struct asteroids *list)
{
	while(list)
	{
		graphics->fillCircle(list->r.p.x, list->r.p.y, list->r.size, WHITE);
		list = list->next;
	}
}

void drawshield(shield player_shield)
{
	//If statements set the colour of the shield based on its quality
	if(player_shield.quality == 3)
	{
		//A quality of 3 produces a green shield
		player_shield.colour = GREEN;
		graphics->drawCircle(player_shield.s.x, player_shield.s.y, player_shield.size, player_shield.colour);
	}
	else if(player_shield.quality == 2)
	{
		//A quality of 2 produces a yellow shield
		player_shield.colour = YELLOW;
		graphics->drawCircle(player_shield.s.x, player_shield.s.y, player_shield.size, player_shield.colour);
	}
	else if(player_shield.quality == 1)
	{
		//A quality of 1 produces a red shield
		player_shield.colour = RED;
		graphics->drawCircle(player_shield.s.x, player_shield.s.y, player_shield.size, player_shield.colour);
	}
	else if(player_shield.quality == 0)
	{
		//A quality of zero produces no shield
	}
}


void drawship(ship player)
{
	
	/*float heading = radians(1);
	
	float x0, x2, newX0, newX1, newX2;
	float y0, y2, newY0, newY1, newY2;
	float originPointY, originPointX;
	float cosine = cos(heading);
	float sine = sin(heading);*/
	
	player.p0.x = player.p.x - 5;	
	player.p2.x = player.p.x + 5;
	
	player.p0.y = player.p.y + 15;
	player.p2.y = player.p.y + 15;
	/*
	//facing up/down
	//originPointY = (player.p.y - y0)/2;
	
	//facing right/left
	//originPointX = (player.p.x - x0)/2;
	
	originPointX = (x0 + player.p.x + x2) /3;
	originPointY = (y0 + player.p.y + y2) /3;
	
	newX0 = ((x0 - originPointX) * cosine) - ((y0 - originPointY) * sine +originPointX);
	//newX1 = (player.p.x - originPointX) * cosine - (player.p.y - originPointY) * sine +originPointX;
	newX2 = ((x2 - originPointX) * cosine) - ((y2 - originPointY) * sine +originPointX);
	
	x0 = newX0;
	//player.p.x = newX1;
	x2 = newX2;
	
	
	newY0 = ((x0 - originPointX) * cosine) - ((y0 - originPointY) * sine +originPointY);
	//newY1 = (player.p.y - originPointX) * cosine - (player.p.y - originPointY) * sine +originPointY;
	newY2 = ((x2 - originPointX) * cosine) - ((y2 - originPointY) * sine +originPointY);
	
	y0 = newY0;
	//player.p.y = newY1;
	y2 = newY2;
	
	*/
	
	graphics->drawTriangle(player.p0.x, player.p0.y, player.p.x, player.p.y, player.p2.x, player.p2.y, WHITE);
	graphics->fillTriangle(player.p0.x, player.p0.y, player.p.x, player.p.y, player.p2.x, player.p2.y, WHITE);
	
}


void draw(void)
{
    graphics->fillScreen(background);
		
	if (gameOver < 1)
	{
		draw_sidebar(score, elapsed_time, lives);
		drawmissiles(active);
		drawrocks(active_asteroid);
		drawshield(player_shield);
		drawship(player);

	}
	else if(paused)
	{
		graphics->setCursor(240, 140);
		graphics->printf("Paused");
	}
    else if(gameOver == 1)
	{
		graphics->setCursor(210, 120);
		graphics->printf("Game Over");
	}
    swap_DBuffer();
}
