#include "ascii_image.hpp"
#include <cstring> // For strlen

const char* ASCII_CHARS = " .`,:\"^`_-\'!Ii><~+*jftrxunvczXYUJCLQ0OZmwdbqkhao*#MW&8B%@$";


int getGrayscaleValue(uint8_t r, uint8_t g, uint8_t b) {
  return static_cast<int>(0.299 * r + 0.587 * g + 0.114 * b);
}

char pixelToAscii(int grayValue) {
  int index = (grayValue * (strlen(ASCII_CHARS)-1)) / 255;\
  return ASCII_CHARS[index];
}

RawImage convertToAscii(const RawImage &source_image) {
  int width = source_image.getWidth();
  int height = source_image.getHeight();
  int channels = 1; // For 1 grayscaled ascii code per pixel
  int size = source_image.getSize();
  uint8_t* source_data = source_image.getData();
  
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

RawImage convertToRainbowAscii(const RawImage& img, int scroll_offset) { // Removed default arg
  int width = img.getWidth();
  int height = img.getHeight();
  uint8_t* data = img.getData();
  
  // Estimate size: 20 bytes per pixel (ANSI + char) + newline per row + footer + null
  size_t estimated_size = height * (width * 20 + 1) + 16;
  RawImage target(estimated_size, 1, 1);

  std::stringstream buffer;

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      size_t pixelIndex = (y * width + x) * 3;  // RGB data assumes 3 channels
      uint8_t red = data[pixelIndex];
      uint8_t green = data[pixelIndex + 1];
      uint8_t blue = data[pixelIndex + 2];
      
      int grayValue = getGrayscaleValue(red, green, blue);
      char asciiChar = pixelToAscii(grayValue);

      getRainbowColor(x, y, scroll_offset, red, green, blue);

      buffer << "\033[38;2;" 
            << (int)red << ";" << (int)green << ";" << (int)blue << "m" 
            << asciiChar; 
    }
  
    buffer << std::endl; // New line after each row
  }
  // Reset color at end
  buffer << "\033[0m";

  std::string str = buffer.str();
  size_t len = str.length();
  if (len >= target.getSize()) len = target.getSize() - 1;
  std::memcpy(target.getData(), str.c_str(), len);
  target.getData()[len] = '\0';

  return target;
}

RawImage convertToColoredAscii(const RawImage &source_image) {
  int width = source_image.getWidth();
  int height = source_image.getHeight();
  uint8_t* data = source_image.getData();
  
  // Estimate size: 20 bytes per pixel (ANSI + char) + newline per row + footer + null
  size_t estimated_size = height * (width * 20 + 1) + 16;
  RawImage target(estimated_size, 1, 1);

  std::stringstream buffer;

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      size_t pixelIndex = (y * width + x) * 3;  // RGB data assumes 3 channels
      uint8_t r = data[pixelIndex];
      uint8_t g = data[pixelIndex + 1];
      uint8_t b = data[pixelIndex + 2];
      
      int grayValue = getGrayscaleValue(r, g, b);
      char asciiChar = pixelToAscii(grayValue);

      buffer << "\033[38;2;" 
            << (int)r << ";" << (int)g << ";" << (int)b << "m" 
            << asciiChar; 
    }
  
    buffer << std::endl; // New line after each row
  }
  // Reset color at end
  buffer << "\033[0m";

  std::string str = buffer.str();
  size_t len = str.length();
  if (len >= target.getSize()) len = target.getSize() - 1;
  std::memcpy(target.getData(), str.c_str(), len);
  target.getData()[len] = '\0';

  return target;
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
    
    std::cout << "\033[H\033[J";  // ANSI escape code to clear screen
    std::cout << "\033[H" << convertToColoredAscii(img).getData() << std::flush;

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