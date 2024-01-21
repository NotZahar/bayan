#include <iostream>

#include <options_parser.hpp>

int main(int argc, char** argv) {
    bayan::OptionsParser op(argc, argv);
    auto options = op.getOptions();
    if (options.helper) {
        std::cout << bayan::messages::general::HELP_MESSAGE;
        return 0;
    }

    std::cout << options;
    return 0;
}
