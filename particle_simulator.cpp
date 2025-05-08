// #include <ncurses.h>
#include <iostream>
#include "particle.h"
#include "world.h"
#include <ncurses.h>
#include <vector>
#include <unistd.h>

void handleMouseClick(World *world)
{

    MEVENT event;

    if (getmouse(&event) == OK)
    {

        int x = event.x;
        int y = event.y;

        if ((event.bstate & BUTTON1_PRESSED) || (event.bstate & BUTTON1_CLICKED) || event.bstate & REPORT_MOUSE_POSITION)
        {

            if (world->eraserEnabled)
            {
                world->eraseParticle(y, x);
            }
            else
            {
                Particle *newParticle = new Particle(world->getCurrentType());
                world->placeParticle(y, x, newParticle);
                delete newParticle;
            }
        }
    }
}

int main()
{
    srand(time(0));
    World *world = new World(100, 30);
    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    mouseinterval(0);
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);  // Sand
    init_pair(2, COLOR_CYAN, COLOR_BLACK);    // Water
    init_pair(3, COLOR_MAGENTA, COLOR_BLACK); // Stone

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
        case '\t':
            world->rotateSelectedType();
            break;
        case 'c':
            world->eraserEnabled = !(world->eraserEnabled);
            break;
        }
        world->tick();
        world->render();
        world->frameCount = world->frameCount + 1;
        usleep(50000);
    }

    endwin();
    delete world;
    return 0;
}