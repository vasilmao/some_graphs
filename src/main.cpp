#include "App.h"

const int WIDTH  = 800;
const int HEIGHT = 600;

int main() {
    App app = {};
    initApp(&app, WIDTH, HEIGHT);
    runApp(&app);
    destroyApp(&app);
}