#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

struct Vector {
    float x;
    float y;
    float x_pr;
    float y_pr;
    float angle_speed;
};

void updateVector(Vector* vector);
void drawVector(SDL_Renderer* renderer, Vector vec);
void drawVector(SDL_Renderer* renderer, float x, float y, float x_pr, float y_pr);