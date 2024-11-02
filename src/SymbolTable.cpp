#include "SymbolTable.h"
#include <iostream>

SymbolTable::SymbolTable() {
    symbolMap = {
        {"SP", 0}, {"LCL", 1}, {"ARG", 2}, {"THIS", 3}, {"THAT", 4},
        {"R0", 0}, {"R1", 1}, {"R2", 2}, {"R3", 3}, {"R4", 4},
        {"R5", 5}, {"R6", 6}, {"R7", 7}, {"R8", 8}, {"R9", 9},
        {"R10", 10}, {"R11", 11}, {"R12", 12}, {"R13", 13}, {"R14", 14},
        {"R15", 15}, {"SCREEN", 16384}, {"KBD", 24576}
    };
}

void SymbolTable::AddEntry(const std::string& symbol, int address) {
    symbolMap[symbol] = address;
}

bool SymbolTable::Contains(const std::string& symbol) const {
    return symbolMap.find(symbol) != symbolMap.end();
}

int SymbolTable::GetAddress(const std::string& symbol) const {
    auto it = symbolMap.find(symbol);
    return (it != symbolMap.end()) ? it->second : -1;
}

void SymbolTable::Print() const {
    for (const auto& elem : symbolMap) {
        std::cout << elem.first << " " << elem.second << std::endl;
    }
}

