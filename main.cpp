#include <iostream>
#include <fstream>
#include <unordered_map> 
#include <vector>
#include <bitset>
#include <cctype>
#include <algorithm>
using namespace std;

class SymbolTable {
private:
    // member variable
    unordered_map<string, int> symbolMap;
public:

    // constructor
    SymbolTable(){
        symbolMap = 
        {
            {"SP", 0},
            {"LCL", 1},
            {"ARG", 2},
            {"THIS", 3},
            {"THAT", 4},
            {"R0", 0},
            {"R1", 1},
            {"R2", 2},
            {"R3", 3},
            {"R4", 4},
            {"R5", 5},
            {"R6", 6},
            {"R7", 7},
            {"R8", 8},
            {"R9", 9},
            {"R10", 10},
            {"R11", 11},
            {"R12", 12},
            {"R13", 13},
            {"R14", 14},
            {"R15", 15},
            {"SCREEN", 16384}, 
            {"KBD", 24576}
        };
    };
    
    // methods
    void AddEntry(const string& symbol, const int address){
        symbolMap[symbol] = address;
    }
 
    bool Contains(const string& symbol) const {
        // return an iterator
        return symbolMap.find(symbol) != symbolMap.end();
    }

    // get address from symbol
    int GetAddress(const string& symbol) const {
        auto it = symbolMap.find(symbol);
        if(it != symbolMap.end()){
            // accesing the value, of the key value pair
            return it->second;
        }
        // no symbol found
        return -1; 
    }

    void Print() const {
        // elm is the key value pair
        for (const auto& elem : symbolMap){
            cout << elem.first << " " << elem.second << endl;
        }
    }
};

// encapsulates access to input code
// reads assembly language, parses it, and provides convenient access
// also removes all whitespace
class Parser{
    private: 
    ifstream HackAssembly;
    ofstream OutputFile;
    string currentInstruction;
    vector<string> instructions;
    size_t currentInstructionIndex;

    public:
        enum CommandType {
            A_COMMAND, // Addresses commands that begin with @
            C_COMMAND, // Computation commands that perform operations and data movements,
            L_COMMAND // Lable declarations enclosed parenthesis (), does not generate machine instructions
        };

        Parser(const string& inputFilename, const string& outputFilename){
            HackAssembly.open(inputFilename);

            if (!HackAssembly.is_open()){
                throw runtime_error("could not open input file: " + inputFilename);
            } else { 
                cout << "input file opened successfully..." << endl;
            }

            OutputFile.open(outputFilename);

            if (!OutputFile.is_open()){
                throw runtime_error("could not open output file: " + outputFilename);
            }

            currentInstructionIndex = 0;

            // read input file line by line
            while(getline (HackAssembly, currentInstruction)) {
                size_t commentOrWhitespace = currentInstruction.find("//");
                if (commentOrWhitespace != string::npos){
                    currentInstruction = currentInstruction.substr(0, commentOrWhitespace);
                }

                size_t firstNonWhitespace = currentInstruction.find_first_not_of(" \t");
                size_t lastNonWhitespace = currentInstruction.find_last_not_of(" \t");

                // trim leading and trailing whitespace from current line 
                if (firstNonWhitespace != string::npos && lastNonWhitespace != string::npos){
                    currentInstruction = currentInstruction.substr(firstNonWhitespace, lastNonWhitespace - firstNonWhitespace + 1);
                } else {
                    // line is entirely whitespace, skip it
                    currentInstruction.clear();
                }

                // skip empty lines after processing 
                if (currentInstruction.empty()){
                    continue;
                }

                instructions.push_back(currentInstruction);
                OutputFile << currentInstruction << endl;
            }

        }

        // deconstructor to ensure file is closed
        ~Parser(){
            if (HackAssembly.is_open()){
                cout << "closing assembly file.. " << endl;
                HackAssembly.close();
            }
            if (OutputFile.is_open()){
                cout << "closing the output file.. " << endl;
                OutputFile.close();
            }
        }

