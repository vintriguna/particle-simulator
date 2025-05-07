// #include <ncurses.h>
#include <iostream>
#include "particle.h"
#include "world.h"
#include <ncurses.h>
#include <unistd.h>

void handleMouseClick(World *world)
{

    MEVENT event;

    if (getmouse(&event) == OK)
    {

        int x = event.x;
        int y = event.y;

        if ((event.bstate & BUTTON1_PRESSED) || (event.bstate & BUTTON1_CLICKED))
        {
            Particle *newParticle = new Particle(ParticleType::SAND);
            world->placeParticle(y, x, newParticle);
            delete newParticle;
        }
    }
}

int main()
{

    World *world = new World(78, 22);

    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    mouseinterval(0);

    world->render();
    bool keepGoing = true;
    while (keepGoing)
    {
        int ch = getch();

        switch (ch)
        {

        case KEY_MOUSE:
            handleMouseClick(world);
            break;
        case 'q':
            keepGoing = false;
            break;
        case '1':
        case '2':
        case '3':
        case '4':
            world->setBrushSize(ch);
            break;
        }
        world->render();
        usleep(10000);
    }

    endwin();
    delete world;
    return 0;
}