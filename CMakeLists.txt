cmake_minimum_required(VERSION 3.10)
project(SimpleBrowser)

# Set the C++ standard before adding the executable
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Disable compiler-specific extensions

# Include SDL2 and CURL libraries
find_package(CURL REQUIRED)
find_package(SDL2 REQUIRED)

# Include directories for CURL, SDL2, and SDL2_ttf
include_directories(${CURL_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS})
include_directories(/opt/homebrew/include/SDL2)  # SDL2 includes
include_directories(/opt/homebrew/include)       # SDL2_ttf includes

# Manually set Gumbo include and library directories (assuming Homebrew install location)
include_directories(/opt/homebrew/include)  # Gumbo include
link_directories(/opt/homebrew/lib)         # Gumbo library

# Add executable and source files
add_executable(SimpleBrowser src/main.cpp)

# Link the libraries, including SDL2_ttf and Gumbo
target_link_libraries(SimpleBrowser 
    ${CURL_LIBRARIES} 
    ${SDL2_LIBRARIES} 
    /opt/homebrew/lib/libSDL2_ttf.dylib 
    /opt/homebrew/lib/libgumbo.dylib  # Manually link Gumbo
)
