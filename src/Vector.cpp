#include "Vector.h"

void updateVector(Vector* vector) {
    // just turn it
    float length = sqrt(vector->x_pr * vector->x_pr + vector->y_pr * vector->y_pr);
    float angle = atan2(vector->y_pr, vector->x_pr);
    angle += vector->angle_speed;
    vector->x_pr = length * cos(angle);
    vector->y_pr = length * sin(angle);
}

void drawVector(SDL_Renderer* renderer, Vector vec) {
    drawVector(renderer, vec.x, vec.y, vec.x_pr, vec.y_pr);
}

void drawVector(SDL_Renderer* renderer, float x, float y, float x_pr, float y_pr) {
    SDL_RenderDrawLineF(renderer, x, y, x + x_pr, y + y_pr);

    float angle = atan2(y_pr, x_pr);
    float length = sqrt(x_pr * x_pr + y_pr * y_pr);

    float angle_arrow_left  = angle + M_PI * 5 / 6; //90 + 60
    float angle_arrow_right = angle - M_PI * 5 / 6;
    float arrow_length = length / 10;
    float arrow_start_x = x + x_pr;
    float arrow_start_y = y + y_pr;

    float left_arrow_x2 = arrow_start_x + arrow_length * cos(angle_arrow_left);
    float left_arrow_y2 = arrow_start_y + arrow_length * sin(angle_arrow_left);
    SDL_RenderDrawLineF(renderer, arrow_start_x, arrow_start_y, left_arrow_x2, left_arrow_y2);

    float right_arrow_x2 = arrow_start_x + arrow_length * cos(angle_arrow_right);
    float right_arrow_y2 = arrow_start_y + arrow_length * sin(angle_arrow_right);
    SDL_RenderDrawLineF(renderer, arrow_start_x, arrow_start_y, right_arrow_x2, right_arrow_y2);
}
