#ifndef FIND_DUPLICATES_H
#define FIND_DUPLICATES_H

#include <boost/filesystem/file_status.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/flyweight/flyweight.hpp>
#include <boost/unordered/unordered_set_fwd.hpp>
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include <boost/container/set.hpp>
#include <boost/regex.hpp>

#include "options_parser.hpp"
#include "../src/helper.hpp"

namespace bayan {
    class FindDuplicates {
    public:
        FindDuplicates() = delete;
        explicit FindDuplicates(OptionsParser::Options options);
        ~FindDuplicates() = default;

        boost::container::set<boost::container::set<boost::filesystem::path>> duplicates() const;

    private:
        enum class fileType {
            regular,
            directory,
            other  
        };

        struct FilterResult {
            bool passed;
            unsigned int fileSize;
        };

        struct FileInfo {
            boost::filesystem::path path;
            unsigned int size;
        };

        using scanFiles_t = boost::unordered_map<unsigned int, boost::unordered_set<boost::filesystem::path>>;

        scanFiles_t getFilesToFind() const;
        boost::container::set<boost::container::set<boost::filesystem::path>> findDuplicates(scanFiles_t scanFiles) const;
        fileType getFileType(const boost::filesystem::directory_entry& file) const;
        FilterResult filterPassed(const boost::filesystem::path& path) const;

        boost::unordered_set<boost::filesystem::path> _scanDirs;
        boost::unordered_set<boost::filesystem::path> _scanEDirs;
        boost::unordered_set<boost::regex> _fileMasks;
        scanLevel _level;
        unsigned int _fileMinSize;
        unsigned int _blockSize;
        hashAlgorithm _algorithm;
    };
}

#endif // FIND_DUPLICATES_H