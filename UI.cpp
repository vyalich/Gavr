#include "UI.h"

Field::~Field(){

}

void Field::Draw(SDL_Surface *display, TTF_Font *font){
    if(bg_color)
        Draw_FillRect(display, x, y, w, h, bg_color);

    SDL_Surface *message;
    SDL_Rect dest = {.x = x, .y = y};

    message = TTF_RenderText_Solid( font, info.c_str(), font_color );
    if(message){
        SDL_BlitSurface(message, NULL, display, &dest);
        SDL_FreeSurface(message);
        message = NULL;
    }
}

Button::~Button(){

}

bool Button::MouseOver(int m_x, int m_y){

}

void Button::Focus(){

}

void Button::UnFocus(){

}

TextField::~TextField(){

}

std::string TextField::Input(SDL_Surface *display, TTF_Font *font){
    SDL_Event event;
    int Max = 12;

    SDL_EnableUNICODE( SDL_ENABLE );

    Draw(display, font);
    SDL_Flip(display);
    while(true){
        while(SDL_PollEvent(&event)){
            if( event.type == SDL_KEYDOWN ){
                if( info.length() <= Max ){
                    if( ( event.key.keysym.unicode >= (Uint16)'0' ) && ( event.key.keysym.unicode <= (Uint16)'9' ) ||
                        ( event.key.keysym.unicode >= (Uint16)'а' ) && ( event.key.keysym.unicode <= (Uint16)'я' ) ||
                        ( event.key.keysym.unicode >= (Uint16)'А' ) && ( event.key.keysym.unicode <= (Uint16)'Я' ) ||
                        ( event.key.keysym.unicode >= (Uint16)'a' ) && ( event.key.keysym.unicode <= (Uint16)'z' ) ||
                        ( event.key.keysym.unicode >= (Uint16)'A' ) && ( event.key.keysym.unicode <= (Uint16)'Z' ) )

                        info += (char)event.key.keysym.unicode;

                }

                if( ( event.key.keysym.sym == SDLK_BACKSPACE ) && ( info.length() != 0 ) ){
                    info.erase( info.length() - 1 );
                }
                if( ( event.key.keysym.sym == SDLK_RETURN ) && ( info.length() != 0 ) ){
                    SDL_EnableUNICODE( SDL_DISABLE );
                    return info;
                }
            }
            if(info.length() == 0){
                info = hint;
                Draw(display, font);
                SDL_Flip(display);
                info.erase();
            }
            else{
                Draw(display, font);
                SDL_Flip(display);
            }
        }
    }
}
