# AsciiWebcam

A C++ project for converting webcam feeds or images into ASCII art. This project leverages OpenCV for image processing and aims to provide real-time ASCII video capabilities.

## Features

*   Convert static images to ASCII art.
*   (Planned) Real-time webcam feed to ASCII video conversion.
*   (Planned) Customizable ASCII character sets and density.

## Setup for Beginners

Follow these steps to get the project up and running on your local machine.

### Prerequisites

Before you begin, ensure you have the following installed:

*   **C++ Compiler:** A C++17 compatible compiler (e.g., GCC, Clang, MSVC).
*   **CMake:** Version 3.16 or newer.
*   **Git:** For cloning the repository.
*   **OpenCV:** The project will attempt to fetch OpenCV if not found on your system. However, for best performance and control, it's recommended to have OpenCV installed globally.

### Cloning the Repository

First, clone the repository to your local machine:

```bash
git clone <https://github.com/haris-mujeeb/ascii-webcam.git>
cd ascii-webcam
```

### Building the Project

The project uses CMake for its build system.

1.  **Create a build directory:**
    ```bash
    mkdir build
    cd build
    ```
2.  **Configure CMake:**
    ```bash
    cmake ..
    ```
    This command will configure the project and download external dependencies like Google Test and OpenCV (if not found) automatically.

3.  **Build the project:**
    ```bash
    cmake --build .
    ```
    This will compile the source code and generate the executables.

### Running Tests

To verify the build and see an example of functionality, you can run the provided tests:

```bash
./ascii_art_test
```

### Running the Application (Placeholder)

**Note:** As of the current `CMakeLists.txt`, the main application executable for real-time webcam processing is not yet defined or built. This section describes how you would typically run such an application once it's implemented and configured in `CMakeLists.txt`.

Once the main application (e.g., `ascii_webcam_app`) is added to `CMakeLists.txt` and built, you would typically run it from the `build` directory:

```bash
# Example: Running the main application (once implemented)
# ./ascii_webcam_app
```


## Contributing

(This section is a placeholder for future contribution guidelines.)

