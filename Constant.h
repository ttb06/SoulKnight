#pragma once

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 675;

const int FPS = 60;
const int frameDelay = 1000/FPS;

const int LEVEL_WIDTH = 16000;
const int LEVEL_HEIGHT = 16000;
const int LV1_SIZE_X = 60;//for map 60 x 60
const int LV1_SIZE_Y = 60;
const float SMOOTHING_FACTOR = 0.05;


const int moveX[] = {0, 0, 1, -1};
const int moveY[] = {1, -1, 0, 0};

const int maxN = 105;

const int IMMORTAL_TIME = 500; 
//max distance to recognize enermies cal by tile set (*16*scale)
const int MAX_DISTANCE_FROM_ENERMY_TILE = 60;
const int RANGE_MELE_WEAPON = 20;

const int TILE_ANIM_FRAMES = 3;
const int TILE_ANIM_SPEED = 100;

enum bfs
{
    dest = -2,
    start = -1,
    block = 10
};