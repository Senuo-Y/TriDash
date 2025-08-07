cmake_minimum_required(VERSION 3.31)
project(TriDash)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(raylib)
include_directories(raylib/include)
include_directories("C:/opencv/build/include")
link_directories("C:/opencv/build/x64/vc16/lib")

add_executable(TriDash
		include/Button.h
		include/Menu.h
		include/Obstacle.h
		include/ObstacleLine.h
		include/Parameters.h
		include/Player.h
		raylib/include/raylib.h
		raylib/include/raymath.h
		raylib/include/rlgl.h
		src/Button.cpp
		src/Menu.cpp
		src/Obstacle.cpp
		src/ObstacleLine.cpp
		src/Player.cpp
		src/TriDash.cpp)
