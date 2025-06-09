cmake_minimum_required(VERSION 3.16)

# Set project name
# Set project name
project(PROJECTMARIO)

# Add all .cpp files in the src/ folder
# Add all .cpp files in the src/ folder
file(GLOB_RECURSE SOURCES src/*.cpp)

# Add all header files in the inc/ folder
# Add all header files in the inc/ folder
include_directories(inc/)

# Include Raylib and RayGUI headers
include_directories(${CMAKE_SOURCE_DIR}/raylib/src)

# Add Raylib as a subdirectory (assumes you have raylib built properly)
add_subdirectory(raylib)

# Create the executable from the source files
add_executable(${PROJECT_NAME} ${SOURCES})

# Link Raylib (RayGUI is header-only, so no need to link it as a library)
# Link Raylib (RayGUI is header-only, so no need to link it as a library)
target_link_libraries(${PROJECT_NAME} raylib)

# Copy the resources folder to the build directory for keeping images, sounds, etc.
# Copy the resources folder to the build directory for keeping images, sounds, etc.
file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})