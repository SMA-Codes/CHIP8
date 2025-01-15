#pragma once

#include <cstdint>
#include <string>
#include <array>

constexpr int START_ADDRESS = 0x200;
constexpr int START = 0x0000;
constexpr int FONT_SET_START_ADDRESS = 0x50;
constexpr int NUMBER_OF_REGISTERS = 16;
constexpr int NUMBER_OF_BIT_VALUES = 16;
constexpr int NUMBER_OF_KEYS = 16;
constexpr int MEMORY_SIZE = 4096;
constexpr int VIDEO_WIDTH = 64;
constexpr int VIDEO_HEIGHT = 32;
constexpr int FONT_SIZE = 80;

class Chip8 {
public:
    Chip8();
    auto LoadRom(const std::string &filepath) -> void;
    auto Cycle() -> void;
    
    std::array<uint8_t, VIDEO_WIDTH * VIDEO_HEIGHT> m_display{};
    std::array<uint8_t, FONT_SIZE> m_fontset {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    std::array<uint8_t, NUMBER_OF_KEYS> m_inputKeys{};

private:
    auto LoadFontSet() -> void;
    auto IncrementProgramCounter() -> void;
    std::array<uint16_t, NUMBER_OF_BIT_VALUES> m_stack{START};
    uint16_t opcode{};
    uint16_t m_index {};
    uint16_t m_programCounter{START_ADDRESS};
    std::array<uint8_t, MEMORY_SIZE> m_memory{};
    std::array<uint8_t, NUMBER_OF_REGISTERS> m_registers{};
    uint8_t m_delayRegister{};
    uint8_t m_soundRegister{};
    uint8_t m_stackPointer{0};
};