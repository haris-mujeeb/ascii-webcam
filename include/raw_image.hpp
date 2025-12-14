#ifndef RAW_IMAGE_HPP
#define RAW_IMAGE_HPP

#include <cstdint>
#include <cstddef>
#include <cstring>

class RawImage
{
private:
  int m_width, m_height, m_channels ;
  size_t m_size;
  uint8_t* m_data = nullptr;
  static inline int s_live_objects = 0;
public:
  RawImage(int width, int height, int channels);
  RawImage(int width, int height, int channels, const uint8_t* data);
  RawImage(const char* filename);
  ~RawImage();
  
  RawImage(const RawImage &other); // Copy constructor
  RawImage& operator= (const RawImage &other); // Copy assignement
  size_t getSize() const { return m_size; }
  int getWidth() const { return m_width; }
  int getHeight() const { return m_height;}
  int getChannels() const { return m_channels;}
  uint8_t* getData() const { return m_data; }
  static int get_live_count() { return s_live_objects; }
};

#endif // RAW_IMAGE_HPP