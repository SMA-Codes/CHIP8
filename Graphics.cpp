#include "Graphics.h"
#include <SDL2/SDL.h>
#include <iostream>
#include "Chip8.h"
#include <unordered_map>

Graphics::Graphics(char const* title, int windowHeight, int windowLength, int pixelWidth, int pixelHeight)
    : m_pixelWidth(pixelWidth), m_pixelHeight(pixelHeight) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowLength, windowHeight, SDL_WINDOW_SHOWN);
    if (!m_window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        std::exit(EXIT_FAILURE);
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (!m_renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        std::exit(EXIT_FAILURE);
    }
}

Graphics::~Graphics() {
    if (m_renderer) SDL_DestroyRenderer(m_renderer);
    if (m_window) SDL_DestroyWindow(m_window);
    SDL_Quit();
}

auto Graphics::Update(const std::array<uint8_t, 64 * 32>& displayBuffer) -> void {
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255); 

    for (int y = 0; y < 32; ++y) {
        for (int x = 0; x < 64; ++x) {
            if (displayBuffer[y * 64 + x]) {
                SDL_Rect rect = {x * m_pixelWidth, y * m_pixelHeight, m_pixelWidth, m_pixelHeight};
                SDL_RenderFillRect(m_renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(m_renderer);
}

auto Graphics::ReadInput(std::array<uint8_t, NUMBER_OF_KEYS>& keys) -> bool {
    bool exit = false;
    SDL_Event event;

    const std::unordered_map<SDL_Keycode, uint8_t> keyMapping = {
        {SDLK_x, 0x0}, {SDLK_1, 0x1}, {SDLK_2, 0x2}, {SDLK_3, 0x3},
        {SDLK_q, 0x4}, {SDLK_w, 0x5}, {SDLK_e, 0x6}, {SDLK_a, 0x7},
        {SDLK_s, 0x8}, {SDLK_d, 0x9}, {SDLK_z, 0xA}, {SDLK_c, 0xB},
        {SDLK_4, 0xC}, {SDLK_r, 0xD}, {SDLK_f, 0xE}, {SDLK_v, 0xF}
    };

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit = true;
                break;

            case SDL_KEYDOWN: {
                auto keyPressed = keyMapping.find(event.key.keysym.sym);
                if (keyPressed != keyMapping.end()) {
                    keys[keyPressed->second] = 1; 
                }
                break;
            }

            case SDL_KEYUP: {
                auto keyPressed = keyMapping.find(event.key.keysym.sym);
                if (keyPressed != keyMapping.end()) {
                    keys[keyPressed->second] = 0; 
                }
                break;
            }
        }
    }

    return exit;
}
