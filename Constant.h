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

enum bfs
{
    dest = -2,
    start = -1,
    block = 10
};