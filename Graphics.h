#pragma once
#include <SDL2/SDL.h>
#include "Chip8.h"
#include <iostream>
#include <cstdint>
#include <array>

class Graphics {
public:
    Graphics(char const* title, int windowHeight, int windowLength, int pixelWidth, int pixelHeight);
    ~Graphics();
    auto ReadInput(std::array<uint8_t, NUMBER_OF_KEYS>& keys) -> bool;
    auto Update(const std::array<uint8_t, 64 * 32>& displayBuffer) -> void;

private:
    SDL_Window* m_window{};
    SDL_Renderer* m_renderer{};
    int m_pixelWidth;
    int m_pixelHeight;
};
