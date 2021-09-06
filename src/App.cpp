#include "App.h"

void parseEvent(App* app, SDL_Event event);

const float SMALL_CHART_X_MIN = -8.0;
const float SMALL_CHART_X_MAX = 12.0;
const float SMALL_CHART_Y_MIN = -3.0;
const float SMALL_CHART_Y_MAX =  5.0;

const int SMALL_CHART_SCREEN_X       = 450;
const int SMALL_CHART_SCREEN_WIDTH   = 200;
const int SMALL_CHART_SCREEN_Y       = 0;
const int SMALL_CHART_SCREEN_HEIGHT  = 150;

const float BIG_CHART_X_MIN   = -10.0;
const float BIG_CHART_X_MAX   =  10.0;
const float BIG_CHART_Y_MIN   = -10.0;
const float BIG_CHART_Y_MAX   =  10.0;

const int BIG_CHART_SCREEN_X       = 0;
const int BIG_CHART_SCREEN_WIDTH   = 400;
const int BIG_CHART_SCREEN_Y       = 0;
const int BIG_CHART_SCREEN_HEIGHT  = 300;

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
void clearPlot(Plot* plot);

float parabola(float x) {
    return x * x;
}


void initApp(App* app, int width, int height) {
    app->width  = width;
    app->height = height;

    int rendererFlags, windowFlags;
    rendererFlags = SDL_RENDERER_ACCELERATED;
    windowFlags = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

    app->window = SDL_CreateWindow("Patcher", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, app->width, app->height, windowFlags);
	if (!app->window) {
		printf("Failed to open %d x %d window: %s\n", app->width, app->height, SDL_GetError());
		exit(1);
	}

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    app->renderer = SDL_CreateRenderer(app->window, -1, rendererFlags);
	if (!app->renderer) {
		printf("Failed to create renderer: %s\n", SDL_GetError());
		exit(1);
	}

    app->plot_big = (Plot*)calloc(1, sizeof(Plot));
    app->plot_small = (Plot*)calloc(1, sizeof(Plot));

    CoordSystem big_chart_coords   = {BIG_CHART_X_MIN, 
                                      BIG_CHART_Y_MIN, 
                                      BIG_CHART_X_MAX, 
                                      BIG_CHART_Y_MAX, 
                                      BIG_CHART_SCREEN_WIDTH, 
                                      BIG_CHART_SCREEN_HEIGHT};

    CoordSystem small_chart_coords = {SMALL_CHART_X_MIN, 
                                      SMALL_CHART_Y_MIN, 
                                      SMALL_CHART_X_MAX, 
                                      SMALL_CHART_Y_MAX, 
                                      SMALL_CHART_SCREEN_WIDTH, 
                                      SMALL_CHART_SCREEN_HEIGHT};


    initPlot(app->renderer, app->plot_big, big_chart_coords, parabola);
    initPlot(app->renderer, app->plot_small, small_chart_coords, parabola);

    app->bg_color = {185, 226, 235, 255};
    SDL_SetRenderTarget(app->renderer, NULL);
    
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
        //break;
        SDL_Delay(10);
    }
}

void updateApp(App* app) {
    updatePlot(app->plot_big);
    updatePlot(app->plot_small);
}

void renderApp(App* app) {
    SDL_SetRenderTarget(app->renderer, NULL);
    SDL_SetRenderDrawColor(app->renderer, app->bg_color.r, app->bg_color.g, app->bg_color.b, app->bg_color.a);
    SDL_RenderClear(app->renderer);
    //printf("yeah\n");
    renderPlot(app->renderer, app->plot_big, 0, 0);
    renderPlot(app->renderer, app->plot_small, 450, 0);

    // SDL_Rect plot_rect = {0, 0, 400, 300};
    // SDL_SetRenderTarget(app->renderer, NULL);
    // SDL_RenderCopy(app->renderer, app->plot_big->pre_rendered_texture, NULL, &(plot_rect));
    SDL_RenderPresent(app->renderer);
}

void parseEvent(App* app, SDL_Event event) {
    if (event.type == SDL_QUIT) {
        app->running = false;
    }
}

void clearApp(App* app) {
    clearPlot(app->plot_big);
    clearPlot(app->plot_small);
    return;
}
