#Specify the version being used aswell as the language
cmake_minimum_required(VERSION 2.8)
#Name your project here
project(SimpleDLLInjector)

# Include headers
include_directories(include)

# Output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")

# Include files
FILE(GLOB headers include/*.h)
source_group(Includes FILES ${headers})

# Source files
FILE(GLOB sources src/*.cpp)
source_group(Sources FILES ${sources})

add_executable(Injector ${headers} ${sources})

