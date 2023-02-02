#include "cleaner.h"

#include <iostream>

int main(int argc, const char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " config_file_path" << std::endl;
        return 1;
    }
    Cleaner cleaner(argv[1]);
    cleaner.ConsoleDialog();
    return 0;
}
