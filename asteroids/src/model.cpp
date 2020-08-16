/* Asteroids model */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "model.h"
#include "utils.h"
#include "asteroids.h"


/* If you want to initialise a linked list:
    You'll have to replace node_t with the right type */

int missileX = 5;
int asteroidX = 5;
int pruneX = 0;
int collisionX = 6;
int outOfBoundsX = 0;

const size_t MAXSize = 10;

static missile_t missile_data[MAXSize];
static missile_t *freenodes;

static asteroids_t asteroid_data[MAXSize];
static asteroids_t *freenodes_asteroids;

//initialise the missile list
missile_t *initialise_missile_list()
{
    int c;
    for(c=0 ; c<(MAXSize-1) ; c++)
    {
        missile_data[c].next = &missile_data[c+1];
    }

    missile_data[c].next = NULL;
	freenodes = missile_data;

    return missile_data;
}

//initialise the asteroid list
asteroids_t *initialise_asteroid_list()
{
    int c;
    for(c=0 ; c<(MAXSize-1) ; c++)
    {
        asteroid_data[c].next = &asteroid_data[c+1];
    }

    asteroid_data[c].next = NULL;
	freenodes_asteroids = asteroid_data;

    return asteroid_data;
}

ship initialise_player()
{
		player.p.x = 240;
		player.p.y = 150;
		player.collision = 0;
	
		player_shield.s.x = player.p.x;
		player_shield.s.y = player.p.y + 7;
		player_shield.size = 20;
		// assign in view - player_shield.colour = GREEN;
		player_shield.quality = 3;
	
	return player;
}

//gets the next free node in the missile list
missile_t *getNode()
{
	missile_t *available = NULL;
	if (freenodes)
	{
		available = freenodes;
		freenodes = freenodes->next;
		available->next = NULL;
	}
	return available;
}

//gets the next free node in the asteroid list
asteroids_t *getNodeAsteroid()
{
	asteroids_t *available_asteroid = NULL;
	if (freenodes_asteroids)
	{
		available_asteroid = freenodes_asteroids;
		freenodes_asteroids = freenodes_asteroids->next;
		available_asteroid->next = NULL;
	}
	return available_asteroid;
}

//frees a node in the missile list
void freenode(struct missile *n)
{
    n->next = freenodes;
    freenodes = n;
	pruneX = 25;
}

//frees a node in the asteroids list
void freenodeAsteroid(struct asteroids *n)
{
    n->next = freenodes_asteroids;
    freenodes_asteroids = n;
}

//prunes a node in the missile list
struct missile *prunehead(struct missile *list)
{
	
	if (list)
	{
		//If out of bounds has been set to zero
		if(list->outOfBounds > 0 || list->collision > 0)
		{
			struct missile *discard = list;
			list = list->next;
			discard->next = NULL;
			freenode(discard);
		}		
	}
	return list;
}

struct missile *prune(struct missile *list)
{
	struct missile *m = prunehead(list);
	return m;
}

//prunes a node in the missile list
struct asteroids *prunehead_asteroid(struct asteroids *list)
{
	
	if (list)
	{	
		if(list->r.collision != 0)
		{
			//outOfBoundsX = 12;
			struct asteroids *discard = list;
			list = list->next;
			discard->next = NULL;
			freenodeAsteroid(discard);	
		}
	}
	return list;
}

struct asteroids *prune_asteroid(struct asteroids *list)
{
  	struct asteroids *a = prunehead_asteroid(list);
	return a;
}


ship move_ship(ship player, float Dt)
{
	
	//updates the player position based on it's velocity
	player.p.x = player.p.x + player.v.x * Dt;
	player.p.y = player.p.y + player.v.y * Dt;
		
	//wraps the ship around to the other edge of the screen if it would go out of the play area
	player = wrap_around(player);
	
	return player;
}

ship wrap_around(ship player)
{
	
	//If the ship has passed the right boundary, move it to the left
	if(player.p.x > 480)
	{
		player.p.x = 0;
	}
	//Else if the ship has passed the left boundary, move it to the right
	else if(player.p.x < 0)
	{
		player.p.x = 480;
	}
	
	
	//If the ship has passed the bottom boundary, move it to the top
	if(player.p.y > 275)
	{
		player.p.y = 10;
	}
	//Else if the ship has passed the top boundary, move it to the bottom
	else if(player.p.y < 10)
	{
		player.p.y = 275;
	}
	
	return player;
}

shield move_shield(ship player, shield player_shield, float Dt)
{
	
	player_shield.s.x = player.p.x;
	player_shield.s.y = player.p.y + 7;
	
	return player_shield;
}

void create_missile(struct missile *pew)
{
	//missile originates at the same point as the ship with the same velocity
	pew->p.x = player.p.x;
	pew->p.y = player.p.y;
	pew->v.x = player.v.x;
	pew->v.y = player.v.y;
	pew->outOfBounds = 0;
	pew->collision = 0;
	
}

