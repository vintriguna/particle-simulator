#include "particle.h"

int Particle::getColorPair()
{
    switch (type)
    {
    case ParticleType::SAND:
        return 1;
    case ParticleType::WATER:
        return 2;
    case ParticleType::STONE:
        return 3;
    default:
        return 0;
    }
};