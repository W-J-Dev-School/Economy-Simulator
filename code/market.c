#include <math.h>
#include "sim.h"

void open_market(struct market *market)
{
    market->price = MAX(market->price, 1);
    market->price_float = MAX(market->price_float, 1);
    market->demand = 0;
    market->supply = 0;
}

void close_market(struct market *market)
{
    if(market->demand > market->supply)
    {
        // Price should go up because demand is higher than supply

        if(market->price_velocity > 0)
        {
            market->price_velocity *= 1.5;
        }
        else
        {
            market->price_velocity *= -0.5;
        }

        market->price_velocity = MAX(market->price_velocity, 0.01);
    }
    else if(market->supply > market->demand)
    {
        // Price should go up because supply is higher than demand

        if(market->price_velocity < 0)
        {
            market->price_velocity *= 1.5;
        }
        else
        {
            market->price_velocity *= -0.5;
        }

        market->price_velocity = MIN(market->price_velocity, -0.01);
    }
    else
    {
        // Price should stay the same because supply is the same as demand

        market->price_velocity = 0;
    }

    market->price_float += market->price_velocity;
    market->price = round(market->price_float);
}