asteroids_t create_asteroid(struct asteroids *rock)
{
	//creates a new asteroid at random position, with a random size and a random velocity
	rock->r.p.x = randrange(30,450);
	rock->r.p.y = randrange(30,250);
	rock->r.v.x = randrange(-25, 25);
	rock->r.v.y = randrange(-25, 25);
	rock->r.size = randrange(2, 20);
	rock->r.collision = 0;
	
	return *rock;
}

struct missile * update_missiles(struct missile *missile)
{
		
	struct missile *m = missile;
	
	for( ; missile; missile = missile->next)
	{
		//move the missile
		missile->p.x += missile->v.x * Dt;
		missile->p.y += missile->v.y * Dt;
		
		//check to see if the missile has hit an asteroid
		
		//check to see if the missle is out of bounds
		if(missile->p.x < 0 || missile->p.x > 480 || missile->p.y < 10 || missile->p.y > 275)
		{
			missile->outOfBounds = 1;
			//removes the missile from the list if this is true
			//
			//frees the pruned node
			//struct missile *expired = missile->next;
			//missile->next = missile->next->next;
			//freenode(expired);
		}
	}
	
		//removes any missiles which are out of bounds
		missile = prune(m);
	return missile;
}

asteroids_t wrap_around_asteroid(struct asteroids *rock)
{
	
	//If the ship has passed the right boundary, move it to the left
	if(rock->r.p.x > 480)
	{
		rock->r.p.x = 0;
	}
	//Else if the ship has passed the left boundary, move it to the right
	else if(rock->r.p.x < 0)
	{
		rock->r.p.x = 480;
	}
	
	//If the ship has passed the bottom boundary, move it to the top
	if(rock->r.p.y > 275)
	{
		rock->r.p.y = 10;
	}
	//Else if the ship has passed the top boundary, move it to the bottom
	else if(rock->r.p.y < 10)
	{
		rock->r.p.y = 275;
	}
	
	return *rock;
}

struct asteroids * update_asteroids(struct asteroids *rock)
{
	
	struct asteroids *a = rock;
	
	for( ; rock; rock = rock->next)
	{
		//move the rock
		rock->r.p.x += rock->r.v.x * Dt;
		rock->r.p.y += rock->r.v.y * Dt;	
		
		//check to see if the asteroid is out of bounds
		//if so, wrap it around to the other edge of the screen
		*rock = wrap_around_asteroid(rock);
		
	}
	
	rock = prune_asteroid(a);
	return rock;
	
}


struct missile *active = NULL;
struct asteroids *active_asteroid = NULL;

void move_missiles()
{
	//if a new missile has been fired
	if(missile_fired)
	{
		missile_fired = false;
		//get the next free space in the missile list
		struct missile *pew = getNode();
		if(pew)
		{
			//create a missile in the free space
			pew->next = active;
			active = pew;
			create_missile(pew);
		}
	}
	
	//moves existing missiles
	active = update_missiles(active);
	
}

void move_asteroids()
{
	//get the next free space in the asteroid list
	struct asteroids *rock = getNodeAsteroid();
	if(rock)
	{
		asteroidX = 122;
		//create a new asteroid
		rock->next = active_asteroid;
		active_asteroid = rock;
		create_asteroid(rock);
	}
	
	//move existing asteroids
	active_asteroid = update_asteroids(active_asteroid);
	
}

//Checks to see if there has been a collision between any of the missiles or asteroids
void missile_collision(struct missile *missile_list, struct asteroids *asteroid_list)
{
	//loops through all the missiles
	for( ; missile_list; missile_list = missile_list->next)
	{
		//for each asteroid in play
		for( ; asteroid_list ; asteroid_list = asteroid_list->next)
		{		
			//create local variables for missile and asteroid values
			float dx = asteroid_list->r.p.x - missile_list->p.x;
			float dy = asteroid_list->r.p.y - missile_list->p.y;			
			//calculate the distance between the missile and the asteroid
			float distance = sqrt(dx * dx + dy * dy);
			//check for a collision
			if(distance < asteroid_list->r.size + 1)
			{
				//remove the missile and asteroid from play
				missile_list->collision = 1;
				asteroid_list->r.collision = 1;
			}
		}
	}
}

bool ship_collision(struct asteroids *list)
{
	//for each asteroid in play
	for( ; list ; list = list->next)
	{
		//create local variables for the asteroid and ship values
		float dx = list->r.p.x - player.p.x;
		float dy = list->r.p.y - player.p.y;
		//calculate the distance between the asteroid and the ship
		float distance = sqrt( dx * dx + dy * dy);
		//check for a collision
		if (distance < list->r.size + 1)
		{
			return true;
		}		
	}
	return false;
}

void physics(void)
{
    if(!paused)
		{
			player = move_ship(player, Dt);
			player_shield = move_shield(player, player_shield, Dt);
			move_missiles();
			move_asteroids();
			//asteroids = create_asteroid(asteroids);
			
			missile_collision(active, active_asteroid);
			if (ship_collision(active_asteroid) )
			{
				if (player_shield.quality > 0)
				{
					player_shield.quality = player_shield.quality - 1;
				}
				else
				{
					if(lives > 0)
					{
						lives = lives - 1;
						player = initialise_player();
					}
					else
					{
						gameOver = 1;
					}
				}
			}
			
		}
		
}

