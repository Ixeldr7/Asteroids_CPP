/* Asteroids Model */
struct point {
    float x,y;
};
typedef struct point coordinate_t;
typedef struct point vector_t;

struct ship {
	coordinate_t p0;
    coordinate_t p;
	coordinate_t p2;
    vector_t     v;
	int collision;
	float heading;
	float headingRands;
};

struct shield {
	coordinate_t s;
	int size;
	int colour;
	bool exists;
	int quality;
};


/* initial struts for building linked lists */
struct rock {
    coordinate_t p;
	vector_t     v;
	int size;
	int collision;
    struct rock *next;
};

typedef struct rock rock_t;

typedef struct asteroids {
	rock r;		
	struct asteroids *next;
} asteroids_t;


typedef struct missile {
    coordinate_t p;
	vector_t v;
	int outOfBounds;
	int collision;
    struct missile *next;
} missile_t;

void physics(void);
ship move_ship(ship, float);
shield move_shield(ship, shield, float);
ship wrap_around(ship);
missile_t *initialise_missile_list();
asteroids_t *initialise_asteroid_list();
ship initialise_player();


extern missile_t *active;
extern asteroids_t *active_asteroid;

extern int missileX;
extern int asteroidX;
extern int pruneX;
extern int collisionX;
extern int outOfBoundsX;
