// #include <ncurses.h>
#include <iostream>
#include "particle.h"
#include "world.h"
#include <ncurses.h>

int main()
{

    World *world = new World(100, 25);

    // for (int i = 0; i < world->yDim; i++)
    // {
    //     for (int j = 0; j < world->xDim; j++)
    //     {
    //         Particle particle = *(world->grid.at(i).at(j));
    //         std::cout << particle.symbol;
    //     }
    //     std::cout << "\n";
    // }

    initscr();

    world->render();

    getch();
    endwin();
    return 0;
}