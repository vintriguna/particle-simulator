#include "particle.h"

int Particle::getColorPair()
{
    switch (type)
    {
    case ParticleType::SAND:
        return 1;
    default:
        return 0;
    }
};