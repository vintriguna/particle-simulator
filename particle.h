#ifndef PARTICLE_H

#define PARTICLE_H

enum class ParticleType
{
    AIR,
    SAND,
    WALL,
    WATER
};

class Particle
{

public:
    char symbol;
    bool updated;
    ParticleType type;

    Particle(ParticleType type)
    {

        this->type = type;
        switch (type)
        {

        case ParticleType::SAND:
            symbol = '*';
            break;
        case ParticleType::AIR:
            symbol = ' ';
            break;
        case ParticleType::WALL:
            symbol = '#';
            break;
        case ParticleType::WATER:
            symbol = '&';
            break;
        }
    }

    int getColorPair();
};

#endif