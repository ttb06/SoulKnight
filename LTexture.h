#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "string.h"

using std::string;

class LTexture
{
public:
    enum
    {
        RED_TEXT = 0,
        WHITE_TEXT = 1,
        BLACK_TEXT = 2,
    };
    // Initializes variables
    LTexture();

    // Deallocates memory
    ~LTexture();

    // Loads image at specified path
    bool loadFromFileee(std::string path);

    // Creates image from font string
    // bool loadFromRenderedText(std::string textureText, SDL_Color textColor);

    // Deallocates texture
    void free();

    // Set color modulation
    void SetColor(const int &type);

    // Set blending
    SDL_Color GetColor()
    {
        return text_color;
    }

    void confirmOx_Oy(int x, int y)
    {
        Ox = x;
        Oy = y;
    }
    int getOx() { return Ox; }
    int getOy() { return Oy; }
    SDL_Rect getRect() { return hcn; }
    void SetRect()
    {
        hcn.x = Ox;
        hcn.y = Oy;
        hcn.w = mWidth;
        hcn.h = mHeight;
    }

    // Renders texture at given point
    void render(int x, int y, SDL_Rect *clip = NULL, double angle = 0.0, SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

    // Gets image dimensions
    int getWidth();
    int getHeight();
    SDL_Texture *texture;
    int mWidth;
    int mHeight;

private:
    // The actual hardware texture
    SDL_Color text_color;
    SDL_Texture *mTexture;

    // Image dimensions
    int Ox, Oy;
    SDL_Rect hcn;
};
