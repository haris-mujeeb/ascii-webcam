#ifndef ASCII_IMAGE_HPP
#define ASCII_IMAGE_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstring>
#include "raw_image.hpp"
#include <opencv2/opencv.hpp>

extern const char* ASCII_CHARS;

int getGrayscaleValue(uint8_t r, uint8_t g, uint8_t b);
char pixelToAscii(int grayValue);

RawImage convertToAscii(const RawImage &source_image) ;


void getRainbowColor(int width, int height, int scroll_offset, 
                    uint8_t& red, uint8_t& green, uint8_t& blue) ;

RawImage convertToRainbowAscii(const RawImage& img, int scroll_offset) ;


RawImage convertToColoredAscii(const RawImage &source_image) ;


void outputAsciiToFile(const RawImage &img, const char* output_filename) ;


void outputRainbowAsciiAnimation(const RawImage& img, size_t FRAMES_TO_PROCESS);


void outputWebcameAsciiStream(size_t FRAMES_TO_PROCESS);
  
#endif // ASCII_IMAGE_HPP