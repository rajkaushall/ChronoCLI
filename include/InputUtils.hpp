#ifndef CHRONOCLI_INPUT_UTILS_HPP
#define CHRONOCLI_INPUT_UTILS_HPP

#include <string>

namespace chronocli {

std::string trim(const std::string& text);

int readInteger(const std::string& prompt);
int readIntegerInRange(const std::string& prompt, int minValue, int maxValue);

std::string readText(const std::string& prompt);
std::string readNonEmptyText(const std::string& prompt);

void waitForEnter();
void printLine(char symbol = '-', int count = 40);

} // namespace chronocli

#endif
