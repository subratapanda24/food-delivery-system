#include <iostream>
#include <limits>

void runSystem();

int main() {
    std::cout << "\033[?25h";
    runSystem();
    return 0;
}
