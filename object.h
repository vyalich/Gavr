#ifndef OBJECT_H
#define OBJECT_H

#include <SDL.h>
#include <SDL_draw.h>
#include <SDL_ttf.h>
#include <cstdlib>
#include <ctime>
#include <vector>

class Static{
    public:
        Static();
        ~Static();
        virtual void Draw(SDL_Surface *display);

    protected:
        SDL_Surface     *sprite;
        int             x;
        int             y;
        int             w;
        int             h;

    private:

};

class Dynamic{
    public:
        Dynamic();
        ~Dynamic();

    protected:
        int     speed;

    private:

};

class Animal: public Static, public Dynamic{
    public:
        Animal();
        ~Animal();
        void        Init();
        void        Compute();
        int         GetPoints() { return points; }
        bool        OnScreen() { return on_screen; }
        void        Die() { on_screen = false; }
        friend bool Compare(Animal*, Animal*);

    protected:

    private:
        bool    on_screen;
        int     dest;
        int     points;

};

class Bullet: public Static, public Dynamic{
    public:
        Bullet();
        ~Bullet();
        void Init();
        void Compute();

    protected:

    private:

};


class Hunter: public Static, public Dynamic{
    public:
        Hunter();
        ~Hunter();
        void Init();
        void StopMove() { speed = 0; }
        void SetLeft() { speed = -6; }
        void SetRight() { speed = 6; }
        void Shoot();
        void Compute();
        void DrawBullets();
        void GainPoints();



    protected:

    private:
        Uint64              last_shoot_time;
        int                 cooldown;
        int                 points;
        std::vector<Bullet*> Bullets;
};

#endif // OBJECT_H
