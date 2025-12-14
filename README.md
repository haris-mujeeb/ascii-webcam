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

| Optimization Step                  | Average FPS | Improvement |
|------------------------------------|-------------|-------------|
| **Initial Baseline**               | **498**     | -           |
| 1. Replaced `stringstream`         | **685**     | +37.5%      |
| 2. Implemented Move Semantics      |             |             |
| 3. Reused Memory Buffers           |             |             |
| 4. Pre-calculated Sine Table       |             |             |
