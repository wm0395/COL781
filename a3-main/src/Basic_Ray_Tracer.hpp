#include <string>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

class Ray_Tracer{
    public:
    bool initialize(const std::string &title, int width, int height, int spp);
    bool

    private:
    SDL_Window *window;
    bool quit;
    SDL_Surface* framebuffer = NULL;
    SDL_Surface *windowSurface = NULL; 
    int frameWidth;
    int frameHeight;
    int displayScale;
    int spp; // Samples-per-pixel

};