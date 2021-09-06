#include "Plot.h"

enum SDL_init_errors {
    SDL_INIT_OK,
    CANT_INITIALIZE_SDL,
    CANT_CREATE_WINDOW,
    CANT_CREATE_RENDERER
};

struct SpinningVector2d {
    float x;
    float y;
    float x_pr;
    float y_pr;

    float spinning_speed;
};

struct App {
    SDL_Renderer* renderer;
	SDL_Window*   window;
    bool   running;
    int width;
    int height;

    SDL_Color bg_color;

    Plot* plot_big;
    Plot* plot_small;

    int vectors_cnt;
    
};

int initApp(App* app, int width, int height);
void runApp(App* app);
void destroyApp(App* app);