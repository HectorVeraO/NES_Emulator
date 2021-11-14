//
// Created by junds on 12-11-2021.
//

#pragma once

#include <string>
#include "C:\repos\WDNES\cmake-build-debug\_deps\sdl2-src\include\SDL.h"

class Canvas {
public:
    Canvas(std::string const& title, int width, int height, int x = SDL_WINDOWPOS_CENTERED, int y = SDL_WINDOWPOS_CENTERED) {
        window = SDL_CreateWindow(title.c_str(), x, y, width, height, SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
        frameBuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, width, height);
        pitch = width * static_cast<int>(sizeof(uint32_t));
    }

    ~Canvas() {
        SDL_DestroyTexture(frameBuffer);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
    }

    /**
     * @param pixels pixel format RGB888
     */
    void drawFrame(uint32_t* pixels) {
        SDL_UpdateTexture(frameBuffer, nullptr, pixels, pitch);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, frameBuffer, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* frameBuffer;
    int pitch;
};



