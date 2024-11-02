#include "../include/Code.h"
#include <stdexcept>

Code::Code() {
    // Initialize destMap, compMap, and jumpMap
    destMap = {
            {"",    0b000}, // No destination
            {"M",   0b001},
            {"D",   0b010},
            {"MD",  0b011},
            {"A",   0b100},
            {"AM",  0b101},
            {"AD",  0b110},
            {"AMD", 0b111}
        };
    compMap = {
            {"0",   0b0101010},
            {"1",   0b0111111},
            {"-1",  0b0111010},
            {"D",   0b0001100},
            {"A",   0b0110000},
            {"!D",  0b0001101},
            {"!A",  0b0110001},
            {"-D",  0b0001111},
            {"-A",  0b0110011},
            {"D+1", 0b0011111},
            {"A+1", 0b0110111},
            {"D-1", 0b0001110},
            {"A-1", 0b0110010},
            {"D+A", 0b0000010},
            {"D-A", 0b0010011},
            {"A-D", 0b0000111},
            {"D&A", 0b0000000},
            {"D|A", 0b0010101},
            // a = 1
            {"M",   0b1110000},
            {"!M",  0b1110001},
            {"-M",  0b1110011},
            {"M+1", 0b1110111},
            {"M-1", 0b1110010},
            {"D+M", 0b1000010},
            {"D-M", 0b1010011},
            {"M-D", 0b1000111},
            {"D&M", 0b1000000},
            {"D|M", 0b1010101}
        }; 
    jumpMap = {
            {"",    0b000}, // No jump
            {"JGT", 0b001},
            {"JEQ", 0b010},
            {"JGE", 0b011},
            {"JLT", 0b100},
            {"JNE", 0b101},
            {"JLE", 0b110},
            {"JMP", 0b111}
        };
}

uint16_t Code::dest(const std::string& mnemonic) {
    auto it = destMap.find(mnemonic);
    if (it != destMap.end()) return it->second;
    throw std::invalid_argument("Invalid dest mnemonic: " + mnemonic);
}

uint16_t Code::comp(const std::string& mnemonic) {
    auto it = compMap.find(mnemonic);
    if (it != compMap.end()) return it->second;
    throw std::invalid_argument("Invalid comp mnemonic: " + mnemonic);
}

uint16_t Code::jump(const std::string& mnemonic) {
    auto it = jumpMap.find(mnemonic);
    if (it != jumpMap.end()) return it->second;
    throw std::invalid_argument("Invalid jump mnemonic: " + mnemonic);
}

