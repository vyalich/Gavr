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

bool Animal::Collides(int b_x, int b_y){
    if(x <= b_x && b_x+10 <= x+w && y <= b_y && b_y+10 <= y+h)
        return true;
    return false;
}

int Animal::Die(){
    on_screen = false;
    return points;
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
    cooldown = 200;
    move_left = move_right = false;
}

void Hunter::Shoot(){
    if(SDL_GetTicks() > last_shoot_time + cooldown){
        last_shoot_time = SDL_GetTicks();
        Bullet *tmp = new Bullet;
        tmp->Init(x, y, w);
        Bullets.push_back(tmp);
    }
}

void Hunter::Compute(std::vector<Animal*> &Animals){
    x += speed;
    if(x < 10 || x > 600)
        x -= speed;

    for(int i = 0; i < Bullets.size(); i++){
        if(Bullets[i]->OnScreen())
            points += Bullets[i]->Compute(Animals);
    }
}

void Hunter::DrawBullets(SDL_Surface *display){
    for(int i = 0; i < Bullets.size(); i++){
        if(Bullets[i]->OnScreen())
            Bullets[i]->Draw(display);
    }
}

void Hunter::Clear(){
    for(int i = 0; i < Bullets.size(); i++){
        delete Bullets[i];
    }
    Bullets.clear();
}

Bullet::Bullet(){

}

Bullet::~Bullet(){

}

void Bullet::Init(int hunter_x, int hunter_y, int hunter_w){
    on_screen = true;
    x = hunter_x + hunter_w/2 - 5;
    y = hunter_y - 6;
    sprite = SDL_CreateRGBSurface(SDL_HWSURFACE, 10, 10, 32, 0, 0, 0, 0);
    Draw_FillCircle(sprite, 5, 5, 4, 0x00ff00);
    speed = 6;
}

int Bullet::Compute(std::vector<Animal*> &Animals){
    y -= speed;
    for(int i = Animals.size() - 1; i >= 0; i--){
         if(Animals[i]->OnScreen() && Animals[i]->Collides(x, y))
            return Animals[i]->Die();
    }
    return 0;
}
