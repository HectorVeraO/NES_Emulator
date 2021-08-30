#include <iostream>

int main() {
    uint8_t a = 0xFF;
    uint8_t b = 0xFF;
    uint16_t c = (a << 8) | b;
    printf("n is %d", c);
    return 0;
}