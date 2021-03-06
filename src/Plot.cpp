#include "Plot.h"

const int POINT_CNT = 100;

void updateVector(Vector* vector);
void drawVector(SDL_Renderer* renderer, Vector vec);
void drawVector(SDL_Renderer* renderer, float x, float y, float x_pr, float y_pr);
void preRenderPlot(SDL_Renderer* renderer, Plot* plot);
void getPixels(float x, float y, CoordSystem coords, float* pixel_x, float* pixel_y);

void initPlot(SDL_Renderer* render, Plot* plot, CoordSystem coords, float (*function)(float x)) {
    plot->coords = coords;
    printf("%d %d\n", plot->coords.pixel_width, plot->coords.pixel_height);
    plot->pre_rendered_texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, coords.pixel_width, coords.pixel_height);
    plot->texture_with_vectors = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, coords.pixel_width, coords.pixel_height);
    plot->function = function;
    preRenderPlot(render, plot);

}

void preRenderPlot(SDL_Renderer* renderer, Plot* plot) {
    // prepare axis
    SDL_SetRenderTarget(renderer, plot->pre_rendered_texture);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    float zero_pixel_x = 0;
    float zero_pixel_y = 0;
    getPixels(0, 0, plot->coords, &zero_pixel_x, &zero_pixel_y);
    // horizontal
    drawVector(renderer, 0, zero_pixel_y, plot->coords.pixel_width, 0);
    // vertical
    drawVector(renderer, zero_pixel_x, plot->coords.pixel_height, 0, -plot->coords.pixel_height);

    //draw function
    SDL_FPoint points[POINT_CNT];
    float point_step = (plot->coords.x2 - plot->coords.x1) / POINT_CNT;
    float current_x = plot->coords.x1;
    for (int i = 0; i < POINT_CNT; ++i, current_x += point_step) {
        float pixel_x = 0;
        float pixel_y = 0;
        float current_y = plot->function(current_x);
        getPixels(current_x, current_y, plot->coords, &pixel_x, &pixel_y);
        points[i] = {pixel_x, pixel_y};
    }
    SDL_RenderDrawLinesF(renderer, points, POINT_CNT);
}

void getPixels(float x, float y, CoordSystem coords, float* pixel_x, float* pixel_y) {
    float delta_x = x - coords.x1;
    float delta_y = y - coords.y1;
    *pixel_x = delta_x * coords.pixel_width / (coords.x2 - coords.x1);
    *pixel_y = coords.pixel_height - delta_y * coords.pixel_height / (coords.y2 - coords.y1); // (coords.pixel_height - ...)because y counts for top
}

void updatePlot(Plot* plot) {
    for (int i = 0; i < plot->vectors_cnt; ++i) {
        updateVector(plot->vectors + i);
    }
}

void renderPlot(SDL_Renderer* renderer, Plot* plot, int x, int y) {
    // draw plot without vectors
    SDL_SetRenderTarget(renderer, plot->texture_with_vectors);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderCopy(renderer, plot->pre_rendered_texture, NULL, NULL);
    // draw vectors
    for (int i = 0; i < plot->vectors_cnt; ++i) {
        float v_pixel_x1 = 0;
        float v_pixel_y1 = 0;
        float v_pixel_x2 = 0;
        float v_pixel_y2 = 0;
        getPixels(plot->vectors[i].x, plot->vectors[i].y, plot->coords, &v_pixel_x1, &v_pixel_y1);
        getPixels(plot->vectors[i].x + plot->vectors[i].x_pr, plot->vectors[i].y + plot->vectors[i].y_pr, plot->coords, &v_pixel_x2, &v_pixel_y2);
        printf("%f %f %f %f\n", v_pixel_x1, v_pixel_y1, v_pixel_x2, v_pixel_y2);
        drawVector(renderer, v_pixel_x1, v_pixel_y1, v_pixel_x2 - v_pixel_x1, v_pixel_y2 - v_pixel_y1);
    }
    // draw plot to window
    SDL_SetRenderTarget(renderer, NULL);
    SDL_Rect plot_rect = {x, y, plot->coords.pixel_width, plot->coords.pixel_height};
    SDL_RenderCopy(renderer, plot->texture_with_vectors, NULL, &plot_rect);
}

void destroyPlot(Plot* plot) {
    if (plot->vectors != NULL) {
        free(plot->vectors);
    }
    SDL_DestroyTexture(plot->pre_rendered_texture);
    SDL_DestroyTexture(plot->texture_with_vectors);
    free(plot);
}

void addVector(Plot* plot, Vector vector) {
    if (plot->vectors_capacity == 0) {
        plot->vectors_capacity = 10;
        plot->vectors = (Vector*)calloc(plot->vectors_capacity, sizeof(Vector));
    }
    if (plot->vectors_capacity == plot->vectors_cnt) {
        plot->vectors_capacity *= 2;
        plot->vectors = (Vector*)realloc(plot->vectors, plot->vectors_capacity * sizeof(Vector));
    }
    plot->vectors[plot->vectors_cnt++] = vector;
}