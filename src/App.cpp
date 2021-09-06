#include "App.h"

void parseEvent(App* app, SDL_Event event);

const SDL_Color BG_COLOR = {185, 226, 235, 255};

const float SMALL_PLOT_X_MIN = -8.0;
const float SMALL_PLOT_X_MAX = 12.0;
const float SMALL_PLOT_Y_MIN = -3.0;
const float SMALL_PLOT_Y_MAX =  5.0;

const int SMALL_PLOT_SCREEN_X       = 450;
const int SMALL_PLOT_SCREEN_WIDTH   = 200;
const int SMALL_PLOT_SCREEN_Y       = 0;
const int SMALL_PLOT_SCREEN_HEIGHT  = 150;

const float BIG_PLOT_X_MIN   = -10.0;
const float BIG_PLOT_X_MAX   =  10.0;
const float BIG_PLOT_Y_MIN   = -10.0;
const float BIG_PLOT_Y_MAX   =  10.0;

const int BIG_PLOT_SCREEN_X       = 0;
const int BIG_PLOT_SCREEN_WIDTH   = 400;
const int BIG_PLOT_SCREEN_Y       = 0;
const int BIG_PLOT_SCREEN_HEIGHT  = 300;

void initPlot(SDL_Renderer* render, Plot* plot, CoordSystem coords, float (*function)(float x));
void preRenderPlot(SDL_Renderer* renderer, Plot* plot);
float getPixels(float x, float y, CoordSystem coords, float* pixel_x, float* pixel_y);
void drawVector(SDL_Renderer* renderer, Vector vec);
void drawVector(SDL_Renderer* renderer, float x, float y, float x_pr, float y_pr);
void runApp(App* app);
void updateApp(App* app);
void updatePlot(Plot* plot);
void updateVector(Vector* vector);
void renderPlot(SDL_Renderer* renderer, Plot* plot, int x, int y);
void renderApp(App* app);
void destroyPlot(Plot* plot);

float function_to_draw(float x) {
    return sin(x);
}


int initApp(App* app, int width, int height) {
    app->width  = width;
    app->height = height;

    int rendererFlags = SDL_RENDERER_ACCELERATED;
    int windowFlags = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return CANT_INITIALIZE_SDL;
	}

    app->window = SDL_CreateWindow("Patcher", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, app->width, app->height, windowFlags);
	if (!app->window) {
		return CANT_CREATE_WINDOW;
	}

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    app->renderer = SDL_CreateRenderer(app->window, -1, rendererFlags);
	if (!app->renderer) {
		return CANT_CREATE_RENDERER;
	}

    app->plot_big = (Plot*)calloc(1, sizeof(Plot));
    app->plot_small = (Plot*)calloc(1, sizeof(Plot));

    CoordSystem big_chart_coords   = {BIG_PLOT_X_MIN, 
                                      BIG_PLOT_Y_MIN, 
                                      BIG_PLOT_X_MAX, 
                                      BIG_PLOT_Y_MAX, 
                                      BIG_PLOT_SCREEN_WIDTH, 
                                      BIG_PLOT_SCREEN_HEIGHT};

    CoordSystem small_chart_coords = {SMALL_PLOT_X_MIN, 
                                      SMALL_PLOT_Y_MIN, 
                                      SMALL_PLOT_X_MAX, 
                                      SMALL_PLOT_Y_MAX, 
                                      SMALL_PLOT_SCREEN_WIDTH, 
                                      SMALL_PLOT_SCREEN_HEIGHT};


    initPlot(app->renderer, app->plot_big, big_chart_coords, function_to_draw);
    initPlot(app->renderer, app->plot_small, small_chart_coords, function_to_draw);

    app->bg_color = BG_COLOR;
    SDL_SetRenderTarget(app->renderer, NULL);

    return SDL_INIT_OK;
}

void runApp(App* app) {
    app->running = true;
    SDL_Event event;
    while (app->running) {
        clock_t t = clock();
        while (SDL_PollEvent(&event)) {
            parseEvent(app, event);
        }
        updateApp(app);
        
        renderApp(app);
        printf("%lf\n", CLOCKS_PER_SEC / ((double)(clock() - t)));
        SDL_Delay(10);
        break;
    }
}

void updateApp(App* app) {
    updatePlot(app->plot_big);
    updatePlot(app->plot_small);
}

#define OPEN_RGBA(x) x.r, x.g, x.b, x.a

void renderApp(App* app) {
    SDL_SetRenderTarget(app->renderer, NULL);
    //SDL_SetRenderDrawColor(app->renderer, app->bg_color.r, app->bg_color.g, app->bg_color.b, app->bg_color.a);
    SDL_SetRenderDrawColor(app->renderer, OPEN_RGBA(app->bg_color));
    SDL_RenderClear(app->renderer);

    renderPlot(app->renderer, app->plot_big, 0, 0);
    renderPlot(app->renderer, app->plot_small, 450, 0);

    SDL_RenderPresent(app->renderer);
}

void parseEvent(App* app, SDL_Event event) {
    if (event.type == SDL_QUIT) {
        app->running = false;
    }
}

void destroyApp(App* app) {
    destroyPlot(app->plot_big);
    destroyPlot(app->plot_small);
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);
    return;
}
