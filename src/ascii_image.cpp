#include "ascii_image.hpp"
#include <stdexcept>
#include <cstdio>   // For sprintf
#include <string> // For std::string, std::to_string
#include <chrono> // For std::chrono
#include <thread> // For std::this_thread::sleep_for
#include <iomanip> // For std::setprecision


// Precompile tables to avoid calculations
const char* ASCII_CHARS = " .`,:\"^`_-\'!Ii><~+*jftrxunvczXYUJCLQ0OZmwdbqkhao*#MW&8B%@$";
const int ASCII_TABLE_SIZE = 256;

struct AsciiTable
{
  char data[ASCII_TABLE_SIZE];
  AsciiTable() {
    int len = std::strlen(ASCII_CHARS) - 1;
    for (int i = 0; i < ASCII_TABLE_SIZE; ++i) {
      data[i] = ASCII_CHARS[ (i * len) / (ASCII_TABLE_SIZE -1)];
    }
  }
};
static const AsciiTable ASCII_LUT;

static const size_t ASCII_CHARS_LEN = strlen(ASCII_CHARS);



int getGrayscaleValue(uint8_t r, uint8_t g, uint8_t b) {
  return (299 * static_cast<int>(r) + 587 * static_cast<int>(g) + 114 * static_cast<int>(b)) / 1000;
}

char pixelToAscii(int grayValue) {
  if (grayValue < 0 ) grayValue = 0;
  if (grayValue > 255 ) grayValue = 255;
  return ASCII_LUT.data[grayValue];
}

RawImage convertToAscii(const RawImage &source_image) {
  int width = source_image.getWidth();
  int height = source_image.getHeight();
  int channels = 1; // For 1 grayscaled ascii code per pixel
  int size = source_image.getSize();
  const uint8_t* source_data = source_image.getData();
  
  RawImage target_image(width + height, height, 1); // 1 extra row for '\n'
  uint8_t* target_data = target_image.getData();
  
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      size_t pixelIndex = (y * width + x) * 3;  // RGB data assumes 3 channels
      uint8_t r = source_data[pixelIndex];
      uint8_t g = source_data[pixelIndex + 1];
      uint8_t b = source_data[pixelIndex + 2];
      
      int grayValue = getGrayscaleValue(r, g, b);
      target_data[pixelIndex / 3] = pixelToAscii(grayValue);
    }
    target_data[(y * width) + 1] = '\n'; // New line after each row
  }
  return target_image;
}


void getRainbowColor(int width, int height, int scroll_offset, 
                    uint8_t& red, uint8_t& green, uint8_t& blue) {
  double freq = 0.1;
  double i = (width + height) + scroll_offset;
  red = static_cast<uint8_t>(std::sin(freq * i + 0) * 127 + 128);
  green = static_cast<uint8_t>(std::sin(freq * i + 2) * 127 + 128);
  blue = static_cast<uint8_t>(std::sin(freq * i + 4) * 127 + 128);
}

void convertToRainbowAscii(const RawImage& img, int scroll_offset, RawImage& target) {
  int width = img.getWidth();
  int height = img.getHeight();
  const uint8_t* data = img.getData();
  
  char* buffer = reinterpret_cast<char*>(target.getData());
  char* p = buffer;

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      size_t pixelIndex = (y * width + x) * 3;  // RGB data assumes 3 channels
      uint8_t r = data[pixelIndex];
      uint8_t g = data[pixelIndex + 1];
      uint8_t b = data[pixelIndex + 2];
      
      int grayValue = getGrayscaleValue(r, g, b);
      char asciiChar = pixelToAscii(grayValue);

      getRainbowColor(x, y, scroll_offset, r, g, b);

      // Using sprintf is much faster than stringstream or std::string concatenation
      p += sprintf(p, "\033[38;2;%d;%d;%dm%c", r, g, b, asciiChar);
    }  
    *p++ = '\n';
  }
  // Reset color at end
  sprintf(p, "\033[0m");

}

void convertToColoredAscii(const RawImage &source_image, RawImage& target) {
  int width = source_image.getWidth();
  int height = source_image.getHeight();
  const uint8_t* data = source_image.getData();
  
  char* buffer = reinterpret_cast<char*>(target.getData());
  char* p = buffer;

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      size_t pixelIndex = (y * width + x) * 3;  // RGB data assumes 3 channels
      uint8_t r = data[pixelIndex];
      uint8_t g = data[pixelIndex + 1];
      uint8_t b = data[pixelIndex + 2];
      
      int grayValue = getGrayscaleValue(r, g, b);
      char asciiChar = pixelToAscii(grayValue);

      p += sprintf(p, "\033[38;2;%d;%d;%dm%c", r, g, b, asciiChar);
    }
  
    *p++ = '\n';
  }
  // Reset color at end
  sprintf(p, "\033[0m");
}

