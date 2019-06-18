#ifndef UI_H
#define UI_H

#include "object.h"
#include <string>

class Field{
    public:
        Field(int x, int y, int w, int h, int bg_color, int r, int g, int b): x(x), y(y), w(w), h(h), bg_color(bg_color), info("") {
            font_color.r = r, font_color.g = g, font_color.b = b;
        }
        virtual ~Field();
        void SetInfo(char *new_info);
        void Draw(SDL_Surface *display, TTF_Font *font);

    protected:
        int x;
        int y;
        int w;
        int h;
        Uint32 bg_color;
        SDL_Color font_color;
        std::string info;
};

class Button: public Field{
    public:
        Button(int x, int y, int w, int h, Uint32 bg_color, int r, int g, int b): Field(x, y, w, h, bg_color, r, g, b) {};
        ~Button();
        bool MouseOver(int m_x, int m_y);
        void Focus();
        void UnFocus();

    private:

};

class TextField: public Field{
    public:
        TextField(int x, int y, int w, int h, Uint32 bg_color, int r, int g, int b): Field(x, y, w, h, bg_color, r, g, b), hint("Enter name.") {};
        ~TextField();
        std::string Input(SDL_Surface *display, TTF_Font *font);

    private:
        std::string hint;


};
#endif // UI_H
