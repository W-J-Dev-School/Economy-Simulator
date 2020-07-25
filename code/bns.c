#include <assert.h>
#include <stdlib.h>
#include "sim.h"

int buy(struct world *world, enum good good, int budget, int *money)
{
    switch(good)
    {
        case GOOD_FOOD:
        case GOOD_CLOTHES:
        {
            struct market *market = &world->markets[good];

            assert(budget <= *money);

            int target_amount = budget / market->price;
            int bought_amount = 0;

            market->demand += target_amount;

            int offset = rand();

            for(int i = 0; i < world->factory_count; ++i)
            {
                struct factory *factory = &world->factories[(i + offset) % world->factory_count];

                if(factory->good == good)
                {
                    int amount = MIN(target_amount - bought_amount, factory->inventory);
                    int cost = amount * market->price;

                    factory->money += cost;
                    *money -= cost;

                    bought_amount += amount;

                    if(bought_amount == target_amount)
                    {
                        break;
                    }
                }
            }

            return bought_amount;
        }
        break;

        case GOOD_LABOR:
        {
            struct market *market = &world->markets[GOOD_LABOR];
            
            assert(budget <= *money);

            int target_amount = budget / market->price;
            int bought_amount = 0;

            market->demand += target_amount;

            int offset = rand();

            for(int i = 0; i < world->pop_count; ++i)
            {
                struct pop *pop = &world->pops[(i + offset) % world->pop_count];

                int amount = MIN(target_amount - bought_amount, pop->labor);
                int cost = amount * market->price;

                pop->money += cost;
                *money -= cost;

                bought_amount += amount;

                if(bought_amount == target_amount)
                {
                    break;
                }
            }

            return bought_amount;
        }
        break;
    }
}

void sell(struct world *world, enum good good, int amount)
{
    world->markets[good].supply += amount;
}