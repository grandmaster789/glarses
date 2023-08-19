#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "Subprocess launch test\n";
    std::cout << argc << '\n';
    for (int i = 0; i < argc; ++i)
        std::cout << argv[i] << '\n';

    return 123;
}