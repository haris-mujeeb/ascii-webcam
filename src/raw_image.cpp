#include "raw_image.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdexcept>


RawImage::RawImage(int width, int height, int channels) 
: m_width(width), m_height(height), m_channels(channels) {
  m_size = static_cast<size_t>(m_width * m_height * m_channels);
  m_data = new uint8_t[m_size];
  s_live_objects++;
}

RawImage::RawImage(int width, int height, int channels, const uint8_t* data)
: m_width(width), m_height(height), m_channels(channels) {
  m_size = static_cast<size_t>(m_width * m_height * m_channels);
  m_data = new uint8_t[m_size];
  std::memcmp(m_data, data, m_size);
  s_live_objects++;
}

RawImage::RawImage(const char* filename) {
    uint8_t* temp_data = stbi_load(filename, &m_width, &m_height, &m_channels, 0);
    if (!temp_data) {
        throw std::runtime_error("Failed to load image");
    }

    m_size = static_cast<size_t>(m_width * m_height * m_channels);
    m_data = new uint8_t[m_size];
    std::memcpy(m_data, temp_data, m_size);
    stbi_image_free(temp_data); // Free the original image

    s_live_objects++;
} 
  
RawImage::RawImage(const RawImage &other) 
: m_width(other.m_width), m_height(other.m_height), m_channels(other.m_channels), m_size(other.m_size) {
  m_data = new uint8_t[m_size];
  std::memcpy(m_data, other.m_data, m_size);
  s_live_objects++;
}

RawImage& RawImage::operator= (const RawImage &other)
{
  if (this != &other) { // self-assignment check
    delete[] m_data;    // free current memory

    // Copy from other
    m_width = other.m_width; 
    m_height = other.m_height;
    m_channels = other.m_channels;
    m_size = other.m_size;

    m_data = new uint8_t[m_size];   // Allocate new memory
    std::memcpy(m_data, other.m_data, m_size);  // Copy data from other
  }
  return *this; // Return current object by reference
}

RawImage::~RawImage()
{
  if(m_data) {
    delete[] m_data;
    s_live_objects--;
  }
}
