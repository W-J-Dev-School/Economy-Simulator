#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "sim.h"

void simulate_factory(struct world *world, struct factory *factory)
{
    int budget_labor = factory->money * 0.8;
    int min_labor    = 1;

    // Buy labour

    int labor = buy(world, GOOD_LABOR, budget_labor, &factory->money);

    // "Produce" the good

    if(labor >= min_labor)
    {
        // Produce the inventory depending on factory type

        switch(factory->good)
        {
            case GOOD_FOOD:
            {
                // Simulate decay and production of food

                factory->inventory *= 0.9;
                factory->inventory += 2 * log(labor + 1);
            }
            break;

            case GOOD_CLOTHES:
            {
                // Simulate decay and production of clothes

                factory->inventory *= 0.98;
                factory->inventory += 0.7 * log(labor + 1);
            }
            break;
        }

        // Put the inventory for sale

        sell(world, factory->good, factory->inventory);

        // Reset days closed

        factory->days_closed = 0;
    }
    else
    {
        ++factory->days_closed;
    }
}

void destroy_factory(struct world *world, struct factory *factory)
{
    int remaining_money = factory->money;

    // Remove the factory using the swap and remove method

    assert(world->factory_count > 1);
    *factory = world->factories[--world->factory_count];

    // Money is inherited by a random factory

    world->pops[rand() % world->pop_count].money += remaining_money;
}

void build_factory(struct world *world, struct factory *parent_factory)
{
    // Use parent factory's money to fund a new one

    assert(parent_factory->money >= FACTORY_COST);
    parent_factory->money -= FACTORY_COST;

    assert(world->factory_count < MAX_FACTORIES);
    world->factories[world->factory_count++] = (struct factory) {.good=parent_factory->good, .money=FACTORY_COST};
}