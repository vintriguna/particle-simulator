// #include <ncurses.h>
#include <iostream>
#include "particle.h"
#include "world.h"
#include <ncurses.h>

int main()
{

    World *world = new World(100, 25);

    // for (int i = 0; i < world->yDim; i++)
    // {
    //     for (int j = 0; j < world->xDim; j++)
    //     {
    //         Particle particle = *(world->grid.at(i).at(j));
    //         std::cout << particle.symbol;
    //     }
    //     std::cout << "\n";
    // }

    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);

    while (true)
    {
        int ch = getch();

        if (ch == KEY_MOUSE)
        {

            MEVENT event;

            if (getmouse(&event) == OK)
            {

                int x = event.x;
                int y = event.y;

                if ((x >= 0 && x < world->xDim) && (y >= 0 && y < world->yDim))
                {

                    Particle *curParticle = new Particle();
                    curParticle->symbol = '*';
                    delete world->grid.at(y).at(x);
                    world->placeParticle(y, x, curParticle);
                }
            }
        }
        else if (ch == 'q')
        {
            break;
        }
        world->render();
    }

    endwin();
    delete world;
    return 0;
}