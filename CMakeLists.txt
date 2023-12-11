cmake_minimum_required(VERSION 3.0)

project(Calculator
    LANGUAGES CXX    
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(DOWNLOAD_EXTRACT_TIMESTAMP true)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/b3a9ba2b8e975550799838332803d468797ae2e1.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory(src)
