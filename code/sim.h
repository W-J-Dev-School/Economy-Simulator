#ifndef SIM_H
#define SIM_H

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define FACTORY_COST 1000

#define PRINT_POPS 1
#define PRINT_FACTORIES 1

enum good
{
    GOOD_FOOD    = 0, // made by food factories, consumed by pops
    GOOD_CLOTHES = 1, // made by cloth factories, consumed by pops
    GOOD_LABOR   = 2, // made by pops, consumed by factories
    GOOD_LAST    = 3
};

extern const char *GOOD_NAMES[GOOD_LAST];

struct market
{
    int   price;
    float price_float;
    float price_velocity;
    int   demand;
    int   supply;
};

struct pop
{
    int money;  // money does this pop own
    int labor;  // how much labour does this pop have available
    int health; // pops with 0 health die
};

struct factory
{
    enum good good;        // what is this factory producing
    int       money;       // how much money does this factory own
    int       inventory;   // how much of the produced good does this factory have available
    int       days_closed; // factories closed for 5 days go bankrupt
};

#define MAX_POPS      8192
#define MAX_FACTORIES 8192

struct world
{
    struct market   markets[GOOD_LAST];       // markets for each good
    struct pop      pops[MAX_POPS];           // the array containing pops
    int             pop_count;                // the number of pops
    struct factory  factories[MAX_FACTORIES]; // the array containing factories
    int             factory_count;            // the number of factories
    int             day;                      // the current day number
};

// --- world.c ---

void init_world(struct world *world);
void simulate_world(struct world *world);
void print_world(struct world *world);

// --- bns.c ---

int buy(struct world *world, enum good good, int budget, int *money);
void sell(struct world *world, enum good good, int amount);

// --- market.c ---

void open_market(struct market *market);
void close_market(struct market *market);

// --- pop.c ---

void simulate_pop(struct world *world, struct pop *pop);
void kill_pop(struct world *world, struct pop *pop);
void reproduce_pop(struct world *world, struct pop *pop);

// --- factory.c ---

void simulate_factory(struct world *world, struct factory *factory);
void destroy_factory(struct world *world, struct factory *factory);
void build_factory(struct world *world, struct factory *parent_factory);

#endif