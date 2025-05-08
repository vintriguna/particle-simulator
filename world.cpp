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
    if (eraserEnabled)
    {
        mvprintw(yDim + 2, xDim - 14, "ERASER ENABLED");
    }
    else
    {
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
        default:
            particleStr = "??";
            break;
        }
        mvprintw(yDim + 2, xDim - 14, "PARTICLE: %s", particleStr.c_str());
    }
    refresh();
}

void World::eraseParticle(int y, int x)
{

    int half = brushSize / 2;
    for (int i = -half; i <= half; i++)
    {
        for (int j = -half; j <= half; j++)
        {

            int ny = y + i;
            int nx = x + j;
            bool fitsInGrid = (nx >= 0 && nx < xDim) && (ny >= 0 && ny < yDim);
            if ((!fitsInGrid) || grid.at(ny).at(nx)->type == ParticleType::WALL)
            {
                continue;
            }

            delete grid.at(ny).at(nx);
            grid.at(ny).at(nx) = new Particle(ParticleType::AIR);
        }
    }
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

bool World::inGrid(int destY, int destX)
{

    if ((destY < 0 || destY >= yDim) || (destX < 0 || destX >= xDim))
    {
        return false;
    }
    return true;
}

bool World::isAir(int destY, int destX)
{
    if (inGrid(destY, destX) && (grid.at(destY).at(destX)->type == ParticleType::AIR))
    {
        return true;
    }
    return false;
}

void World::swapParticle(int srcY, int srcX, int destY, int destX)
{
    Particle *temp = grid.at(srcY).at(srcX);
    grid.at(srcY).at(srcX) = grid.at(destY).at(destX);
    grid.at(destY).at(destX) = temp;
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

    int belowY = y + 1;
    int leftX = x - 1;
    int rightX = x + 1;

    // deal with sand particle
    if (particle->type == ParticleType::SAND)
    {
        if (belowY < yDim)
        {
            bool canGoDiagonalLeft = (leftX >= 0) && grid.at(belowY).at(leftX)->type == ParticleType::AIR || (leftX >= 0) && grid.at(belowY).at(leftX)->type == ParticleType::WATER;
            bool canGoDiagonalRight = (rightX < xDim) && grid.at(belowY).at(rightX)->type == ParticleType::AIR || (rightX < xDim) && grid.at(belowY).at(rightX)->type == ParticleType::WATER;
            bool canGoDown = grid.at(belowY).at(x)->type == ParticleType::AIR || grid.at(belowY).at(x)->type == ParticleType::WATER;
            if (canGoDown)
            {
                swapParticle(y, x, belowY, x);
            }
            else if (canGoDiagonalLeft && canGoDiagonalRight)
            {
                int leftOrRight = rand() % 2;
                if (leftOrRight == 0)
                {
                    swapParticle(y, x, belowY, leftX);
                }
                else
                {
                    swapParticle(y, x, belowY, rightX);
                }
            }
            else if (canGoDiagonalLeft)
            {
                swapParticle(y, x, belowY, leftX);
            }
            else if (canGoDiagonalRight)
            {
                swapParticle(y, x, belowY, rightX);
            }
        }
    }
    // deal with water particle
    else if (particle->type == ParticleType::WATER)
    {
        bool canGoDown = isAir(belowY, x);
        bool canGoLeft = isAir(y, leftX);
        bool canGoRight = isAir(y, rightX);
        bool canGoDiagonalLeft = isAir(belowY, leftX);
        bool canGoDiagonalRight = isAir(belowY, rightX);

        if (inGrid(y, x))
        {
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
            else if (canGoLeft && canGoRight)
            {
                int leftOrRight = rand() % 2;
                if (leftOrRight == 0)
                {
                    swapWith(ParticleType::AIR, y, x, y, leftX);
                }
                else
                {
                    swapWith(ParticleType::AIR, y, x, y, rightX);
                }
            }
            else if (canGoLeft)
            {
                swapWith(ParticleType::AIR, y, x, y, leftX);
            }
            else if (canGoRight)
            {
                swapWith(ParticleType::AIR, y, x, y, rightX);
            }
        }
    }
    particle->updated = true;
}
