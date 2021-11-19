//
// Created by junds on 12-11-2021.
//

#include "Canvas.h"

Canvas::Canvas(std::string const& title, int width, int height, int x, int y) : event() {
    window = SDL_CreateWindow(title.c_str(), x, y, width, height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    frameBuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, width, height);
    pitch = width * static_cast<int>(sizeof(uint32_t));
}

Canvas::~Canvas() {
    SDL_DestroyTexture(frameBuffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void Canvas::drawFrame(uint32_t* pixels) {
    SDL_UpdateTexture(frameBuffer, nullptr, pixels, pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, frameBuffer, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

bool Canvas::processInput() {
    bool quit = false;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            default:
                break;
        }
    }

    return quit;
}
