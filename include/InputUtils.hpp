#ifndef CHRONOCLI_INPUT_UTILS_HPP
#define CHRONOCLI_INPUT_UTILS_HPP

#include <string>

namespace chronocli {

int readInteger(const std::string& prompt);
int readIntegerInRange(const std::string& prompt, int minValue, int maxValue);

void waitForEnter();
void printLine(char symbol = '-', int count = 40);

} // namespace chronocli

#endif