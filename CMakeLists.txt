cmake_minimum_required(VERSION 3.10)
project(LibrarySys VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 1. Ensure full compatibility with Qt 6.10 and link Widgets
find_package(Qt6 6.2 COMPONENTS Widgets REQUIRED)

# 3. Add all source files, including the new GUI files
add_executable(LibrarySys
    main.cpp
    MainWindow.cpp
    Library.cpp
    Book.cpp
)

# 2. Include the nlohmann/json directory
target_include_directories(LibrarySys PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# 4. Link properly with Qt6::Widgets
target_link_libraries(LibrarySys PRIVATE Qt6::Widgets)
