#include "sim.h"

int main(void)
{
    struct world world = {0};

    init_world(&world);

    for(;;)
    {
        simulate_world(&world);
    }
}