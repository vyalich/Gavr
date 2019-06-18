#include "App.h"

App::App()
{
    //ctor
}

App::~App()
{
    //dtor
}

bool App::Init(){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        return false;
    atexit(SDL_Quit);

    if(TTF_Init() < 0)
        return false;
    atexit(TTF_Quit);

    display = SDL_SetVideoMode(640,480,32, SDL_ANYFORMAT | SDL_DOUBLEBUF | SDL_HWSURFACE);
    if (!display)
        return false;

    Draw_FillRect(display, 0, 0, display->w, display->h, 0xff00ff);
    SDL_Flip(display);
    SDL_Delay(1000);

    font = TTF_OpenFont("..//CharisSILR.ttf", 15);
    TextField Nickname(20, 40, 80, 40, 0x00ff00, 0, 0, 0);
    data.name = Nickname.Input(display, font);
    //SDL_EnableKeyRepeat(1, SDL_DEFAULT_REPEAT_INTERVAL / 3);

    return true;
}

void App::Execute(){

    Game m_Game;
    if(m_Game.Init())
        m_Game.MainCycle(display);
    m_Game.Clean();
}

void App::Clean(){
    SDL_FreeSurface(display);
    TTF_Quit();
    SDL_Quit();
}
