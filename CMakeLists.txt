cmake_minimum_required(VERSION 3.10.0)
project(bayan VERSION 0.1.0 LANGUAGES C CXX)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch Version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(bulk VERSION ${PROJECT_VERSION} LANGUAGES C CXX)

option(WITH_GTEST_TEST "Whether to build GTEST test" ON)

configure_file(version.h.in version.h)

find_package(Boost REQUIRED COMPONENTS program_options filesystem)
find_package(GTest REQUIRED)

add_executable(bayan 
    src/main.cpp
    src/options.cpp
    src/bayan.cpp
)

target_include_directories(bayan PRIVATE inc)
target_link_libraries(bayan PRIVATE Boost::program_options Boost::filesystem)

add_executable(test_bayan 
    tests/test.cpp
    src/options.cpp
    src/bayan.cpp
)

target_include_directories(test_bayan PRIVATE inc)
target_link_libraries(test_bayan PRIVATE Boost::program_options Boost::filesystem GTest::GTest)

install(TARGETS bayan RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

if(WITH_GTEST_TEST)
    enable_testing()
    add_test(test_bayan test_bayan)
endif()