#include <assert.h>
#include <stdlib.h>
#include "sim.h"

void simulate_pop(struct world *world, struct pop *pop)
{
    int budget_food    = pop->money * 0.4; // use 40% of money on food
    int budget_clothes = pop->money * 0.2; // use 20% of money on clothes
    int labor_output   = 5; // the amount of labour "produced" every day in some imaginary units
    int min_food       = 2; // the amount of food needed to stay healthy
    int min_clothes    = 1; // the amount of clothes needed to stay healthy

    // Buy food

    int food = buy(world, GOOD_FOOD, budget_food, &pop->money);

    // Buy clothes

    int clothes = buy(world, GOOD_CLOTHES, budget_clothes, &pop->money);

    // "Produce" labour

    pop->labor = labor_output;
    sell(world, GOOD_LABOR, pop->labor);

    // Simulate health

    if(food >= min_food && clothes >= min_clothes)
    {
        ++pop->health;
    }
    else
    {
        --pop->health;
    }
}

void kill_pop(struct world *world, struct pop *pop)
{
    int inheritance = pop->money;

    // Remove a pop using the swap and remove method

    assert(world->pop_count > 1);
    *pop = world->pops[--world->pop_count];

    // Money is inherited by a random pop

    world->pops[rand() % world->pop_count].money += inheritance;
}

void reproduce_pop(struct world *world, struct pop *pop)
{
    // Give birth i.e. slash health and money in half 😅

    int birth_money = pop->money / 2;

    pop->health = 5;
    pop->money -= birth_money;

    assert(MAX_POPS > world->pop_count);
    world->pops[world->pop_count++] = (struct pop) {.money=birth_money, .health=3};
}