#include "world.h"
#include <ncurses.h>

// Render the World's grid
void World::render()
{
    clear();
    for (int y = 0; y < yDim; y++)
    {
        for (int x = 0; x < xDim; x++)
        {
            char curSymbol = grid.at(y).at(x)->symbol;
            move(y, x);
            printw("%c", curSymbol);
        }
    }
    refresh();
}

// Place a particle on the World's grid at the specified coordinates
void World::placeParticle(int y, int x, Particle *particle)
{
    grid.at(y).at(x) = particle;
}