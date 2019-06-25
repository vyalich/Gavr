#include "App.h"

bool Greater(User *A, User *B){
    return A->points > B->points;
}

User::User(){
    name = new Uint16[32];
    for(int i = 0; i < 32; i++)
        name[i] = 0;
}

User::User(User &tmp){
    points = tmp.points;
    name = new Uint16[32];
    for(int i = 0; i < 32; i++)
        name[i] = 0;
    for(int i = 0; tmp.name[i]; i++)
        name[i] = tmp.name[i];
}

User::~User(){
    delete [] name;
}

App::App()
{
    //ctor
}

App::~App()
{
    //dtor
}

bool App::Init(){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        return false;
    atexit(SDL_Quit);

    if(TTF_Init() < 0)
        return false;
    atexit(TTF_Quit);

    display = SDL_SetVideoMode(800,600,32, SDL_ANYFORMAT | SDL_DOUBLEBUF | SDL_HWSURFACE | SDL_FULLSCREEN);
    if (!display)
        return false;

    //SDL_EnableKeyRepeat(1, SDL_DEFAULT_REPEAT_INTERVAL / 3);
    background = SDL_LoadBMP("./img/bg.bmp");
    SDL_BlitSurface(background, 0, display, 0);

    SDL_Flip(display);
    SDL_Delay(5000);

    running = true;

    font = TTF_OpenFont("./fonts/CharisSILR.ttf", 25);
    TextField Nickname(display->w/2 - 150, display->h/3*2, 300, 40, 0xffffff, 0, 0, 0);
    delete [] current.name;
    current.name = Nickname.Input(display, font);

    std::ifstream in("./data/results.dat", std::ios::binary);
    if(in){
        in.seekg(in.beg);
        while(!in.eof()){
            User *tmp = new User;
            in.read((char*)&(tmp->points), sizeof(int));
            for(int i = 0; !in.eof(); i++){
                in.read((char*)(tmp->name + i), sizeof(Uint16));
                if(tmp->name[i] == 0xd){
                    tmp->name[i] = 0;
                    break;
                }
            }
            if(tmp->name[0])
                results.push_back(tmp);
            else
                delete tmp;
        }
    }
    in.close();

    return true;
}

void App::Execute(){
    Menu main_menu("./data/menu.dat", 300, 40, display->w/2-150, display->h/2, 300, 200, 0x939393, 0, 0, 0, VERTICAL);

    while(running){
        SDL_BlitSurface(background, 0, display, 0);
        int x, y;
        SDL_GetMouseState(&x, &y);
        int key = main_menu.GetKey(y, VERTICAL);
        if(main_menu.MouseOver(x, y))
            main_menu.Draw(display, font, key);
        else
            main_menu.Draw(display, font, -1);
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && main_menu.MouseOver(x, y)){
                switch(key){
                    case 0:{
                        Game m_Game;
                        if(m_Game.Init(current.name))
                            current.points = m_Game.MainCycle(display, font);
                        m_Game.Clean();
                        if(current.points){
                            User *tmp = new User (current);
                            results.push_back(tmp);
                            std::sort(results.begin(), results.end(), Greater);
                            if(results.size()>10){
                                delete results[10];
                                results.pop_back();
                            }
                        }
                        break;
                    }
                    case 1:{
                        SDL_BlitSurface(background, 0, display, 0);
                        TextField Nickname(display->w/2 - 150, display->h/3*2, 300, 40, 0xffffff, 0, 0, 0);
                        delete [] current.name;
                        current.name = Nickname.Input(display, font);
                        break;
                    }
                    case 2:{
                        SDL_BlitSurface(background, 0, display, 0);
                        Field res(display->w/2-200, 40, 400, 460, 0xf0f0f0, 0, 0, 0);
                        Uint16 txt[7] = {0x0422, 0x043e, 0x043f, 0x20, 49, 48, 0};
                        res.SetInfo(txt);
                        res.Draw(display, font, 0);
                        for(int i = 0; i < results.size(); i++){
                            Field name(display->w/2-190, 90+40*i, 300, 40, 0, 0, 0, 0);
                            name.SetInfo(results[i]->name);
                            Field points(display->w/2+100, 90+40*i, 90, 40, 0, 0, 0, 0);
                            name.Draw(display, font, -1);
                            points.Draw(display, font, 1, results[i]->points);
                        }
                        Menu res_menu("./data/res-menu.dat", 150, 40, display->w/2-150, 510, 300, 40, 0x939393, 0, 0, 0, HORIZONTAL);
                        bool flag = true;
                        while(flag){
                            int x, y;
                            SDL_GetMouseState(&x, &y);
                            int res_key = res_menu.GetKey(x, HORIZONTAL);
                            if(res_menu.MouseOver(x, y))
                                res_menu.Draw(display, font, res_key);
                            else
                                res_menu.Draw(display, font, -1);
                            while(SDL_PollEvent(&event)){
                                if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && res_menu.MouseOver(x, y)){
                                    if(res_key == 0){
                                        for(int i = 0; i < results.size(); i++)
                                            delete results[i];
                                        results.clear();
                                        res.Draw(display, font, 0);
                                        SDL_Flip(display);
                                    }
                                    else if(res_key == 1){
                                        flag = false;
                                    }
                                }
                            }
                        }
                        break;
                    }
                    case 3:{
                        SDL_Surface *rules = SDL_LoadBMP("./img/rules.bmp");
                        SDL_BlitSurface(rules, 0, display, 0);
                        Button back(display->w/2-150, 530, 300, 40, 0x939393, 0, 0, 0);
                        Uint16 tmp[10] = {0x041d, 0x0430, 0x0437, 0x0430, 0x0434, 0};
                        back.SetInfo(tmp);
                        bool flag = true, focused = false;
                        while(flag){
                            int x, y;
                            SDL_GetMouseState(&x, &y);
                            if(back.MouseOver(x, y) && !focused){
                                back.Focus();
                                focused = true;
                            }
                            else if(!back.MouseOver(x, y) && focused){
                                back.UnFocus();
                                focused = false;
                            }
                            back.Draw(display, font, 0);
                            SDL_Flip(display);
                            while(SDL_PollEvent(&event)){
                                if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && back.MouseOver(x, y))
                                    flag = false;
                            }
                        }
                        SDL_FreeSurface(rules);
                        break;
                    }
                    case 4:{
                        running = false;
                        break;
                    }
                }
            }
        }
    }
}

void App::Clean(){
    std::ofstream out("./data/results.dat", std::ios::binary);
    if(out){
        for(int i = 0; i < results.size(); i++){
            out.write((char*)&(results[i]->points), sizeof(results[i]->points));
            for(int j = 0; results[i]->name[j]; j++)
                out.write((char*)((results[i]->name) + j), sizeof(Uint16));
            Uint16 c = 0xd;
            out.write((char*)&c, sizeof(Uint16));
        }
    }
    out.close();
    for(int i = 0; i < results.size(); i++)
        delete results[i];
    results.clear();
    TTF_CloseFont(font);
    SDL_FreeSurface(display);
    TTF_Quit();
    SDL_Quit();
}
