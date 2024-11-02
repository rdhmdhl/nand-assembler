#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <unordered_map> 
#include <string>

class SymbolTable {
private:
	std::unordered_map<std::string, int> symbolMap;

public:
	SymbolTable();
	void AddEntry(const std::string& symbol, int address);
	bool Contains(const std::string& symbol) const;
	int GetAddress(const std::string& symbol) const;
	void Print() const;
};

#endif
