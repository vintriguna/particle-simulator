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

                Particle *curParticle = new Particle();
                curParticle->symbol = prototype->symbol;

                delete grid.at(ny).at(nx);
                grid.at(ny).at(nx) = curParticle;
            }
        }
    }

    // if ((x >= brushSize - 1 && x < xDim - brushSize) && (y >= brushSize - 1 && y < yDim - brushSize))
    // {

    //     for (int i = 0; i < brushSize; i++)
    //     {
    //         for (int j = 0; j < brushSize; j++)
    //         {
    //             Particle *curParticle = new Particle();
    //             curParticle->symbol = prototype->symbol;

    //             delete grid.at(y + i).at(x + j);
    //             grid.at(y + i).at(x + j) = curParticle;
    //         }
    //     }
    // }
}

// Change the size of the brush
void World::setBrushSize(int size)
{
    int newSize = size - '0';
    brushSize = newSize;
}