# LibrarySys: A Modern C++ Qt Library Management System

## Overview

**LibrarySys** is a cross-platform desktop application designed for efficient management of a book library. Developed in **C++** and utilizing the **Qt 6 Widgets** framework, this project provides a clean, functional Graphical User Interface (GUI) for all core library operations. Data persistence is handled using the popular **nlohmann/json** header-only library, ensuring all book records are automatically saved to and loaded from a `library.json` file.

## Features

The application provides a comprehensive set of features for library management:

*   **Add Book:** Easily add new books with unique ID, title, author, category, and price.
*   **Delete Book:** Remove books from the library by their unique ID.
*   **Borrow/Return:** Track the status of books, allowing users to borrow and return items.
*   **Search Functionality:** Quickly find books by their unique ID or by a case-insensitive substring search of the title.
*   **Persistent Storage:** Automatically loads data from `library.json` on startup and saves data upon exit.
*   **Modern Toolchain:** Built using **CMake** for a robust and standard build process.

## Prerequisites

To build and run this project, you will need:

*   A C++ compiler (e.g., GCC, Clang) supporting C++17 or later.
*   **CMake** (version 3.10 or higher).
*   **Qt 6** development libraries (version 6.2 or higher is recommended, compatible with 6.10).

## Building the Project

Follow these steps to build the application using CMake:

1.  **Navigate to the project directory:**
    ```bash
    cd LibrarySys
    ```

2.  **Create a build directory and navigate into it:**
    ```bash
    mkdir build
    cd build
    ```

3.  **Configure the project with CMake:**
    ```bash
    cmake ..
    ```
    *Note: CMake will automatically find your installed Qt 6 libraries.*

4.  **Build the executable:**
    ```bash
    make
    ```

## Running the Application

Once built, the executable will be located in the `build` directory.

```bash
./LibrarySys
