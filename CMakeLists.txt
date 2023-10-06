cmake_minimum_required(Version 3.15) # CMake version check
project(lsb_steg)              # Create project "lsb_steg"

# Add source files to variable
set(SOURCES main.c decode.c encode.c)

# Add the required header files to variable
set(HEADERS decode.h encode.h types.h common.h)

# Create executable from source files
add_executable(lsb_steg ${SOURCES} ${HEADERS})

# Link the required header files to the include directory
target_link_directories(lsb_steg PUBLIC {CMAKE_CURRENT_SOURCE_DIR})
