#include "Game.h"

bool Compare(Animal *A, Animal *B){
    return A->y < B->y;
}

Game::Game(){
    running         = true;
    pause           = false;
    delay           = 20;
    FPS             = 0;
    ticks           = 0;
    srand(time(0));
}

Game::~Game(){

}

int Game::MainCycle(SDL_Surface *display){
    Uint64 game_over = next_frame_time + 10000;
    while(SDL_GetTicks() < game_over && running){
        while(SDL_GetTicks() > next_frame_time){
            next_frame_time += delay;
            while(SDL_PollEvent(&event))
                Event();
            Compute();
            Draw(display);

        }
    }

    return player.GetPoints();
}

bool Game::Init(){
    player.Init();
    start = next_spawn_time = next_frame_time = SDL_GetTicks();

}

void Game::Event(){
    switch(event.type){
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym){
                case SDLK_ESCAPE:
                    running = false;
                    break;
                case SDLK_SPACE:
                    player.Shoot();
                    break;
                case SDLK_RIGHT:
                    player.SetRight();
                    break;
                case SDLK_LEFT:
                    player.SetLeft();
                    break;
            }
            break;
        case SDL_KEYUP:
            if(event.key.keysym.sym == SDLK_LEFT && player.MovingLeft() || event.key.keysym.sym == SDLK_RIGHT && player.MovingRight())
                player.StopMove();
            break;
    }
}

void Game::Compute(){
    if(SDL_GetTicks() > next_spawn_time){
        next_spawn_time = SDL_GetTicks() + 500 + rand()%1000;
        Animal *tmp = new Animal;
        tmp->Init();
        Animals.push_back(tmp);
        std::sort(Animals.begin(), Animals.end(), Compare);
    }

    player.Compute(Animals);

    for(int i = 0; i < Animals.size(); i++)
        Animals[i]->Compute();

    if(SDL_GetTicks() - start > 1000){
        start = SDL_GetTicks();
        FPS = ticks;
        ticks = 0;
    }
    ticks++;
}

void Game::Draw(SDL_Surface *display){
    Draw_FillRect(display, 0, 0, display->w, display->h/6, 0x00bfff);
    Draw_FillRect(display, 0, display->h/6, display->w, display->h/6*4, 0xc9bc0f);
    Draw_FillRect(display, 0, display->h/6*5, display->w, display->h/6, 0x99761b);

    player.Draw(display);

    for(int i = 0; i < Animals.size(); i++){
        if(Animals[i]->OnScreen())
            Animals[i]->Draw(display);
    }

    player.DrawBullets(display);

    /***************/
    SDL_Surface *message;
    TTF_Font *font;
    SDL_Rect dest = {.x = 100, .y = 100};
    SDL_Color textColor = {.r = 255, .g = 255, .b = 255};
    std::string str;

    str = std::to_string(FPS);
    font = TTF_OpenFont("CharisSILR.ttf", 20);
    message = TTF_RenderText_Solid( font, str.c_str(), textColor );
    if(message){
        SDL_BlitSurface(message, NULL, display, &dest);
        SDL_FreeSurface(message);
        message = NULL;
    }

    TTF_CloseFont(font);
    /**************/

    SDL_Flip(display);
}

void Game::Clean(){
    for(int i = 0; i < Animals.size(); i++)
        delete Animals[i];
    Animals.clear();
}
