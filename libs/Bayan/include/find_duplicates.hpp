#ifndef FIND_DUPLICATES_H
#define FIND_DUPLICATES_H

#include <unordered_set>

#include "options_parser.hpp"
#include "../src/helper.hpp"

namespace bayan {
    class FindDuplicates {
    public:
        FindDuplicates() = delete;
        explicit FindDuplicates(OptionsParser::Options options);
        ~FindDuplicates() = default;

        std::string duplicates() const;

    private:
        std::unordered_set<std::string> _scanDirs;
        std::unordered_set<std::string> _scanEDirs;
        std::unordered_set<std::string> _fileMasks;
        scanLevel _level;
        unsigned int _fileMinSize;
        unsigned int _blockSize;
        hashAlgorithm _algorithm;
    };
}

#endif // FIND_DUPLICATES_H