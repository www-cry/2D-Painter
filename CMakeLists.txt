cmake_minimum_required(VERSION 3.16)
project(FetchTheGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/msys64/ucrt64")

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window Audio System Network)

add_executable(2DPainter src/main.cpp
src/Canvas.cpp
src/NewFileMenu.cpp
src/PainterMenu.cpp
src/WindowCore.cpp
src/Canvas.h
src/NewFileMenu.h
src/PainterMenu.h
src/WindowCore.h
)

target_link_libraries(2DPainter PRIVATE sfml-graphics sfml-window sfml-audio sfml-system sfml-network)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/MFONT.ttf DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/ToolsImg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})