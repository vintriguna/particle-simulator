#include "world.h"
#include <ncurses.h>

// Render the World's grid.
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

// Place a particle on the World's grid at the specified coordinates.
void World::placeParticle(int y, int x, Particle *prototype)
{
    int half = brushSize / 2;
    for (int i = -half; i <= half; i++)
    {
        for (int j = -half; j <= half; j++)
        {

            int ny = y + i;
            int nx = x + j;
            bool fitsInGrid = (nx >= 0 && nx < xDim) && (ny >= 0 && ny < yDim);
            if (!fitsInGrid)
            {
                return;
            }
            bool occupiedByAir = grid.at(ny).at(nx)->type == ParticleType::AIR;
            bool canPlace = occupiedByAir && fitsInGrid;
            if (canPlace)
            {

                Particle *curParticle = new Particle(prototype->type);
                delete grid.at(ny).at(nx);
                grid.at(ny).at(nx) = curParticle;
            }
        }
    }
}

// Change the size of the brush.
void World::setBrushSize(int size)
{
    int newSize = size - '0';
    brushSize = newSize;
}

void World::tick()
{
    // first pass thru grid to update particles
    for (int y = 0; y < yDim; y++)
    {
        for (int x = 0; x < xDim; x++)
        {
            Particle *curParticle = grid.at(y).at(x);
            updateParticle(curParticle, y, x);
        }
    }

    // second pass thru grid to reset all particles update states
    for (int y = 0; y < yDim; y++)
    {
        for (int x = 0; x < xDim; x++)
        {
            Particle *curParticle = grid.at(y).at(x);
            curParticle->updated = false;
        }
    }
}

// Causes a particle update depending on its type and location on the grid.
void World::updateParticle(Particle *particle, int y, int x)
{
    // Particle already updated this turn
    if (particle->updated)
    {
        return;
    }

    if (particle->type == ParticleType::SAND)
    {
        if (y > 0)
        {
            int belowY = y + 1;
            if (grid.at(belowY).at(x)->type == ParticleType::AIR)
            {
                delete grid.at(belowY).at(x);
                grid.at(y + 1).at(x) = particle;
                Particle *air = new Particle(ParticleType::AIR);
                grid.at(y).at(x) = air;
            }
        }
    }

    particle->updated = true;
}
