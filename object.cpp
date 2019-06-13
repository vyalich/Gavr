#include "object.h"


Static::Static(){

}

Static::~Static(){
    SDL_FreeSurface(sprite);
}

void Static::Draw(SDL_Surface *display){
    SDL_Rect coords;
    coords.x = x;
    coords.y = y;
    SDL_BlitSurface(sprite, 0, display, &coords);
}

Dynamic::Dynamic(){

}

Dynamic::~Dynamic(){

}

Animal::Animal(){

}

Animal::~Animal(){

}

void Animal::Init(){
    int n = rand()%10;
    sprite = SDL_CreateRGBSurface(SDL_HWSURFACE, 40, 20, 32, 0, 0, 0, 0);   //создаю поверхность для спрайта
    Draw_FillRect(sprite, 0, 0, 40, 20, 0xffffff);
    Draw_Rect(sprite, 0, 0, 40, 20, 0x000000);
    w = sprite->w;
    h = sprite->h;
    SDL_Surface *display = SDL_GetVideoSurface();
    y = display->w/6 + rand()%(display->w/4);    //выбираю случайное место спауна
    if(n < 5){
        x = -w;
        speed = 5 + rand()%3;
        dest = display->w - 1;
    }
    else{
        x = display->w - 1;
        speed = -5 - rand()%3;
        dest = -w;
    }
    points = 10+rand()%6;
    on_screen = true;
}

void Animal::Compute(){
    if(!on_screen)
        return;
    if(abs(dest - x) > abs(speed))
        x += speed;
    else
        on_screen = false;

}

Hunter::Hunter(){

}

Hunter::~Hunter(){

}

void Hunter::Init(){
    sprite = SDL_CreateRGBSurface(SDL_HWSURFACE, 30, 30, 32, 0, 0, 0, 0);
    Draw_FillRect(sprite, 0, 0, 30, 30, 0xff0000);
    Draw_Rect(sprite, 0, 0, 30, 30, 0x000000);
    SDL_Surface *display = SDL_GetVideoSurface();
    w = sprite->w;
    h = sprite->h;
    x = display->w/2 - w/2;
    y = display->h - h - 10;
    last_shoot_time = 0;
    cooldown = 100;
}

void Hunter::Shoot(){
    if(SDL_GetTicks() > last_shoot_time + cooldown){
        Bullet *tmp = new Bullet;
        tmp->Init();
        Bullets.push_back(tmp);
    }
}

void Hunter::Compute(){
    x += speed;
    if(x < 10 || x > 590)
        x -= speed;
}

Hunter::DrawBullets(){

}

Bullet::Bullet(){

}

Bullet::~Bullet(){

}
