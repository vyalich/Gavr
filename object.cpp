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

void Dynamic::Move(){

}

Animal::Animal(){

}

Animal::~Animal(){

}

void Animal::Init(){
    int n = rand()%10;
    SDL_Surface *tmp = SDL_LoadBMP("./img/animal.bmp");
    SDL_SetColorKey(tmp, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(tmp->format, 255, 0, 255));
	sprite = SDL_DisplayFormat(tmp);
	SDL_FreeSurface(tmp);

    next_clip_time = SDL_GetTicks() + 100;
    w = sprite->w/3;
    h = sprite->h/2;
    clip.x = 0;
    clip.w = 27;
    clip.h = 26;
    SDL_Surface *display = SDL_GetVideoSurface();
    y = display->w/6 + rand()%(display->w/4);    //выбираю случайное место спауна
    if(n < 5){
        x = -w;
        clip.y = 0;
        speed = 2 + rand()%3;
        dest = display->w - 1;
    }
    else{
        x = display->w - 1;
        clip.y = 26;
        speed = -2 - rand()%3;
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
    if(SDL_GetTicks() >= next_clip_time){
        next_clip_time = SDL_GetTicks() + 100;
        clip.x += 27;
        if(clip.x == 81)
            clip.x = 0;
    }
}

void Animal::Draw(SDL_Surface *display){
    SDL_Rect coords;
    coords.x = x;
    coords.y = y;
    SDL_BlitSurface(sprite, &clip, display, &coords);
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
    SDL_Surface *tmp = SDL_LoadBMP("./img/hunter.bmp");
    SDL_SetColorKey(tmp, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(tmp->format, 255, 0, 255));
	sprite = SDL_DisplayFormat(tmp);
	SDL_FreeSurface(tmp);
    SDL_Surface *display = SDL_GetVideoSurface();
    w = sprite->w;
    h = sprite->h;
    x = display->w/2 - w/2;
    y = display->h - h - 10;
    last_shoot_time = 0;
    cooldown = 200;
    move_left = move_right = false;
    points = 0;
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
    SDL_Surface *tmp = SDL_CreateRGBSurface(SDL_HWSURFACE, 10, 10, 32, 0, 0, 0, 0);
    Draw_FillRect(tmp, 0, 0, 10, 10, 0xff00ff);
    Draw_FillCircle(tmp, 5, 5, 4, 0x000000);
    SDL_SetColorKey(tmp, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(tmp->format, 255, 0, 255));
	sprite = SDL_DisplayFormat(tmp);
	SDL_FreeSurface(tmp);
    speed = 4;
}

int Bullet::Compute(std::vector<Animal*> &Animals){
    y -= speed;
    if(y <= 40){
        on_screen = false;
        return 0;
    }
    for(int i = Animals.size() - 1; i >= 0; i--){
        if(Animals[i]->OnScreen() && Animals[i]->Collides(x, y)){
            on_screen = false;
            return Animals[i]->Die();
        }
    }
    return 0;
}
