#include "../include/Parser.h"
#include <stdexcept>
#include <iostream>

Parser::Parser(const std::string& inputFilename, const std::string& outputFilename)
    : currentInstructionIndex(0) {
    HackAssembly.open(inputFilename);
   
    if (!HackAssembly.is_open()) throw std::runtime_error("could not open input file: " + inputFilename);
    
    OutputFile.open(outputFilename);
    if (!OutputFile.is_open()) throw std::runtime_error("could not open output file: " + outputFilename);

    while (getline(HackAssembly, currentInstruction)) {
        // Strip comments and whitespace
        size_t commentOrWhitespace = currentInstruction.find("//");
        if (commentOrWhitespace != std::string::npos) {
            currentInstruction = currentInstruction.substr(0, commentOrWhitespace);
        }
        // Trim whitespace
        size_t firstNonWhitespace = currentInstruction.find_first_not_of(" \t");
        size_t lastNonWhitespace = currentInstruction.find_last_not_of(" \t");

        if (firstNonWhitespace != std::string::npos && lastNonWhitespace != std::string::npos) {
            currentInstruction = currentInstruction.substr(firstNonWhitespace, lastNonWhitespace - firstNonWhitespace + 1);
        } else {
            currentInstruction.clear();
        }

        if (!currentInstruction.empty()) {
            instructions.push_back(currentInstruction);
            OutputFile << currentInstruction << std::endl;
        }
    }
}

Parser::~Parser() {
    if (HackAssembly.is_open()) HackAssembly.close();
    if (OutputFile.is_open()) OutputFile.close();
};

Parser::CommandType Parser::commandType() {
    if (currentInstruction.empty()) {
        throw std::runtime_error("No current instruction to determine command type.");    
    }

    if (currentInstruction[0] == '@') {
        return A_COMMAND;
    } else if (currentInstruction[0] == '(' && currentInstruction.back() == ')') {
        return L_COMMAND;
    } else {
        return C_COMMAND;
    }
}

bool Parser::hasMoreCommands() {
    return  currentInstructionIndex < instructions.size();
}
    
// reads next command from input, makes it the current command 
// should only be called it hasMoreCommands is true
std::string Parser::Advance(){
    if (hasMoreCommands()){
        currentInstruction = instructions[currentInstructionIndex];
        currentInstructionIndex++;
    } else {
        currentInstruction = "EOF";
    }
    return currentInstruction;
}

std::string Parser::getCurrentInstruction() const {
    return currentInstruction;
}

void Parser::Reset(){
    currentInstructionIndex = 0;
    currentInstruction = "";
}

// return the symbol or decimal(screen address) of the current command 
// should only be called when commandType is A_COMMAND OR L_COMMAND
std::string Parser::symbol(const std::string& instruction){
    if (instruction[0] == '@'){
        // a_command @xxx
        // strip the @ symbol and return 
        return instruction.substr(1);
            
    } else if(instruction[0] == '(' && instruction.back() == ')'){
        // l_command (Xxx)
        // strip the parentesis, return
        return instruction.substr(1, instruction.length() - 2);
    }
        return "Invalid input for symbol method"; 
}

// return dest mnemonic
std::string Parser::dest(){
    size_t equalPos = currentInstruction.find('=');
    if (equalPos != std::string::npos){
        // return mnemonic is before the '=' symbol
        return currentInstruction.substr(0, equalPos);
    }
    // no dest mnemonic
    return "";
}
        
// return comp mnemonic
std::string Parser::comp(){
    size_t equalPos = currentInstruction.find('=');
    size_t semicolanPos = currentInstruction.find(';');
    size_t compStart = (equalPos != std::string::npos) ? equalPos + 1 : 0;
    size_t compEnd = (semicolanPos != std::string::npos) ? semicolanPos : currentInstruction.length();
    return currentInstruction.substr(compStart, compEnd - compStart);
}

// return jump mnemonic
std::string Parser::jump(){
    size_t semiColonPos = currentInstruction.find(';');
    if(semiColonPos != std::string::npos){
        // jump mnemonic is after ';'
        return currentInstruction.substr(semiColonPos + 1);
    }    
    // no jump mnemonic found    
    return "";
}

