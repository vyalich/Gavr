#include "Game.h"

bool Compare(Animal *A, Animal *B){
    return A->y < B->y;
}

Game::Game(): timer(395, 0, 10, 40, 0, 0xff, 0xff, 0xff), username(5, 0, 10, 40, 0, 0xff, 0xff, 0xff), points(785, 0, 10, 40, 0, 0xff, 0xff, 0xff) {
    running         = true;
    delay           = 8;
    ticks           = 0;
    elapsed         = 65;
    srand(time(0));
}

Game::~Game(){

}

int Game::MainCycle(SDL_Surface *display, TTF_Font *font){
    while(elapsed){
        while(SDL_GetTicks() > next_frame_time){
            next_frame_time += delay;
            if(elapsed >= 5){
                while(SDL_PollEvent(&event))
                    if(Event(display, font))
                        return 0;
            }
            else{
                player.StopMove();
            }
            Compute();
            Draw(display, font);

        }
    }
    return player.GetPoints();
}

bool Game::Init(Uint16 *user){

    player.Init();
    start = next_spawn_time = next_frame_time = SDL_GetTicks();
    username.SetInfo(user);
    return true;
}

bool Game::Event(SDL_Surface *display, TTF_Font *font){
    switch(event.type){
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym){
                case SDLK_ESCAPE:{
                    Uint64 tmp = next_frame_time - SDL_GetTicks();
                    Menu pause("./data/pause.dat", 300, 40, display->w/2 - 150, display->h/2-40, 300, 80, 0x939393, 0, 0, 0, VERTICAL);
                    pause.Draw(display, font, 0);
                    bool paused = true;
                    while(paused){
                        int x, y;
                        SDL_GetMouseState(&x, &y);
                        int key = pause.GetKey(y, VERTICAL);
                        if(pause.MouseOver(x, y))
                            pause.Draw(display, font, key);
                        else
                            pause.Draw(display, font, -1);
                        while(SDL_PollEvent(&event)){
                            if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && pause.MouseOver(x, y)){
                                if(key == 0)
                                    paused = false;
                                else if(key == 1)
                                    return true;
                            }
                        }
                    }
                    next_frame_time = SDL_GetTicks() + tmp;
                    break;
                }
                case SDLK_SPACE:{
                    player.Shoot();
                    break;
                }
                case SDLK_RIGHT:{
                    player.SetRight();
                    break;
                }
                case SDLK_LEFT:{
                    player.SetLeft();
                    break;
                }
            }
            break;
        case SDL_KEYUP:
            if(event.key.keysym.sym == SDLK_LEFT && player.MovingLeft() || event.key.keysym.sym == SDLK_RIGHT && player.MovingRight())
                player.StopMove();
            break;
    }
    return false;
}

void Game::Compute(){
    if(SDL_GetTicks() > next_spawn_time && elapsed > 5){
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
        ticks = 0;
        elapsed--;
    }
    ticks++;
}

void Game::Draw(SDL_Surface *display, TTF_Font *font){
    Draw_FillRect(display, 0, 0, display->w, display->h/6, 0x00bfff);
    Draw_FillRect(display, 0, 0, display->w, 40, 0x0f0f0f);
    Draw_FillRect(display, 0, display->h/6, display->w, display->h/6*4, 0xc9bc0f);
    Draw_FillRect(display, 0, display->h/6*5, display->w, display->h/6, 0x99761b);

    username.Draw(display, font, -1);
    timer.Draw(display, font, 0, (elapsed - 5 > 0)? elapsed - 5: 0);
    points.Draw(display, font, 1, player.GetPoints());

    player.Draw(display);

    for(int i = 0; i < Animals.size(); i++){
        if(Animals[i]->OnScreen())
            Animals[i]->Draw(display);
    }

    player.DrawBullets(display);

    SDL_Flip(display);
}

void Game::Clean(){
    for(int i = 0; i < Animals.size(); i++)
        delete Animals[i];
    Animals.clear();
}
