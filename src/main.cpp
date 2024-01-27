#include <iostream>

#include <options_parser.hpp>
#include <find_duplicates.hpp>

int main(int argc, char** argv) {
    bayan::OptionsParser op(argc, argv);
    auto options = op.getOptions();
    if (options.helper) {
        std::cout << bayan::messages::general::HELP_MESSAGE;
        return 0;
    }

    bayan::FindDuplicates fd(std::move(options));
    for (const auto& d : fd.duplicates())
        std::cout << d << '\n';

    return 0;
}
