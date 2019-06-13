#include "Game.h"

int main(int argc, char *argv[])
{
    Game m_game;
    if(m_game.Init())
        m_game.MainCycle();
    m_game.Clean();

return 0;
}
