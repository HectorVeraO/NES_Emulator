//
// Created by junds on 12-11-2021.
//

#include "Canvas.h"

Canvas::Canvas(std::string const& title, int width, int height, std::array<uint8_t, 2>* controllerState, int x, int y) : event() {
    window = SDL_CreateWindow(title.c_str(), x, y, width, height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    frameBuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, width, height);
    pitch = width * static_cast<int>(sizeof(uint32_t));
    this->controllerState = controllerState;
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
    static Uint8 const* keyboardState = SDL_GetKeyboardState(nullptr);

    bool quit = false;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
        }
    }

    // Fetch Controller_1 state
    (*controllerState)[0] = 0x00;
    (*controllerState)[0] |= ((keyboardState[SDL_SCANCODE_RIGHT] & 0x01) << 0);
    (*controllerState)[0] |= ((keyboardState[SDL_SCANCODE_LEFT] & 0x01) << 1);
    (*controllerState)[0] |= ((keyboardState[SDL_SCANCODE_DOWN] & 0x01) << 2);
    (*controllerState)[0] |= ((keyboardState[SDL_SCANCODE_UP] & 0x01) << 3);
    (*controllerState)[0] |= ((keyboardState[SDL_SCANCODE_C] & 0x01) << 4);     // Start
    (*controllerState)[0] |= ((keyboardState[SDL_SCANCODE_V] & 0x01) << 5);     // Select
    (*controllerState)[0] |= ((keyboardState[SDL_SCANCODE_X] & 0x01) << 6);     // B
    (*controllerState)[0] |= ((keyboardState[SDL_SCANCODE_Z] & 0x01) << 7);     // A
    // SDL_Log("Controller_1 state is %d", (*controllerState)[0]);
    // TODO: Find out why SDL_Log severely slows down the execution of the emulator.

    return quit;
}
