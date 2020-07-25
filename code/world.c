#include <stdlib.h>
#include "sim.h"

const char *GOOD_NAMES[GOOD_LAST] = {
    [GOOD_FOOD]    = "food",
    [GOOD_CLOTHES] = "clothes",
    [GOOD_LABOR]   = "labor"
};

void init_world(struct world *world)
{
    for(int i = 0; i < 10; ++i)
    {
        world->pops[world->pop_count++] = (struct pop) {.money = 10000, .health = 10};
    }

    for(int i = 0; i < 10; ++i)
    {
        world->factories[world->factory_count++] = (struct factory) {.good = GOOD_FOOD, .money = 10000};
    }

    for(int i = 0; i < 10; ++i)
    {
        world->factories[world->factory_count++] = (struct factory) {.good = GOOD_CLOTHES, .money = 10000};
    }

    world->day = 1;
}

void simulate_world(struct world *world)
{
    // Open markets

    for(int i = 0; i < GOOD_LAST; ++i)
    {
        open_market(&world->markets[i]);
    }

    // Simulate pops

    int pop_offset = rand();
    for(int i = 0; i < world->pop_count; ++i)
    {
        struct pop *pop = &world->pops[(i + pop_offset) % world->pop_count];
        simulate_pop(world, pop);
    }

    for(int i = 0; i < world->pop_count; ++i)
    {
        struct pop *pop = &world->pops[i];
        if(pop->health == 0)
        {
            kill_pop(world, pop);
            --i;
        }
        else if(pop->health == 10)
        {
            reproduce_pop(world, pop);
        }
    }

    // Simulate factories

    int factory_offset = rand();
    for(int i = 0; i < world->factory_count; ++i)
    {
        struct factory *factory = &world->factories[(i + factory_offset) % world->factory_count];
        simulate_factory(world, factory);
    }

    for(int i = 0; i < world->factory_count; ++i)
    {
        struct factory *factory = &world->factories[i];
        if(factory->days_closed == 5)
        {
            destroy_factory(world, factory);
            --i;
        }
        else if(factory->money / 2 >= FACTORY_COST)
        {
            build_factory(world, factory);
        }
    }

    // Print the world

    if(world->day % 1 == 0)
    {
        print_world(world);
        getchar();
    }

    // Close markets

    for(int i = 0; i < GOOD_LAST; ++i)
    {
        close_market(&world->markets[i]);
    }

    ++world->day;
}


void print_world(struct world *world)
{
    puts("--- BEGIN ---");

#if PRINT_POPS
    // Print pops

    printf("%+8s %+8s %+8s %+8s\n", "POP", "MONEY", "LABOR", "HEALTH");

    for(int i = 0; i < world->pop_count; ++i)
    {
        struct pop *pop = &world->pops[i];
        printf("%8d %8d %8d %8d\n", i, pop->money, pop->labor, pop->health);
    }

    putchar('\n');
#endif

#if PRINT_FACTORIES
    // Print factories

    printf("%+8s %+8s %+8s %+8s %+8s\n", "FACTORY", "GOOD", "MONEY", "INV", "DAYCLOSD");

    for(int i = 0; i < world->factory_count; ++i)
    {
        struct factory *factory = &world->factories[i];
        printf("%8d %+8s %8d %8d %8d\n", i, GOOD_NAMES[factory->good], factory->money, factory->inventory, factory->days_closed);
    }

    putchar('\n');
#endif

    // Print markets

    printf("%+8s %+8s %+8s %+8s %+8s\n", "MARKET", "PRICE", "P_VEL", "DEMAND", "SUPPLY");

    for(int i = 0; i < GOOD_LAST; ++i)
    {
        struct market *market = &world->markets[i];
        printf("%+8s %8d %+8.2f %8d %8d\n", GOOD_NAMES[i], market->price, market->price_velocity, market->demand, market->supply);
    }

    putchar('\n');

    // Get stats

    int money_supply = 0;
    int food_factory_count = 0;
    int clothes_factory_count = 0;

    for(int i = 0; i < world->pop_count; ++i)
    {
        struct pop *pop = &world->pops[i];
        money_supply += pop->money;
    }

    for(int i = 0; i < world->factory_count; ++i)
    {
        struct factory *factory = &world->factories[i];
        money_supply += factory->money;
        switch(factory->good)
        {
            case GOOD_FOOD:
            {
                ++food_factory_count;
            }
            break;
            case GOOD_CLOTHES:
            {
                ++clothes_factory_count;
            }
            break;
        }
    }

    printf("Pops: %d\n", world->pop_count);
    printf("Factories (food): %d\n", food_factory_count);
    printf("Factories (clothes): %d\n", clothes_factory_count);
    printf("Day: %d\n", world->day);
    printf("Money supply: %d\n", money_supply);

    puts("--- END ---");
}
