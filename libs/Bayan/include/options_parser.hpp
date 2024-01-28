#ifndef OPTIONS_PARSER_H
#define OPTIONS_PARSER_H

#include <cassert>
#include <iostream>
#include <memory>

#include <boost/bimap.hpp>
#include <boost/assign.hpp>
#include <boost/program_options.hpp>

#include "../src/helper.hpp"

namespace bayan {
    class OptionsParser {
    public:
        using options_description = boost::program_options::options_description;

        inline static const boost::bimap<std::string, hashAlgorithm> hashAlgorithms = 
            boost::assign::list_of<boost::bimap<std::string, hashAlgorithm>::relation>
            ( "crc32", hashAlgorithm::crc32 )
            ( "sha1", hashAlgorithm::sha1 );

        struct Options {
            bool helper = false;
            std::vector<std::string> dirs = { "./" };
            std::vector<std::string> eDirs = {};
            scanLevel level = scanLevel::all;
            unsigned int fileMinSize = 1;
            std::vector<std::string> fileMasks = { ".*" };
            unsigned int blockSize = 5;
            hashAlgorithm algorithm = hashAlgorithm::sha1;

            friend std::ostream& operator<<(std::ostream& os, const Options& options);
        };

        OptionsParser() = delete;
        OptionsParser(int argc, char** argv);
        ~OptionsParser() = default;

        Options getOptions() const;

    private:
        std::unique_ptr<options_description> initOptions() const;
        Options parseOptions(int argc, 
            char** argv, 
            std::unique_ptr<options_description> description) const;

        Options _options;
    };
}

#endif // OPTIONS_PARSER_H