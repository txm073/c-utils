#include <iostream>

char intToHexChar(int i) {
    if (0 <= i && i <= 9) {
        return char(i + 48); 
    }
    return char(i + 55);
}

int hexCharToInt(char c) {
    int asciiValue = int(c);
    if (65 <= asciiValue && asciiValue <= 71) {
        return asciiValue - 55;
    } else {
        return asciiValue - 48;
    }
}

std::string hex(int i) {
    int bit1 = i / 16, bit2 = i % 16;
    return std::string(1, intToHexChar(bit1)) + std::string(1, intToHexChar(bit2));
}

int denary(std::string hexString) {
    return (16 * hexCharToInt(hexString[0])) + hexCharToInt(hexString[1]);
}

int main(const int argc, const char* argv[]) {
    std::cout << denary("d8");
    return 0;
}