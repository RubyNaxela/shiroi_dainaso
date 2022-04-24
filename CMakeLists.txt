cmake_minimum_required(VERSION 3.15)
project(shiroi_dainaso C)

add_compile_options("-v")

set(CMAKE_C_STANDARD 11)

include_directories(shiroi_dainaso /Library/CSFML-2.5.1/include)
link_directories(shiroi_dainaso /Library/CSFML-2.5.1/lib)

add_executable(shiroi_dainaso main.c)

target_link_libraries(shiroi_dainaso csfml-audio csfml-graphics csfml-system csfml-window)