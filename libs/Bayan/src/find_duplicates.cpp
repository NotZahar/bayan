#include "find_duplicates.hpp"

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/filesystem/operations.hpp>

namespace bayan {
    FindDuplicates::FindDuplicates(OptionsParser::Options options)
        : _level(options.level),
          _fileMinSize(options.fileMinSize),
          _blockSize(options.blockSize),
          _algorithm(options.algorithm)
    {
        for (const auto& dir : options.dirs)
            _scanDirs.insert(dir);

        for (const auto& eDir : options.eDirs)
            _scanEDirs.insert(eDir);

        for (const auto& fileMask : options.fileMasks)
            _fileMasks.insert(fileMask);
    }

    std::string FindDuplicates::duplicates() const {
        boost::filesystem::directory_iterator begin("./");
        boost::filesystem::directory_iterator end;
        for (; begin != end; ++begin) {
            const auto fileStatus = begin->status();
            switch (fileStatus.type()) {
            case boost::filesystem::regular_file:
                std::cout << "FILE ";
                break;
            case boost::filesystem::directory_file:
                std::cout << "DIRECTORY ";
                break;
            default: 
                std::cout << "OTHER ";
                break;
            }

            std::cout << *begin << '\n';
        }

        return "";
    }
}
