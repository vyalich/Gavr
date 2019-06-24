#ifndef APP_H
#define APP_H

#include "Game.h"
#include "UI.h"
#include <fstream>

struct User{
    Uint16* name;
    int     points;

    User();
    User(User&);
    ~User();
};

class App
{
    public:
        App();
        virtual ~App();
        bool Init();
        void Clean();
        void Execute();

    protected:

    private:
        TTF_Font                *font;
        SDL_Event               event;
        bool                    running;
        SDL_Surface             *display;
        SDL_Surface             *background;
        User                    current;
        std::vector<User*>           results;
};


#endif // APP_H
