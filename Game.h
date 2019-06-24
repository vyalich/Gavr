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
#include "UI.h"


class Game
{
    public:
        Game();
        virtual ~Game();
        int    MainCycle(SDL_Surface *display, TTF_Font *font);
        bool    Init(Uint16 *user);
        bool    Event(SDL_Surface *display, TTF_Font *font);
        void    Compute();
        void    Draw(SDL_Surface *display, TTF_Font *font);
        void    Clean();


    protected:

    private:
        Field                   timer;
        Field                   username;
        Field                   points;
        int                     elapsed;
        bool                    running;
        SDL_Event               event;
        Uint64                  next_frame_time;
        Uint64                  next_spawn_time;
        int                     delay;
        int                     ticks;
        Uint64                  start;
        Hunter                  player;
        std::vector<Animal*>    Animals;
};

#endif // GAME_H
