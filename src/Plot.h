#include "Vector.h"

struct TwoPoints {
    float x1;
    float y1;
    float x2;
    float y2;
};

struct CoordSystem {
    float x1;
    float y1;
    float x2;
    float y2;

    int pixel_width;
    int pixel_height;
};

struct Plot {
    Vector* vectors;
    int vectors_cnt;
    int vectors_capacity;

    SDL_Texture* pre_rendered_texture;
    SDL_Texture* texture_with_vectors;

    CoordSystem coords;

    float (*function)(float x);

};

void initPlot(SDL_Renderer* render, Plot* plot, CoordSystem coords, float (*function)(float x));
void updatePlot(Plot* plot);
void renderPlot(SDL_Renderer* renderer, Plot* plot, int x, int y);
void clearPlot(Plot* plot);
void addVector(Plot* plot, Vector vector);