//
// Created by junds on 12-11-2021.
//

#pragma once

#include <string>
#include "C:\repos\WDNES\cmake-build-debug\_deps\sdl2-src\include\SDL.h"

class Canvas {
public:
    Canvas(std::string const& title, int width, int height, int x = SDL_WINDOWPOS_CENTERED, int y = SDL_WINDOWPOS_CENTERED);
    ~Canvas();

    /**
     * @param pixels pixel format RGB888
     */
    void drawFrame(uint32_t* pixels);
    bool processInput();
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* frameBuffer;
    SDL_Event event;
    int pitch;
};



