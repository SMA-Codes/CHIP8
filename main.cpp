#include "Chip8.h"
#include "Graphics.h"
#include <iostream>
#include <chrono>
#include <array>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <ROM file>" << std::endl;
        return EXIT_FAILURE;
    }

    constexpr int windowWidth = 640;
    constexpr int windowHeight = 320;
    constexpr int pixelWidth = windowWidth / 64;
    constexpr int pixelHeight = windowHeight / 32;
    const char* romFile = argv[1];

    try {
        Graphics window("Chip-8 Emulator", windowHeight, windowWidth, pixelWidth, pixelHeight);
        Chip8 chip8Emulator;

        chip8Emulator.LoadRom(romFile);

        auto lastCycleTime = std::chrono::high_resolution_clock::now();
        constexpr int delayMs = 5;
        bool quit = false;

        while (!quit) {
            quit = window.ReadInput(chip8Emulator.m_inputKeys);

            auto currentTime = std::chrono::high_resolution_clock::now();
            auto elapsedTime = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();

            if (elapsedTime > delayMs) {
                lastCycleTime = currentTime;
                chip8Emulator.Cycle();
                window.Update(chip8Emulator.m_display);
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
