cmake_minimum_required(VERSION 3.26)
project(Ludens LANGUAGES C CXX)


# build third-party dependencies
add_subdirectory("Extra/glfw")
add_subdirectory("Extra/glad")


# build core modules
add_subdirectory("Ludens/Core/Header")
add_subdirectory("Ludens/Core/OS")
add_subdirectory("Ludens/Core/Application")
add_subdirectory("Ludens/Core/Serialize")
