#ifndef CODE_H
#define CODE_H

#include <cstdint>
#include <unordered_map>
#include <string>

class Code{
private:
	std::unordered_map<std::string, uint16_t> destMap;
	std::unordered_map<std::string, uint16_t> compMap;
	std::unordered_map<std::string, uint16_t> jumpMap;

public:
	Code();
	uint16_t dest(const std::string& mnemonic);
	uint16_t comp(const std::string& mnemonic);
	uint16_t jump(const std::string& mnemonic);
};

#endif
