#ifndef UI_H
#define UI_H

#include <SDL.h>
#include <SDL_draw.h>
#include <SDL_ttf.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <fstream>
#include <stdio.h>

#define VERTICAL true
#define HORIZONTAL false

class Field{
    public:
        Field(int x, int y, int w, int h, Uint32 bg_color, int r, int g, int b);
        virtual ~Field();
        void SetInfo(Uint16* new_info);
        virtual void Draw(SDL_Surface *display, TTF_Font *font, int allign, int num = -1);

    protected:
        int x;
        int y;
        int w;
        int h;
        Uint32 bg_color;
        SDL_Color font_color;
        Uint16* info;
};

class Button: public Field{
    public:
        Button(int x, int y, int w, int h, Uint32 bg_color, int r, int g, int b);
        ~Button();
        bool MouseOver(int m_x, int m_y);
        void Focus();
        void UnFocus();

    private:

};

class TextField: public Field{
    public:
        TextField(int x, int y, int w, int h, Uint32 bg_color, int r, int g, int b);
        ~TextField();
        Uint16* Input(SDL_Surface *display, TTF_Font *font);

    private:
        Uint16* hint;


};

class Menu: public Button{
    private:
        std::vector<Button*>    buttons;
        int                     last_focused;

    public:
        Menu(char *path, int b_w, int b_h, int x, int y, int w, int h, Uint32 bg_color, int r, int g, int b, bool orient);
        ~Menu();
        void Draw(SDL_Surface *display, TTF_Font *font, int key);
        int GetKey(int m, bool orient);
};
#endif // UI_H
