cmake_minimum_required(VERSION 3.26)
project(Ludens LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# build third-party dependencies
add_subdirectory("Extra/glfw")
add_subdirectory("Extra/glad")


# build core modules
add_subdirectory("Ludens/Core/Header")
add_subdirectory("Ludens/Core/Math")
add_subdirectory("Ludens/Core/DSA")
add_subdirectory("Ludens/Core/OS")
add_subdirectory("Ludens/Core/IO")
add_subdirectory("Ludens/Core/Application")
add_subdirectory("Ludens/Core/RenderBase")
add_subdirectory("Ludens/Core/Serialize")
