#include "world.h"
#include <ncurses.h>

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