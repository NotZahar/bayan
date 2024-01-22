#include "options_parser.hpp"

#include <vector>

namespace boost_po = boost::program_options;

namespace bayan {
    OptionsParser::OptionsParser(int argc, char** argv) {
        std::unique_ptr<options_description> description = initOptions();
        _options = parseOptions(argc, argv, std::move(description));
    }

    OptionsParser::Options OptionsParser::getOptions() const {
        return _options;
    }

    std::unique_ptr<boost_po::options_description> OptionsParser::initOptions() const {
        std::unique_ptr<options_description> description = 
            std::make_unique<options_description>(messages::general::HELP_HEADER);
        description->add_options()
            (
                messages::commands::HELP.c_str(), 
                messages::commands::HELP_DESCR.c_str()
            )
            (
                messages::commands::DIR_SCAN.c_str(),
                boost_po::value<std::vector<std::string>>(),
                messages::commands::DIR_SCAN_DESCR.c_str()
            )
            (
                messages::commands::EXC_SCAN.c_str(),
                boost_po::value<std::vector<std::string>>(),
                messages::commands::EXC_SCAN_DESCR.c_str()
            )
            (
                messages::commands::LEV_SCAN.c_str(),
                boost_po::value<unsigned int>(),
                messages::commands::LEV_SCAN_DESCR.c_str()
            )
            (
                messages::commands::FILE_MIN_SIZE.c_str(),
                boost_po::value<unsigned int>(),
                messages::commands::FILE_MIN_SIZE_DESCR.c_str()
            )
            (
                messages::commands::FILE_MASKS.c_str(),
                boost_po::value<std::vector<std::string>>(),
                messages::commands::FILE_MASKS_DESCR.c_str()
            )
            (
                messages::commands::BLOCK_SIZE.c_str(),
                boost_po::value<unsigned int>(),
                messages::commands::BLOCK_SIZE_DESCR.c_str()
            )
            (
                messages::commands::ALGORITHM.c_str(),
                boost_po::value<std::string>(),
                messages::commands::ALGORITHM_DESCR.c_str()
            )
        ;

        return description;
    }

    OptionsParser::Options OptionsParser::parseOptions(int argc, 
            char** argv, 
            std::unique_ptr<options_description> description) const {
        Options options;
        boost_po::variables_map variablesMap;
        boost_po::store(
            boost_po::parse_command_line(argc, argv, *description),
            variablesMap);
        boost_po::notify(variablesMap);

        if (variablesMap.count(messages::commands::HELP_F)) {
            options.helper = true;
            return options;
        }

        if (variablesMap.count(messages::commands::DIR_SCAN_F))
            options.dirs = 
                variablesMap[messages::commands::DIR_SCAN_F].as<std::vector<std::string>>();
        
        if (variablesMap.count(messages::commands::EXC_SCAN_F))
            options.eDirs = 
                variablesMap[messages::commands::EXC_SCAN_F].as<std::vector<std::string>>();

        if (variablesMap.count(messages::commands::LEV_SCAN_F)) {
            switch (variablesMap[messages::commands::LEV_SCAN_F].as<unsigned int>()) {
            case scanLevel::current:
                options.level = scanLevel::current;
                break;
            case scanLevel::all:
                options.level = scanLevel::all;
                break;
            default:
                Utility::throwError(messages::errors::INVALID_OPTIONS);
            }
        }

        if (variablesMap.count(messages::commands::FILE_MIN_SIZE_F))
            options.fileMinSize = 
                variablesMap[messages::commands::FILE_MIN_SIZE_F].as<unsigned int>();

        if (variablesMap.count(messages::commands::FILE_MASKS_F))
            options.fileMasks = 
                variablesMap[messages::commands::FILE_MASKS_F].as<std::vector<std::string>>();
                
        if (variablesMap.count(messages::commands::BLOCK_SIZE_F))
            options.blockSize = 
                variablesMap[messages::commands::BLOCK_SIZE_F].as<unsigned int>();

        if (variablesMap.count(messages::commands::ALGORITHM_F))
            options.algorithm = 
                hashAlgorithms.left.find(
                    variablesMap[messages::commands::ALGORITHM_F].as<std::string>()
                )->second;

        return options;
    }

    std::ostream& operator<<(std::ostream& os, const OptionsParser::Options& options) {
        os << messages::general::DIRS << '\n';
        for (const auto& dir : options.dirs)
            os << ' ' << dir << '\n';
        
        os << messages::general::EDIRS << '\n';
        for (const auto& eDir : options.eDirs)
            os << ' ' << eDir << '\n';
        
        os << messages::general::SCAN_LEVEL << ' ' << (unsigned int)options.level << '\n';

        os << messages::general::FILE_MASKS << '\n';
        for (const auto& mask : options.fileMasks)
            os << ' ' << mask << '\n';

        os << messages::general::FILE_MIN_SIZE << ' ' << options.fileMinSize << '\n';
        os << messages::general::BLOCK_SIZE << ' ' << options.blockSize << '\n';
        os << messages::general::ALGORITHM << ' ' 
            << OptionsParser::hashAlgorithms.right.find(options.algorithm)->second << '\n';

        return os;
    }
}
