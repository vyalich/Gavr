#include "UI.h"

Field::Field(int x, int y, int w, int h, Uint32 bg_color, int r, int g, int b): x(x), y(y), w(w), h(h), bg_color(bg_color) {
    font_color.r = r;
    font_color.g = g;
    font_color.b = b;
    info = new Uint16 [32];
    for(int i = 0; i < 32; i++)
        info[i] = 0;
}

Field::~Field(){
    delete [] info;
}

void Field::Draw(SDL_Surface *display, TTF_Font *font, int allign, int num){
    if(bg_color){
        Draw_FillRound(display, x, y, w, h, 5, 0x000000);
        Draw_FillRound(display, x+2, y+2, w-4, h-4, 5, bg_color);
    }

    SDL_Surface *message;
    if(num != -1){
        char s[6];
        sprintf(s, "%d", num);
        message = TTF_RenderText_Blended( font, s, font_color );
    }
    else{
        message = TTF_RenderUNICODE_Blended( font, info, font_color );
    }

    SDL_Rect dest;
    dest.y = y;
    switch(allign){
        case -1:
            dest.x = x;
            break;
        case 0:
            dest.x = x + w/2 - message->w/2;
            break;
        case 1:
            dest.x = x+w-1 - message->w;
            break;
    }
    if(message){
        SDL_BlitSurface(message, NULL, display, &dest);
        SDL_FreeSurface(message);
        message = NULL;
    }
}

void Field::SetInfo(Uint16* new_info){
    int i = 0;
    do{
        info[i] = new_info[i];
        i++;
    }while(new_info[i-1] != 0);
}

Button::Button(int x, int y, int w, int h, Uint32 bg_color, int r, int g, int b): Field(x, y, w, h, bg_color, r, g, b) {

}

Button::~Button(){

}

bool Button::MouseOver(int m_x, int m_y){
    if(m_x >= x && m_x <= x+w-1 && m_y >= y && m_y <= y+h-1)
        return true;
    return false;
}

void Button::Focus(){
    Uint16 r, g, b;
    r = bg_color/0x10000;
    g = bg_color/0x100%0x100;
    b = bg_color%0x100;
    if(r)
        r += 0x3a;
    if(g)
        g += 0x3a;
    if(b)
        b += 0x3a;
    bg_color = r*0x10000 + g*0x100 + b;
}

void Button::UnFocus(){
    Uint16 r, g, b;
    r = bg_color/0x10000;
    g = bg_color/0x100%0x100;
    b = bg_color%0x100;
    if(r)
        r -= 0x3a;
    if(g)
        g -= 0x3a;
    if(b)
        b -= 0x3a;
    bg_color = r*0x10000 + g*0x100 + b;
}

TextField::TextField(int x, int y, int w, int h, Uint32 bg_color, int r, int g, int b): Field(x, y, w, h, bg_color, r, g, b) {
    hint = new Uint16 [12];
    Uint16 tmp[] = {0x412, 0x432, 0x435, 0x434, 0x438, 0x442, 0x435, 0x20, 0x438, 0x43C, 0x44F};
    for(int i = 0; i < 11; i++)
        hint[i] = tmp[i];
    hint[11] = 0;
}

TextField::~TextField(){
    delete [] hint;
}

Uint16* TextField::Input(SDL_Surface *display, TTF_Font *font){
    SDL_Event event;
    int Max = 32;
    int length = 0;
    for(int i = 0; i < 11; i++)
        info[i] = hint[i];
    info[11] = 0;
    SDL_EnableUNICODE( SDL_ENABLE );

    Draw(display, font, 0, true);
    for(int i = 0; i < 11; i++)
        info[i] = 0;
    SDL_Flip(display);
    while(true){
        while(SDL_WaitEvent(&event)){
            if( event.type == SDL_KEYDOWN ){
                if( length <= Max ){
                    if( ( event.key.keysym.unicode >= 0x0030 ) && ( event.key.keysym.unicode <= 0x0039 ) ||
                        ( event.key.keysym.unicode >= 0x0041 ) && ( event.key.keysym.unicode <= 0x005a ) ||
                        ( event.key.keysym.unicode >= 0x0061 ) && ( event.key.keysym.unicode <= 0x007a ) ||
                        ( event.key.keysym.unicode >= 0x0410 ) && ( event.key.keysym.unicode <= 0x044F ) ||
                        ( event.key.keysym.unicode == 0x0401 ) || ( event.key.keysym.unicode == 0x0451 ) ||
                        ( event.key.keysym.unicode == 0x005f ) || ( event.key.keysym.unicode == 0x002d ) ){

                        info[length] = event.key.keysym.unicode;
                        length++;
                    }

                }

                if( event.key.keysym.sym == SDLK_BACKSPACE && length ){
                    info[--length] = 0;
                }
                if( event.key.keysym.sym == SDLK_RETURN && length ){
                    SDL_EnableUNICODE( SDL_DISABLE );
                    Uint16 *tmp = new Uint16 [length + 1];
                    for(int i = 0; i < length; i++)
                        tmp[i] = info[i];
                    tmp[length] = 0;
                    return tmp;
                }
            }
            if(!length){
                for(int i = 0; i < 11; i++)
                    info[i] = hint[i];
                Draw(display, font, 0);
                SDL_Flip(display);
                for(int i = 0; i < 32; i++)
                    info[i] = 0;
            }
            else{
                Draw(display, font, 0);
                SDL_Flip(display);
            }
        }
    }
}

Menu::Menu(char *path, int b_w, int b_h, int x, int y, int w, int h, Uint32 bg_color, int r, int g, int b, bool orient): Button(x, y, w, h, bg_color, r, g, b) {
    std::ifstream in(path, std::ios::binary);
    Uint16 *text;
    Button *tmp;
    int j = 0;
    while(!in.eof()){
        text = new Uint16 [32];
        if(orient)
            tmp = new Button(x, y+b_h*j, w, b_h, bg_color, r, g, b);
        else
            tmp = new Button(x+b_w*j, y, b_w, h, bg_color, r, g, b);
        for(int i = 0; i < 32; i++)
            text[i] = 0;

        int i = 0;
        do{
            in.read((char*)(text+i), sizeof(Uint16));
            i++;
        }while(text[i-1] != 0xD && !in.eof());
        text[i-1] = 0;

        tmp->SetInfo(text);
        delete [] text;
        buttons.push_back(tmp);
        j++;
    }
    last_focused = -1;
}

Menu::~Menu(){
    for(int i = 0; i < buttons.size(); i++)
        delete buttons[i];
    buttons.clear();
}

void Menu::Draw(SDL_Surface *display, TTF_Font *font, int key){
    if(key != last_focused){
        if(last_focused != -1)
            buttons[last_focused]->UnFocus();
        last_focused = key;
        if(key != -1)
            buttons[key]->Focus();
    }
    for(int i = 0; i < buttons.size(); i++)
        buttons[i]->Draw(display, font, 0);
    SDL_Flip(display);
}

int Menu::GetKey(int m, bool orient){
    if(orient)
        return (m - y) / (h / buttons.size());
    return (m - x) / (w / buttons.size());
}
