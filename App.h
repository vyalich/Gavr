#ifndef APP_H
#define APP_H

#include "Game.h"
#include "UI.h"

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
        TTF_Font        *font;
        bool            running;
        SDL_Surface     *display;
        struct          User{
            std::string name;
            int         points;
        } data;
};

#endif // APP_H
