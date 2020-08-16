
struct rock {
    coordinate_t p;
    struct rock *next;
};

typedef struct node {
    rock asteroid;
    struct node *next;
} asteroid_t;

const size_t MAXSize = 10;
asteroid_t data[MAXSize];
asteroid_t *initialise()

{
    int c;
    for(c=0 ; c<(MAXSize-1) ; c++)
    {
        data[c].next = &data[c+1];
    }
    data[c].next = NULL;
    return data;
}
//
