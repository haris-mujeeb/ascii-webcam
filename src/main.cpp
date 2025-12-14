#include "ascii_image.hpp"
#include <chrono>
#include <iostream>
#include <string>

int main() {

  size_t FRAMES_TO_PROCESS = 65535; // Will run for 36 minutes and 24.5 seconds
  outputWebcameAsciiStream(FRAMES_TO_PROCESS);

  return 0;
}