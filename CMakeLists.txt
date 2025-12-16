cmake_minimum_required(VERSION 3.16)
project(AmongUsGUI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/SFML-3.0.2/include)

# Link directories
link_directories(${CMAKE_SOURCE_DIR}/SFML-3.0.2/lib)

# Add executable
add_executable(among_us_gui main.cpp)

# Link SFML libraries directly by name
target_link_libraries(among_us_gui 
    sfml-graphics
    sfml-window
    sfml-system
)

# Copy fonts folder to build directory
add_custom_command(TARGET among_us_gui POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/fonts
    $<TARGET_FILE_DIR:among_us_gui>/fonts
)

# Copy SFML DLLs to build directory
add_custom_command(TARGET among_us_gui POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/SFML-3.0.2/bin
    $<TARGET_FILE_DIR:among_us_gui>
)

# Set output directory
set_target_properties(among_us_gui PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

message(STATUS "Project configured successfully!")
message(STATUS "SFML include: ${CMAKE_SOURCE_DIR}/SFML-3.0.2/include")
message(STATUS "SFML lib: ${CMAKE_SOURCE_DIR}/SFML-3.0.2/lib")