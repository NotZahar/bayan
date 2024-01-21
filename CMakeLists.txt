cmake_minimum_required(VERSION 3.21)

set(PATCH_VERSION 1 CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project("bayan" VERSION ${PROJECT_VESRION})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)

add_subdirectory(libs)

add_executable(${PROJECT_NAME})
add_executable(bayan_tests
    tests/bayan.cpp
)

target_sources(${PROJECT_NAME}
    PRIVATE
        src/main.cpp
)

target_include_directories(bayan_tests 
    PRIVATE 
        ${GTEST_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        Bayan)
target_link_libraries(bayan_tests 
    PRIVATE 
        ${GTEST_BOTH_LIBRARIES} Bayan)

if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -pedantic)
    target_compile_options(bayan_tests PRIVATE -Wall -Wextra -Werror -pedantic)
endif()

# generate deb
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()
add_test(NAME bayan_tests COMMAND bayan_tests)