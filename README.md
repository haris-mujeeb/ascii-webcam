# ASCII Webcam

A C++ project to live-stream webcam data as ASCII art in the terminal.

## Building the Project

```bash
mkdir build
cd build
cmake ..
make
```

## Performance Benchmarks

This section tracks the performance improvements of the `AnimationStressTest` after applying various optimizations.

| Optimization Step                                                                                                                                           | Average FPS | Improvement |
| :---------------------------------------------------------------------------------------------------------------------------------------------------------- | :---------- | :---------- |
| **Initial Baseline**                                                                                                                                        | **498**     | -           |
| 1. Replaced `std::stringstream` with `sprintf` for ASCII output generation, `strlen(ASCII_CHARS)` optimization, `RawImage` passed by `const&` to animation | **685**     | +37.5%       |
| 2. Optimized terminal output (single clear command, disabled C-style I/O sync)                                                                              | **690**     |  (insignificant)      |
| 3. Implemented Move Semantics for `RawImage`                                                                                                                | **730**     | +46.6%      |
| 4. Reused Memory Buffers                                                                                                                                    |     **737**   |   (insignificant)     |
| 5. Pre-calculated Sine Table                                                                                                                                |             |             |
