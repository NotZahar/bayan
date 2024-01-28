#include "find_duplicates.hpp"

#include <fstream>
#include <functional>
#include <memory>

#include <boost/filesystem/directory.hpp>
#include <boost/filesystem/file_status.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/container/list.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/algorithm/cxx11/all_of.hpp>
#include <boost/crc.hpp>
#include <boost/compute/detail/sha1.hpp>

#include "helper.hpp"

namespace boost_fs = boost::filesystem;
namespace boost_cr = boost::container;

namespace bayan {
    FindDuplicates::FindDuplicates(OptionsParser::Options options)
        : _level(options.level),
          _fileMinSize(options.fileMinSize),
          _blockSize(options.blockSize),
          _algorithm(options.algorithm)
    {
        for (const auto& dir : options.dirs)
            _scanDirs.insert(boost_fs::canonical(dir));

        for (const auto& eDir : options.eDirs)
            _scanEDirs.insert(boost_fs::canonical(eDir));

        for (const auto& fileMask : options.fileMasks)
            _fileMasks.insert(boost::regex{ fileMask });
    }

    boost_cr::set<boost_cr::set<std::string>> FindDuplicates::duplicates() const {
        auto files = getFilesToFind();
        return findDuplicates(std::move(files));
    }

    FindDuplicates::scanFiles_t FindDuplicates::getFilesToFind() const {
        scanFiles_t scanFiles;
        boost_cr::list<boost_fs::path> notLookedDirs;
        
        boost::range::copy(_scanDirs, std::back_inserter(notLookedDirs));
        
        auto pushFile = [this, &scanFiles](const boost_fs::path& path) {
            const auto result = filterPassed(path);
            if (!result.passed)
                return;
            if (scanFiles.contains(result.fileSize))
                scanFiles[result.fileSize].push_back(path);
            scanFiles.insert({ result.fileSize, { path } });
        };
        
        auto pushDir = [this, &notLookedDirs](const boost_fs::path& path) {
            if (_level == scanLevel::all) 
                notLookedDirs.push_back(path);
        };

        for (const auto& nlDir : notLookedDirs) {
            if (_scanEDirs.contains(nlDir))
                continue;
            boost_fs::directory_iterator bDirIt(nlDir);
            boost_fs::directory_iterator eDirIt;
            for (; bDirIt != eDirIt; ++bDirIt) {
                switch (getFileType(*bDirIt)) {
                case fileType::regular:
                    pushFile(*bDirIt);
                    break;
                case fileType::directory:
                    pushDir(*bDirIt);
                    break;
                case fileType::other:
                    break;
                }
            }
        }

        return scanFiles;
    }

    boost_cr::set<boost_cr::set<std::string>> FindDuplicates::findDuplicates(
        scanFiles_t scanFiles) const {
        boost_cr::set<boost_cr::set<std::string>> duplicates;
        if (scanFiles.size() < 2ul)
            return duplicates;

        for (const auto& [fileSize, files] : scanFiles) {
            if (files.size() <= 1ul)
                continue;

            for (const auto& file : files) {
                file.filename(); // TODO: [here]
            }
        }
        // struct data {
        //     boost_fw::flyweight<unsigned long> 
        // };
        
        // boost::unordered_map<std::string, > filesData;
        // for (auto& file : files) {
            
            
            // std::ifstream reader(file, std::ifstream::binary);
            // std::string buffer(_blockSize, '\0');
            // reader.read(&buffer[0], _blockSize);
            
            // boost::crc_32_type hashResult;
            // hashResult.process_bytes(buffer.data(), buffer.length());

            // boost::crc_32_type hashResult2;
            // hashResult2.process_bytes(buffer.data(), buffer.length());
            
            // boost::compute::detail::sha1 shaHash1{ buffer };
            // std::string shaRes1{ shaHash1 };
            // // auto d = std::hash<std::string>{}(shaRes1);

            // // boost::compute::detail::sha1 shaHash2{ buffer };
            // // std::string shaRes2{ shaHash2 };

            // std::cout << hashResult.checksum() << ' ' << hashResult2.checksum();
        // }

        return duplicates;
    }

    FindDuplicates::fileType FindDuplicates::getFileType(
        const boost_fs::directory_entry& file) const {
        if (boost_fs::is_symlink(file) || boost_fs::is_other(file)) {
            return fileType::other;
        } else if (boost_fs::is_regular_file(file)) {
            return fileType::regular;
        } else {
            return fileType::directory;
        }
    }

    FindDuplicates::FilterResult FindDuplicates::filterPassed(
        const boost_fs::path& path) const {
        const unsigned int fileSize = boost_fs::file_size(path);
        const bool minFileSizePassed = fileSize >= _fileMinSize;
        const bool masksPassed = boost::algorithm::all_of(_fileMasks, 
            [&path](const boost::regex& mask) {
                return boost::regex_match(path.string(), mask);
            }
        );

        return { 
            minFileSizePassed && masksPassed,
            fileSize
        };
    }
}
