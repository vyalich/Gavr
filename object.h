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
        int         Die();
        friend bool Compare(Animal*, Animal*);
        bool        Collides(int x, int y);

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
        void Init(int hunter_x, int hunter_y, int hunter_w);
        int  Compute(std::vector<Animal*> &Animals);
        bool OnScreen() { return on_screen; }

    protected:

    private:
        bool on_screen;

};


class Hunter: public Static, public Dynamic{
    public:
        Hunter();
        ~Hunter();
        void Init();
        void StopMove() { speed = 0; move_left = move_right = false; }
        void SetLeft() { speed = -6; move_left = true; move_right = false; }
        void SetRight() { speed = 6; move_left = false; move_right = true; }
        bool MovingLeft() { return move_left; }
        bool MovingRight() { return move_right; }
        void Shoot();
        void Compute(std::vector<Animal*> &Animals);
        void DrawBullets(SDL_Surface *display);
        int  GetPoints() { return points; }
        void Clear();



    protected:

    private:
        bool                move_left;
        bool                move_right;
        Uint64              last_shoot_time;
        int                 cooldown;
        int                 points;
        std::vector<Bullet*> Bullets;
};

#endif // OBJECT_H
