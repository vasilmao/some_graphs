#include "App.h"

const int WIDTH  = 800;
const int HEIGHT = 600;

int main() {
    App app = {};
    int result = initApp(&app, WIDTH, HEIGHT);
    switch (result)
    {
    case CANT_INITIALIZE_SDL:
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        return 0;
    case CANT_CREATE_WINDOW:
        printf("Failed to open window: %s\n", SDL_GetError());
        return 0;
    case CANT_CREATE_RENDERER:
        printf("Failed to create renderer: %s\n", SDL_GetError());
        return 0;
    default:
        break;
    }

    addVector(app.plot_big, {1, 1, 0, 9, 0.1});
    addVector(app.plot_small, {0, 0, 3, 0, 0.1});

    runApp(&app);
    destroyApp(&app);
}