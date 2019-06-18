#include "App.h"

int main(int argc, char *argv[])
{
    App m_app;
    if(m_app.Init())
        m_app.Execute();
    m_app.Clean();

return 0;
}