        CommandType commandType() {
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

        // methods
        bool hasMoreCommands() {
            return currentInstructionIndex < instructions.size();
        }
    
        
        // reads next command from input, makes it the current command 
        // should only be called it hasMoreCommands is true
        string Advance(){
            if (hasMoreCommands()){
                currentInstruction = instructions[currentInstructionIndex];
                currentInstructionIndex++;
            } else {
                currentInstruction = "EOF";
            }
            return currentInstruction;
        }

        string getCurrentInstruction() const {
            return currentInstruction;
        }

        void Reset(){
            currentInstructionIndex = 0;
            currentInstruction = "";
        }

        // return the symbol or decimal(screen address) of the current command 
        // should only be called when commandType is A_COMMAND OR L_COMMAND
        string symbol(const string& instruction){
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
        string dest(){
            size_t equalPos = currentInstruction.find('=');
            if (equalPos != string::npos){
                // dest mnemonic is before the '=' symbol
                return currentInstruction.substr(0, equalPos);
            } 
            // no dest mnemonic
            return "";
        }
        // return comp mnemonic
        string comp(){
            size_t equalPos = currentInstruction.find('=');
            size_t semicolanPos = currentInstruction.find(';');
            size_t compStart = (equalPos != string::npos) ? equalPos + 1 : 0;
            size_t compEnd = (semicolanPos != string::npos) ? semicolanPos : currentInstruction.length();
            return currentInstruction.substr(compStart, compEnd - compStart);
        }
        // return jump mnemonic
        string jump(){
            size_t semiColonPos = currentInstruction.find(';');
            if(semiColonPos != string::npos){
                // jump mnemonic is after ';'
                return currentInstruction.substr(semiColonPos + 1);
            }    
            // no jump mnemonic found
            return "";
        };
};

class Code{
    private:
    unordered_map<string, uint16_t> destMap;
    unordered_map<string, uint16_t> compMap;
    unordered_map<string, uint16_t> jumpMap;

    public:
    Code() {
                // Initialize destMap
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

        // Initialize compMap
        compMap = {
            // a = 0
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

        // Initialize jumpMap
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

    uint16_t dest(const std::string& mnemonic) {
        auto it = destMap.find(mnemonic);
        if (it != destMap.end()) {
            return it->second;
        } else {
            // Handle invalid mnemonic
            throw std::invalid_argument("Invalid dest mnemonic: " + mnemonic);
        }
    }

    uint16_t comp(const std::string& mnemonic) {
        auto it = compMap.find(mnemonic);
        if (it != compMap.end()) {
            return it->second;
        } else {
            // Handle invalid mnemonic
            throw std::invalid_argument("Invalid comp mnemonic: " + mnemonic);
        }
    }

    uint16_t jump(const std::string& mnemonic) {
        auto it = jumpMap.find(mnemonic);
        if (it != jumpMap.end()) {
            return it->second;
        } else {
            // Handle invalid mnemonic
            throw std::invalid_argument("Invalid jump mnemonic: " + mnemonic);
        }
    }
};
 

int main(){
    // parse assembly file into output file
    Parser parser("Max.asm", "output.txt");
    SymbolTable symbolTable;
    cout << "initial symbol table: " << endl;
    symbolTable.Print();
    
    // first pass - ensure all labels (and their addresses) will be in the symbol table
    string instruction;
    size_t instructionLine = 0;
    size_t romAddress = 0;

    // loop through output.txt file, which has been cleaned 
    while(parser.hasMoreCommands()){
        parser.Advance();
        instructionLine++;

        string instruction = parser.getCurrentInstruction();
        if (instruction == "EOF"){
            break;
        }

        Parser::CommandType type = parser.commandType();

        if(type == Parser::L_COMMAND){
            // if labels are not in symbol table, add them
            // look for label declaration, if not in symbol tabel, add them 
            if(instruction[0] == '('){
                string labelToAdd = parser.symbol(instruction);
                if (!symbolTable.Contains(labelToAdd)){
                    symbolTable.AddEntry(labelToAdd, romAddress);
                }
            }
        } else{ 
            // it is an A or C command, 
            // increment ROM counter
            romAddress++;
        }
    }
    cout << "all instructions have been processed" << endl;     
    symbolTable.Print();

    // second pass
    // start over at the top of the file
    parser.Reset();
    size_t variableAddress = 16; 
    Code code;
    ofstream BinaryFile;
    BinaryFile.open("final.txt");

    while(parser.hasMoreCommands()){
        parser.Advance();
        string instruction = parser.getCurrentInstruction();

        if (instruction == "EOF"){
            break;
        }

        Parser::CommandType type = parser.commandType();

        if (type == Parser::A_COMMAND){
            // handle A instruction
            string symbol = parser.symbol(instruction);
            
            uint16_t address;
            if(all_of(symbol.begin(), symbol.end(), ::isdigit)){
                // symbol is a numberic constant
                address = static_cast<uint16_t>(stoi(symbol));
            } else {
                // symbol is variable or label
                if(symbolTable.Contains(symbol)){
                    address = static_cast<uint16_t>(symbolTable.GetAddress(symbol));
                } else {
                    // add new variable to symbol table
                    symbolTable.AddEntry(symbol, variableAddress);
                    address = static_cast<uint16_t>(symbolTable.GetAddress(symbol));
                    variableAddress++;
                }
            }
            // construct A instruction
            uint16_t binaryAInstruction = 0b0000000000000000;
            binaryAInstruction |= address & 0x7FFF; 

            // write A instruction to binary file
            BinaryFile << bitset<16>(binaryAInstruction) << endl;

        } else if (type == Parser::C_COMMAND){
            // handle C instructions
            // parsing mnemonics for dest, comp and jump bits
            string destMnemonic = parser.dest();
            string compMnemonic = parser.comp();
            string jumpMnemonic = parser.jump();

            uint16_t destBits = code.dest(destMnemonic);
            uint16_t compBits = code.comp(compMnemonic);
            uint16_t jumpBits = code.jump(jumpMnemonic);

            // Construct the C-instruction
            uint16_t binaryCInstruction = 0b1110000000000000; // C-instruction starts with '111'
            binaryCInstruction |= (compBits << 6); // comp bits in bits 6-12
            binaryCInstruction |= (destBits << 3); // dest bits in bits 3-5
            binaryCInstruction |= jumpBits;        // jump bits in bits 0-2

            // write C instruction to binary file
            BinaryFile << bitset<16>(binaryCInstruction) << endl;
        }
    }

    BinaryFile.close();
    cout << "second pass complete.. " << endl;
    symbolTable.Print();
}