#ifndef HELPER_H
#define HELPER_H

#include <string>
#include <iostream>

namespace bayan {
    enum scanLevel : unsigned int {
        current,
        all
    };

    enum class hashAlgorithm : unsigned int {
        crc32,
        sha1
    };

    namespace messages {
        struct general {
            inline static const std::string HELP_HEADER = "Парамеры";
            inline static const std::string DIRS = "Директории:";
            inline static const std::string EDIRS = "Исключенные директории:";
            inline static const std::string SCAN_LEVEL = "Уровень сканирования:";
            inline static const std::string FILE_MIN_SIZE = "Минимальный размер файла:";
            inline static const std::string FILE_MASKS = "Маски файлов:";
            inline static const std::string BLOCK_SIZE = "Размер блока чтения:";
            inline static const std::string ALGORITHM = "Алгоритм хеширования:";
            
            inline static const std::string HELP_MESSAGE = 
R"(Парамеры:
  -h [ --help ]          печать данного текста
  -d [ --dir ] arg       директория для сканирования
  -e [ --exclude ] arg   директория для исключения из сканирования
  -l [ --level ] arg     уровень сканирования (0, 1)
  -f [ --min ] arg       минимальный размер файла
  -m [ --mask ] arg      маски имен файлов разрешенных для сравнения
  -b [ --block ] arg     размер блока чтения
  -a [ --algorithm ] arg алгоритм хэширования
)";
        };

        struct errors {
            inline static const std::string GENERAL_PREFIX = "ERROR:";
            inline static const std::string INVALID_OPTIONS = "Недопустимые параметры";
        };

        struct commands {
            inline static const std::string HELP = "help,h";
            inline static const std::string HELP_F = "help";
            inline static const std::string HELP_DESCR = "печать данного текста";
            
            inline static const std::string DIR_SCAN = "dir,d";
            inline static const std::string DIR_SCAN_F = "dir";
            inline static const std::string DIR_SCAN_DESCR = "директория для сканирования";
            
            inline static const std::string EXC_SCAN = "exclude,e";
            inline static const std::string EXC_SCAN_F = "exclude";
            inline static const std::string EXC_SCAN_DESCR = "директория для исключения из сканирования";
            
            inline static const std::string LEV_SCAN = "level,l";
            inline static const std::string LEV_SCAN_F = "level";
            inline static const std::string LEV_SCAN_DESCR = "уровень сканирования (0, 1)";            
            
            inline static const std::string FILE_MIN_SIZE = "min,f";
            inline static const std::string FILE_MIN_SIZE_F = "min";
            inline static const std::string FILE_MIN_SIZE_DESCR = "минимальный размер файла";
            
            inline static const std::string FILE_MASKS = "mask,m";
            inline static const std::string FILE_MASKS_F = "mask";
            inline static const std::string FILE_MASKS_DESCR = "маски имен файлов разрешенных для сравнения";
            
            inline static const std::string BLOCK_SIZE = "block,b";
            inline static const std::string BLOCK_SIZE_F = "block";
            inline static const std::string BLOCK_SIZE_DESCR = "размер блока чтения";
            
            inline static const std::string ALGORITHM = "algorithm,a";
            inline static const std::string ALGORITHM_F = "algorithm";
            inline static const std::string ALGORITHM_DESCR = "алгоритм хэширования";
        };
    };

    struct Utility {
        static void throwError(const std::string& message) {
            std::cerr << messages::errors::GENERAL_PREFIX << ' ' << message << '\n';
            throw;
        }
    };
}

#endif // HELPER_H