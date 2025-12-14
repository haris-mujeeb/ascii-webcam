# ASCII Webcam

A C++ project to live-stream webcam data as ASCII art in the terminal.

## Overview

This project is a simple C++ application that captures video from a webcam, converts each frame to ASCII art, and prints it to the console. The project is built using CMake and uses OpenCV for webcam access and image manipulation.

## Table of Contents

- [Demo](#demo)
- [Dependencies](#dependencies)
- [Building the Project](#building-the-project)
- [Code Formatting](#code-formatting)
- [How to Run](#how-to-run)
- [Running Tests](#running-tests)
- [Contributing](#contributing)
- [License](#license)
- [Performance Benchmarks](#performance-benchmarks)
- [Documentation](#documentation)

## Demo

Here is a quick demo of the application in action:

![Animation](https://github.com/haris-22/ascii-webcam/assets/56654924/25291583-b883-4893-86a0-4ff667107779)

## Dependencies

Before you begin, ensure you have the following dependencies installed:

- **CMake**: For building the project.
- **OpenCV**: For webcam and image processing.
- **Google Test**: For running tests.
- **clang-format**: For formatting the code.

## Building the Project

```bash
mkdir build
cd build
cmake ..
make
```

## Code Formatting

To format the code, run the following command from the root of the project:

```bash
find . -name "*.cpp" -o -name "*.hpp" | xargs clang-format -i
```

## How to Run

To run the application, execute the following command from the `build` directory:

```bash
./bin/ascii_webcam_app
```

## Running Tests

To run the tests, execute the following command from the `build` directory:

```bash
ctest
```

## Contributing

Contributions are welcome! If you would like to contribute to the project, please follow these steps:

1. Fork the repository.
2. Create a new branch for your feature or bug fix.
3. Make your changes and commit them with a descriptive message.
4. Push your changes to your fork.
5. Create a pull request to the main repository.

Please note that this project is released with a Contributor Code of Conduct. By participating in this project you agree to abide by its terms. Please read our [Code of Conduct](CODE_OF_CONDUCT.md) before contributing.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Performance Benchmarks

This section tracks the performance improvements of the `AnimationStressTest` after applying various optimizations.

| Optimization Step                                                                                                                                           | Average FPS | Improvement |
| :---------------------------------------------------------------------------------------------------------------------------------------------------------- | :---------- | :---------- |
| **Initial Baseline**                                                                                                                                        | **498**     | -           |
| 1. Replaced `std::stringstream` with `sprintf` for ASCII output generation, `strlen(ASCII_CHARS)` optimization, `RawImage` passed by `const&` to animation | **685**      | +37.5%       |
| 2. Optimized terminal output (single clear command, disabled C-style I/O sync)                                                                              | **690**     | (insignificant) |
| 3. Implemented Move Semantics for `RawImage`                                                                                                                | **730**     | +46.6%      |
| 4. Reused Memory Buffers                                                                                                                                    | **737**     | (insignificant) |
| 5. Pre-calculated Sine Table                                                                                                                                | **715**     |(insignificant) |

## Documentation

- [Source Code README](src/README.md)
- [Include Files README](include/README.md)
- [Test Files README](tests/README.md)
- [Image Files README](images/README.md)
- [Output Files README](output/README.md)
- [Third-Party Libraries README](third_party/README.md)
