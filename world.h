#include <vector>
#include "particle.h"

#ifndef WORLD_H

#define WORLD_H

class World
{

public:
    int xDim;
    int yDim;
    int brushSize;
    int selectedParticleIdx;
    std::vector<ParticleType> availableTypes;
    std::vector<std::vector<Particle *>> grid;

    World(int xDim, int yDim)
    {
        this->xDim = xDim;
        this->yDim = yDim;
        this->brushSize = 1;
        this->selectedParticleIdx = 0;
        this->availableTypes = {ParticleType::SAND,
                                ParticleType::WATER};

        // fill grid with air particles
        for (int i = 0; i < yDim; i++)
        {
            std::vector<Particle *> nextRow;
            for (int j = 0; j < xDim; j++)
            {
                Particle *curParticle = new Particle(ParticleType::AIR);
                nextRow.push_back(curParticle);
            }
            grid.push_back(nextRow);
        }
        // make top row of border
        for (int i = 0; i < xDim; i++)
        {
            Particle *border = new Particle(ParticleType::WALL);
            grid.at(0).at(i) = border;
        }

        // make bottom row of border
        for (int i = 0; i < xDim; i++)
        {
            Particle *border = new Particle(ParticleType::WALL);
            grid.at(yDim - 1).at(i) = border;
        }

        // make left column of border
        for (int i = 0; i < yDim; i++)
        {
            Particle *border = new Particle(ParticleType::WALL);
            grid.at(i).at(0) = border;
        }

        // make right column of border
        for (int i = 0; i < yDim; i++)
        {
            Particle *border = new Particle(ParticleType::WALL);
            grid.at(i).at(xDim - 1) = border;
        }
    }

    void render();
    void tick();
    void updateParticle(Particle *particle, int y, int x);
    void placeParticle(int y, int x, Particle *particle);
    void setBrushSize(int size);
    bool inGrid(int destY, int destX);
    bool isAir(int destY, int destX);
    void swapWith(ParticleType type, int srcY, int srcX, int destY, int destX);
    void rotateSelectedType();
    ParticleType getCurrentType();
    void swapParticle(int srcY, int srcX, int destY, int destX);

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