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
    mvprintw(yDim + 2, 0, "BRUSH SIZE: %d", brushSize);
    refresh();
}

// Place a particle on the World's grid at the specified coordinates
void World::placeParticle(int y, int x, Particle *prototype)
{
    int half = brushSize / 2;
    for (int i = -half; i <= half; i++)
    {
        for (int j = -half; j <= half; j++)
        {

            int ny = y + i;
            int nx = x + j;
            if ((nx >= 0 && nx < xDim) && (ny >= 0 && ny < yDim))
            {

                Particle *curParticle = new Particle(prototype->type);
                delete grid.at(ny).at(nx);
                grid.at(ny).at(nx) = curParticle;
            }
        }
    }
}

// Change the size of the brush
void World::setBrushSize(int size)
{
    int newSize = size - '0';
    brushSize = newSize;
}