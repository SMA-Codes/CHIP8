#include "Chip8.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cstring>

Chip8::Chip8() {
    m_display.fill(0);
    m_registers.fill(0);
    m_stack.fill(0);
    m_memory.fill(0);
    m_inputKeys.fill(0);
    m_programCounter = START_ADDRESS;
    m_stackPointer = 0;
    m_index = 0;
    LoadFontSet();
}

auto Chip8::LoadRom(const std::string &filepath) -> void {
    std::ifstream file;
    file.open(filepath, std::ios::binary);

    auto c = static_cast<u_int8_t>(file.get());
    int i = 0;
    while (!file.eof()) {
        m_memory[i + START_ADDRESS] = c;
        c = file.get();
        i++;
    }
}

auto Chip8::Cycle() -> void {
    opcode = (m_memory[m_programCounter] << 8) | (m_memory[m_programCounter + 1]);
    std::cout << "PC: " << std::hex << m_programCounter << ", Opcode: " << opcode << std::endl;
    IncrementProgramCounter();
    switch (((opcode & 0xF000) >> 12)) {
        case 0x0: {
            if (opcode == 0x00E0) {
                for (int i = 0; i < m_display.size(); i++) {
                    m_display.at(i) = 0;
                }
            }

            else if (opcode == 0x00EE) {
                m_stackPointer--;
                m_programCounter = m_stack[m_stackPointer];
            }
            break;
        }
        
        case 0x1:
            m_programCounter = (opcode & 0x0FFF);
            break;

        case 0x2: 
            m_stack[m_stackPointer] = m_programCounter;
            m_stackPointer++;
            m_programCounter = opcode & 0x0FFF;
            break;

        case 0x3: {
            auto registerNumber = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
            auto lowestEightBits = static_cast<uint8_t>(opcode & 0x00FF);
            if (m_registers[registerNumber] == lowestEightBits) {
                IncrementProgramCounter();
            }
            break;
        }
        
        case 0x4: {
            auto registerNumber = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
            auto lowestEightBits = static_cast<uint8_t>(opcode & 0x00FF);
            if (m_registers[registerNumber] != lowestEightBits) {
                IncrementProgramCounter();
            }
            break;
        }
        
        case 0x5: {
            auto firstRegisterNumber = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
            auto secondRegisterNumber = static_cast<uint8_t>(opcode & 0x00F0) >> 4;
            if (m_registers[firstRegisterNumber] == m_registers[secondRegisterNumber]) {
                IncrementProgramCounter();
            }
            break;
        }

        case 0x6: {
            auto registerNumber = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
            auto lowestEightBits = static_cast<uint8_t>(opcode & 0x00FF);
            m_registers[registerNumber] = lowestEightBits;
            break;
        }
        
        case 0x7: {
            auto registerNumber = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
            auto lowestEightBits = static_cast<uint8_t>(opcode & 0x00FF);
            m_registers[registerNumber] += lowestEightBits;
            break;
        }
        
        case 0x8: {
            auto lowestFourBits = static_cast<uint8_t>((opcode & 0xF));
            auto firstRegister = static_cast<uint8_t>((opcode >> 8) & 0x000F);
            auto secondRegister = static_cast<uint8_t>((opcode >> 4) & 0x000F);
            switch (lowestFourBits) {
                case 0x0:
                    m_registers[firstRegister] = m_registers[secondRegister];
                    break;

                case 0x1:
                    m_registers[firstRegister] |= m_registers[secondRegister];
                    break;

                case 0x2:
                    m_registers[firstRegister] &= m_registers[secondRegister];
                    break;
                
                case 0x3:
                    m_registers[firstRegister] ^= m_registers[secondRegister];
                    break;
                
                case 0x4:
                    m_registers[firstRegister] += m_registers[secondRegister];
                    if (m_registers[firstRegister] > 255) {
                        m_registers[m_registers.size() - 1] = 1;
                    }

                    else {
                        m_registers[m_registers.size() - 1] = 0;
                    }

                    m_registers[firstRegister] &= 0x00FF;
                    break;
                
                case 0x5:
                    if (m_registers[firstRegister] > m_registers[secondRegister]) {
                        m_registers[m_registers.size() - 1] = 1;
                    }

                    else {
                        m_registers[m_registers.size() - 1] = 0;
                    }
                    m_registers[firstRegister] -= m_registers[secondRegister];
                    break;
                
                case 0x6:
                    if ((m_registers[firstRegister] & 0x01) == 1) {
                        m_registers[m_registers.size() - 1] = 1;
                    }

                    else {
                        m_registers[m_registers.size() - 1] = 0;
                    }
                    m_registers[firstRegister] /= 2;
                    break;
                
                case 0x7:
                    if (m_registers[secondRegister] > m_registers[firstRegister]) {
                        m_registers[m_registers.size() - 1] = 1;
                    }

                    else {
                        m_registers[m_registers.size() - 1] = 0;
                    }
                    m_registers[firstRegister] = m_registers[secondRegister] - m_registers[firstRegister];
                    break;
                
                case 0xE:
                    if ((m_registers[firstRegister] & 0x80) == 1) {
                        m_registers[m_registers.size() - 1] = 1;
                    }

                    else {
                        m_registers[m_registers.size() - 1] = 0;
                    }
                    m_registers[firstRegister] *= 2;
                    break;
            }
        }
        
        case 0x9: {
            auto firstRegister = static_cast<uint8_t>((opcode >> 8) & 0x000F);
            auto secondRegister = static_cast<uint8_t>((opcode >> 4) & 0x000F);
            if (m_registers[firstRegister] != m_registers[secondRegister]) {
                IncrementProgramCounter();
            }
            break;
        }

        case 0xA: {
            auto lowestTwelveBits = static_cast<uint16_t>((opcode & 0x0FFF));
            m_index = lowestTwelveBits;
            std::cout << "Set I to: " << std::hex << m_index << std::endl;
            break;
        }
        
        case 0xB: {
            auto lowestTwelveBits = static_cast<uint16_t>((opcode & 0x0FFF));
            m_programCounter = m_registers[0] + lowestTwelveBits;
            break;
        }
        
        case 0xC: {
            auto randomByte = static_cast<uint8_t>(rand() % 256);
            auto lowestEightBits = static_cast<uint8_t>(opcode & 0x00FF);
            auto result = static_cast<uint8_t>(randomByte & lowestEightBits);
            auto firstRegisterNumber = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
            m_registers[firstRegisterNumber] = result;
            break;
        }
        
        case 0xD: {
            auto registerX = static_cast<uint8_t>((opcode & 0x0F00) >> 8);
            auto registerY = static_cast<uint8_t>((opcode & 0x00F0) >> 4);
            auto xCoordinate = static_cast<uint8_t>(m_registers[registerX] % VIDEO_WIDTH);
            auto yCoordinate = static_cast<uint8_t>(m_registers[registerY] % VIDEO_HEIGHT);
            auto nBytes = static_cast<uint8_t>(opcode & 0x000F);
            m_registers[m_registers.size() - 1] = 0;

            for (int i = 0; i < nBytes; i++) {
                auto sprite = static_cast<uint8_t>(m_memory[m_index + i]);
                for (int j = 0; j < 8; j++) {
                    if ((sprite & (0x80 >> j)) != 0) {
                        if (m_display[((yCoordinate + i) * VIDEO_WIDTH) + (xCoordinate + j)] == 1) {
                            m_registers[m_registers.size() - 1] = 1;
                        }
                        m_display[((yCoordinate + i) * VIDEO_WIDTH) + (xCoordinate + j)] ^= 1;
                    }
                }
            }
            break;
        }
        
        case 0xE: {
            auto lowestEightBits = static_cast<uint8_t>((opcode & 0x00FF));
            switch (lowestEightBits) {
                case 0x9E: {
                    uint8_t registerNumber = (opcode & 0x0F00) >> 8;
                    uint8_t keyPressed = m_registers[registerNumber];
                    if (m_inputKeys[keyPressed] == 1) {
                        IncrementProgramCounter();
                    }
                    break;
                }
                
                case 0xA1: {
                    uint8_t registerNumber = (opcode & 0x0F00) >> 8;
                    uint8_t keyPressed = m_registers[registerNumber];
                    if (m_inputKeys[keyPressed] == 0) {
                        IncrementProgramCounter();
                    }
                    break;
                }
            }
        }
        
        case 0xF: {
            auto lowestEightBits = static_cast<uint8_t>((opcode & 0x00FF));
            switch (lowestEightBits) {
                case 0x07: {
                    uint8_t registerNumber = (opcode & 0x0F00) >> 8;
                    m_registers[registerNumber] = m_delayRegister;
                    break;
                }
                
                case 0x0A: {
                    uint8_t registerNumber = (opcode & 0x0F00) >> 8;
                    bool keyPressed = false;
                    while (!keyPressed) {
                        for (int i = 0; i < m_inputKeys.size(); i++) {
                            if (m_inputKeys.at(i) == 1) {
                                m_registers[registerNumber] = i;
                                keyPressed = true;
                                break;
                            }
                        }
                    }
                    break;
                }
                
                case 0x15: {
                    uint8_t registerNumber = (opcode & 0x0F00) >> 8;
                    m_delayRegister = m_registers[registerNumber];
                    break;
                }
                
                case 0x18: {
                    uint8_t registerNumber = (opcode & 0x0F00) >> 8;
                    m_soundRegister = m_registers[registerNumber];
                    break;
                }

                case 0x1E: {
                    uint8_t registerNumber = (opcode & 0x0F00) >> 8;
                    m_index += m_registers[registerNumber];
                    break;
                }
                
                case 0x29: {
                    uint8_t registerNumber = (opcode & 0x0F00) >> 8;
                    m_index = FONT_SET_START_ADDRESS + (m_registers[registerNumber] * 5);
                    break;
                }

                case 0x33: {
                    uint8_t registerNumber = (opcode & 0x0F00) >> 8;
                    uint8_t registerValue = m_registers[registerNumber];
                    m_memory[m_index] = registerValue % 100;
                    m_memory[m_index + 1] = (registerValue/10) % 10;
                    m_memory[m_index + 2] = registerValue % 10;
                    
                    break;
                }
                
                case 0x55:
                    for (int i = 0; i < m_registers.size(); i++) {
                        m_memory[m_index + i] = m_registers[i];
                    }
                    break;
                
                case 0x65:
                    for (int i = 0; i < m_registers.size(); i++) {
                        m_registers[i] = m_memory[m_index + i];
                    }
                    break;

            }
        }   
    }
}

auto Chip8::LoadFontSet() -> void {
    for (int i = 0; i < m_fontset.size(); i++) {
        m_memory[FONT_SET_START_ADDRESS + i] = m_fontset[i];
    }
}

auto Chip8::IncrementProgramCounter() -> void {
    m_programCounter += 2;
}
