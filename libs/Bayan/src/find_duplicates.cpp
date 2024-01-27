#include "find_duplicates.hpp"

#include <boost/filesystem/directory.hpp>
#include <boost/filesystem/file_status.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/container/list.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/algorithm/cxx11/all_of.hpp>

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

    boost::unordered_set<boost_fs::path> FindDuplicates::duplicates() const {
        boost::unordered_set<boost_fs::path> resultFilePaths;
        boost_cr::list<boost_fs::path> notLookedDirs;

        boost::range::copy(_scanDirs, std::back_inserter(notLookedDirs));
        
        for (const auto& nlDir : notLookedDirs) {
            if (_scanEDirs.contains(nlDir))
                continue;
            boost_fs::directory_iterator bDirIt(nlDir);
            boost_fs::directory_iterator eDirIt;
            for (; bDirIt != eDirIt; ++bDirIt) {
                switch (getFileType(*bDirIt)) {
                case fileType::regular:
                    if (filterPassed(*bDirIt)) 
                        resultFilePaths.insert(*bDirIt);
                    break;
                case fileType::directory:
                    if (_level == scanLevel::all) 
                        notLookedDirs.push_back(*bDirIt);
                    break;
                case fileType::other:
                    break;
                }
            }
        }

        return resultFilePaths;
    }

    FindDuplicates::fileType FindDuplicates::getFileType(
        const boost::filesystem::directory_entry& file) const {
        if (boost_fs::is_symlink(file) || boost_fs::is_other(file)) {
            return fileType::other;
        } else if (boost_fs::is_regular_file(file)) {
            return fileType::regular;
        } else {
            return fileType::directory;
        }
    }

    bool FindDuplicates::filterPassed(const boost::filesystem::path& path) const {
        const bool minFileSizePassed = boost_fs::file_size(path) >= _fileMinSize;
        const bool masksPassed = boost::algorithm::all_of(_fileMasks, 
            [&path](const boost::regex& mask) {
                return boost::regex_match(path.string(), mask);
            }
        );

        return minFileSizePassed && masksPassed;
    }
}
