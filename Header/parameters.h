#pragma once
#define MAX_PARTICLES 3200
#define CIRCLESIZE 5
#define NOOFTRIANGLES 30
#define PI std::acos(-1)
#define TIMESTEP 0.025f
#define MAX_CIRCLES 3200
#define MAX_GRAVITY_CIRCLES 3200
#define FULLSCREEN false

#if FULLSCREEN
#define SCREEN_HEIGHT 1080
#define SCREEN_WIDTH 1980
#define SIM_VIEW_HEIGHT 1080 
#define SIM_VIEW_WIDTH 1600
#define UI_VIEW_WIDTH 320
#define UI_VIEW_HEIGHT 1080
#else 
#define SCREEN_HEIGHT 1057
#define SCREEN_WIDTH 1980
#define SIM_VIEW_HEIGHT 1057 
#define SIM_VIEW_WIDTH 1600
#define UI_VIEW_WIDTH 320
#define UI_VIEW_HEIGHT 1057
#endif
