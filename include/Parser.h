#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <string>
#include <vector>

class Parser{
public:
	enum CommandType {
		A_COMMAND,
		C_COMMAND,
		L_COMMAND
	};

	Parser(const std::string& inputFilename, const std::string& outputFilename);
	~Parser();

	CommandType commandType();
	bool hasMoreCommands();
	std::string Advance();
	std::string getCurrentInstruction() const;
	void Reset();
	std::string symbol(const std::string& instruction);
	std::string dest();
	std::string comp();
	std::string jump();

private:
	std::ifstream HackAssembly;
	std::ofstream OutputFile;
	std::string currentInstruction; 
	std::vector<std::string> instructions;
	size_t currentInstructionIndex;
};

#endif
