#include <vector>
#include "particle.h"

#ifndef WORLD_H

#define WORLD_

class World
{

public:
    int xDim;
    int yDim;
    std::vector<std::vector<Particle *>> grid;

    World(int xDim, int yDim)
    {
        this->xDim = xDim;
        this->yDim = yDim;

        // fill grid with empty chars
        for (int i = 0; i < yDim; i++)
        {
            std::vector<Particle *> nextRow;
            for (int j = 0; j < xDim; j++)
            {
                Particle *curParticle = new Particle();
                curParticle->symbol = ' ';
                nextRow.push_back(curParticle);
            }
            grid.push_back(nextRow);
        }

        // make top row of border
        for (int i = 0; i < xDim; i++)
        {
            Particle *border = new Particle();
            border->symbol = '#';
            grid.at(0).at(i) = border;
        }

        // make bottom row of border
        for (int i = 0; i < xDim; i++)
        {
            Particle *border = new Particle();
            border->symbol = '#';
            grid.at(yDim - 1).at(i) = border;
        }

        // make left column of border
        for (int i = 0; i < yDim; i++)
        {
            Particle *border = new Particle();
            border->symbol = '#';
            grid.at(i).at(0) = border;
        }

        // make right column of border
        for (int i = 0; i < yDim; i++)
        {
            Particle *border = new Particle();
            border->symbol = '#';
            grid.at(i).at(xDim - 1) = border;
        }
    }

    ~World()
    {

        for (int i = 0; i < grid.size(); i++)
        {
            for (Particle *particle : grid.at(i))
            {
                delete particle;
            }
        }
    }
};

#endif