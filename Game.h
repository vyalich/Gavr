#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_draw.h>
#include <SDL_ttf.h>
#include <string>
#include <stdlib.h>
#include <ctime>
#include <vector>
#include <algorithm>

#include "object.h"


class Game
{
    public:
        Game();
        virtual ~Game();
        void    MainCycle();
        bool    Init();
        void    Event();
        void    Compute();
        void    Draw();
        void    Clean();

    protected:

    private:
        bool                    running;
        bool                    pause;
        SDL_Surface             *display;
        SDL_Event               event;
        Uint64                  next_frame_time;
        Uint64                  next_spawn_time;
        int                     delay;
        int                     FPS;
        int                     ticks;
        Uint64                  start;
        Hunter                  player;
        std::vector<Animal*>    Animals;
};

#endif // GAME_H
