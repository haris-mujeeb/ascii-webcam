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
  std::memcpy(m_data, data, m_size);
  s_live_objects++;
}

RawImage::RawImage(const char* filename) {
    uint8_t* temp_data = stbi_load(filename, &m_width, &m_height, &m_channels, 0);
    if (!temp_data) {
        throw std::runtime_error("Failed to load image");
    }

    try {
        m_size = static_cast<size_t>(m_width * m_height * m_channels);
        m_data = new uint8_t[m_size];
        std::memcpy(m_data, temp_data, m_size);
        s_live_objects++;
    } catch (...) {
        stbi_image_free(temp_data); // Ensure temp_data is freed on exception
        throw; // Re-throw the exception
    }
    stbi_image_free(temp_data); // Free the original image
} 
  
RawImage::RawImage(const RawImage &other) 
: m_width(other.m_width), m_height(other.m_height), m_channels(other.m_channels), m_size(other.m_size) {
  m_data = new uint8_t[m_size];
  std::memcpy(m_data, other.m_data, m_size);
  s_live_objects++;
}

RawImage& RawImage::operator= (const RawImage &other)
{
  RawImage temp(other); // Create a temporary copy
  std::swap(m_width, temp.m_width);
  std::swap(m_height, temp.m_height);
  std::swap(m_channels, temp.m_channels);
  std::swap(m_size, temp.m_size);
  std::swap(m_data, temp.m_data);
  // No change to s_live_objects here as resources are swapped, not newly allocated/deleted in this object.
  // The temp object's destructor will handle the decrement for the old resources.
  return *this;
}

RawImage::~RawImage()
{
  if(m_data) {
    delete[] m_data;
    s_live_objects--;
  }
}


RawImage::RawImage(RawImage &&other) noexcept 
: m_width(other.m_width), m_height(other.m_height), m_channels(other.m_channels), 
m_size(other.m_size), m_data(other.m_data) { // Corrected: m_data(other.m_data)
  other.m_data = nullptr; // Transfering the ownership
  other.m_width = 0;
  other.m_height = 0;
  other.m_channels = 0;
  other.m_size = 0;
  // s_live_objects--; // Removed decrement
}
  
RawImage& RawImage::operator= (RawImage &&other) noexcept {
  
  if (this != &other) {
  
    delete[] m_data;    // Delete current resources
  
    // Transfer ownership from 'other'
  
    m_data = other.m_data;    
  
    m_size = other.m_size;    
  
    m_width = other.m_width;    
  
    m_height = other.m_height;    
  
    m_channels = other.m_channels;    
  

  
    // Reset 'other' to a valid, empty state
  
    other.m_data = nullptr; 
  
    other.m_size = 0;
  
    other.m_width = 0;
  
    other.m_height = 0;
  
    other.m_channels = 0;
  
    // No change to s_live_objects for 'other' as its resources are moved, not destroyed.
  
  }
  
  return *this;
  
}