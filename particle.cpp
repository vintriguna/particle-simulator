#include "particle.h"

int Particle::getColorPair()
{
    switch (type)
    {
    case ParticleType::SAND:
        return 1;
    case ParticleType::WATER:
        return 2;
    default:
        return 0;
    }
};