#include "world.h"
#include <ncurses.h>
#include <cstdlib>
#include <string>

// Render the World's grid.
void World::render()
{
    clear();
    for (int y = 0; y < yDim; y++)
    {
        for (int x = 0; x < xDim; x++)
        {
            char curSymbol = grid.at(y).at(x)->symbol;
            int colorPair = grid.at(y).at(x)->getColorPair();
            attron(COLOR_PAIR(colorPair));
            move(y, x);
            printw("%c", curSymbol);
            attroff(COLOR_PAIR(colorPair));
        }
    }
    mvprintw(yDim + 2, 0, "BRUSH SIZE: %d", brushSize);
    std::string particleStr;
    switch (getCurrentType())
    {
    case ParticleType::AIR:
        particleStr = "AIR";
        break;
    case ParticleType::WATER:
        particleStr = "WATER";
        break;
    case ParticleType::SAND:
        particleStr = "SAND";
        break;
    case ParticleType::WALL:
        particleStr = "WALL";
        break;
    }
    mvprintw(yDim + 2, xDim, "PARTICLE: %s", particleStr.c_str());
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
                continue;
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

// Returns the current selected particle type.
ParticleType World::getCurrentType()
{
    return (availableTypes[selectedParticleIdx]);
}

void World::rotateSelectedType()
{
    selectedParticleIdx = (selectedParticleIdx + 1) % availableTypes.size();
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

bool World::canMoveTo(int destY, int destX)
{

    if ((destY < 0 || destY >= yDim) || (destX < 0 || destX >= xDim))
    {
        return false;
    }
    return true;
}

void World::swapWith(ParticleType type, int srcY, int srcX, int destY, int destX)
{
    delete grid.at(destY).at(destX);
    grid.at(destY).at(destX) = grid.at(srcY).at(srcX);
    grid.at(srcY).at(srcX) = new Particle(type);
}

// Causes a particle update depending on its type and location on the grid.
void World::updateParticle(Particle *particle, int y, int x)
{
    // Particle already updated this turn
    if (particle->updated)
    {
        return;
    }

    // deal with sand particle
    if (particle->type == ParticleType::SAND)
    {
        int belowY = y + 1;
        if (belowY < yDim)
        {
            int leftX = x - 1;
            int rightX = x + 1;
            bool canGoDiagonalLeft = (leftX >= 0) && grid.at(belowY).at(leftX)->type == ParticleType::AIR;
            bool canGoDiagonalRight = (rightX < xDim) && grid.at(belowY).at(rightX)->type == ParticleType::AIR;
            bool canGoDown = grid.at(belowY).at(x)->type == ParticleType::AIR;
            if (canGoDown)
            {
                swapWith(ParticleType::AIR, y, x, belowY, x);
            }
            else if (canGoDiagonalLeft && canGoDiagonalRight)
            {
                int leftOrRight = rand() % 2;
                if (leftOrRight == 0)
                {
                    swapWith(ParticleType::AIR, y, x, belowY, leftX);
                }
                else
                {
                    swapWith(ParticleType::AIR, y, x, belowY, rightX);
                }
            }
            else if (canGoDiagonalLeft)
            {
                swapWith(ParticleType::AIR, y, x, belowY, leftX);
            }
            else if (canGoDiagonalRight)
            {
                swapWith(ParticleType::AIR, y, x, belowY, rightX);
            }
        }
    }
    particle->updated = true;
}
