#include "Plot.h"

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

void initApp(App* app, int width, int height);
void runApp(App* app);
void destroyApp(App* app);