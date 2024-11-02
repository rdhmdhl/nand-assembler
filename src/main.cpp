#include "../include/SymbolTable.h"
#include "../include/Parser.h"
#include "../include/Code.h"
#include <iostream>
using namespace std;

int main(){
    // parse assembly file into output file
    Parser parser("Add.asm", "output.txt");
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