void outputAsciiToFile(const RawImage &img, const char* output_filename) {
  std::ofstream outputFile(output_filename, std::ios::trunc) ;
  
  if (!outputFile) {
    throw std::runtime_error("Error opening file!");
  }

  std::streambuf* originalCoutBuffer = std::cout.rdbuf();

  std::cout.rdbuf(outputFile.rdbuf());

  std::cout << img.getData();
  
  std::cout.rdbuf(originalCoutBuffer);
}


void outputWebcameAsciiStream(size_t FRAMES_TO_PROCESS) {
  
  cv::VideoCapture cap(0);
  if (!cap.isOpened()) {
    throw std::runtime_error("Error: Could not open webcame.");
  }
  
  cv::Mat frame, resized_frame;

  // Disable synchronization with C-style I/O for faster terminal output
  std::ios::sync_with_stdio(false);
  std::cin.tie(NULL);

  // Estimate size: max 19 bytes per pixel (\033[38;2;255;255;255mC) + newline + footer + null
  int initial_width = 100; // Assuming this as max width
  int initial_height = 100 * 0.55; // Assuming this as max height with aspect ratio
  size_t estimated_size = initial_height * (initial_width * 20 + 1) + 16;
  RawImage buffer_image(estimated_size, 1, 1);

  // calculating average frame rate
  unsigned int current_fps = 0, avg_fps = 0; 
  
  for (int i = 0; i < FRAMES_TO_PROCESS; i++){
    auto start = std::chrono::high_resolution_clock::now();  

    cap >> frame;
    if (frame.empty()) {
      throw std::runtime_error("Error: No frame captured from the webcam.");
    }
    
    // Resize to fit terminal width (e.g., 100 chars)
    // We multiply height by 0.55 to account for the rectangular shape of terminal characters
    int new_width = 100;
    int new_height = static_cast<int>(frame.rows * (static_cast<float>(new_width) / (frame.cols) * 0.55f));
    cv::resize(frame, resized_frame, cv::Size(new_width, new_height));
    
    // OpenCV uses BGR by default, convert to RGB for RawImage
    cv::cvtColor(resized_frame, resized_frame, cv::COLOR_BGR2RGB);
    
    // Create RawImage from the resized OpenCV matrix data
    RawImage img(resized_frame.cols, resized_frame.rows, resized_frame.channels(), resized_frame.data);
    
    std::cout << "\033[H\033[2J";  // ANSI escape code to clear screen and move cursor home
    convertToColoredAscii(img, buffer_image);
    std::cout << buffer_image.getData() << std::flush;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;

    current_fps = static_cast<int>(1000 / elapsed.count());
    std::string fps_str = std::to_string(current_fps);
    
    std::cout << "Frame Rate: " <<  fps_str << std::endl;
    avg_fps += current_fps;
  }
  
  avg_fps /= FRAMES_TO_PROCESS;
  
  std::string avg_fps_str = std::to_string(avg_fps);
  std::cout << "Avg. Frame Rate: " <<  avg_fps_str << std::endl;
}


void outputRainbowAsciiAnimation(const RawImage& img, size_t FRAMES_TO_PROCESS) {
  // Disable synchronization with C-style I/O for faster terminal output
  std::ios::sync_with_stdio(false);
  std::cin.tie(NULL);
  
  // Create the RawImage object once

  // Estimate size: max 19 bytes per pixel (\033[38;2;255;255;255mC) + newline + footer + null
  int width = img.getWidth();
  int height = img.getHeight();
  size_t estimated_size = height * (width * 20 + 1) + 16;

  RawImage buffer_image(estimated_size, 1, 1);

  // calculating average frame rate
  unsigned int current_fps = 0, avg_fps = 0; 
  
  for (int i = 0; i < FRAMES_TO_PROCESS; i++){
    auto start = std::chrono::high_resolution_clock::now();

    std::cout << "\033[H\033[2J";  // ANSI escape code to clear screen and move cursor home
    
    convertToRainbowAscii(img, i, buffer_image);

    std::cout << buffer_image.getData() << std::flush;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;

    current_fps = static_cast<int>(1000 / elapsed.count());
    std::string fps_str = std::to_string(current_fps);

    std::cout << "Frame Rate: " <<  fps_str << std::endl;
    avg_fps += current_fps;
  }
  
  avg_fps /= FRAMES_TO_PROCESS;
  
  std::string avg_fps_str = std::to_string(avg_fps);
  std::cout << "Avg. Frame Rate: " <<  avg_fps_str << std::endl;
}