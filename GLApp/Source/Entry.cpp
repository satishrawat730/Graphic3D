#define STB_IMAGE_IMPLEMENTATION

#include "RocketEngine.h"

using namespace RE;
// -- MAIN --------
int main()
{
    RocketEngine *rocketApp = new RocketEngine();
    rocketApp->Intialize();
    rocketApp->Run();


    rocketApp->Release();
    delete rocketApp;
    return 0;
}