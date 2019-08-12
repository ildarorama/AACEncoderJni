cmake_minimum_required(VERSION 3.10)
project(repack)

include_directories(/usr/local/Cellar/ffmpeg/3.4.2/include)
link_directories(/usr/local/Cellar/ffmpeg/3.4.2/lib)
link_libraries(avcodec avformat     )

set(CMAKE_CXX_STANDARD 11)

add_executable(repack main.cpp)