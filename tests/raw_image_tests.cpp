#include <gtest/gtest.h>
#include <thread>
#include "raw_image.hpp"

// Helper macro to stringify preprocessor definitions
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

class RawImageTests : public ::testing::Test {
protected:
  void SetUp() override {
    // This will run before each test
  }
  void TearDown() override {
    // This will run after each test
  }
};

TEST_F(RawImageTests, ConstructorInitializesCorrectly) {
  RawImage raw_img(100, 200, 3);
  EXPECT_EQ(raw_img.getSize(), 100 * 200 * 3);
  EXPECT_NE(raw_img.getData(), nullptr);
}

TEST_F(RawImageTests, LoadsImageCorrectly) {
  RawImage raw_img(TOSTRING(IMAGE_FILE_PATH));
  EXPECT_EQ(raw_img.getSize(), raw_img.getHeight() * raw_img.getWidth() * raw_img.getChannels());
  EXPECT_NE(raw_img.getData(), nullptr);
  EXPECT_GT(raw_img.getWidth(), 0);
  EXPECT_GT(raw_img.getHeight(), 0);
  EXPECT_GT(raw_img.getChannels(), 0);
}

TEST_F(RawImageTests, ObjectCountIncrementsAndDecrements) {
  EXPECT_EQ(RawImage::get_live_count(), 0);

  {
    RawImage img1(100, 100, 3);
    EXPECT_EQ(RawImage::get_live_count(), 1);
    {
      RawImage img2(200, 200, 3);    
      EXPECT_EQ(RawImage::get_live_count(), 2);
    }
    EXPECT_EQ(RawImage::get_live_count(), 1);
  }

  EXPECT_EQ(RawImage::get_live_count(), 0) << "Memory Leak Detected: Live objects remain!";
}

TEST_F(RawImageTests,  CopyConstructorCreatesDeepCopy) {
  RawImage img1(100, 200, 3);
  RawImage img2 = img1; // Invoke copy constructor

  EXPECT_EQ(img1.getSize(), img2.getSize());
  EXPECT_NE(img1.getData(), img2.getData()); // Ensure different data pointers
  EXPECT_EQ(img1.getData()[0], img2.getData()[0]);
  EXPECT_EQ(RawImage::get_live_count(), 2);
}

TEST_F(RawImageTests,  CopyAssignmentOperatorCreatesDeepCopy) {
  RawImage img1(100, 200, 3);
  RawImage img2(150, 200, 3);
  img2 = img1; // Invoke copy assigment operator

  EXPECT_EQ(img1.getSize(), img2.getSize());
  EXPECT_NE(img1.getData(), img2.getData()); // Ensure different data pointers
  EXPECT_EQ(img1.getData()[0], img2.getData()[0]);
}

TEST_F(RawImageTests, DestructorFreesMemory) {
    RawImage* image = new RawImage(100, 200, 3);
    delete image; // Ensure it can be deleted without memory leak
    EXPECT_EQ(RawImage::get_live_count(), 0);
}

TEST_F(RawImageTests, MoveConstructor) {
  RawImage source(10, 10, 3);
  // Fill with some data to verify transfer
  if (source.getData()) {
    source.getData()[0] = 123;
  }
  size_t original_size = source.getSize();
  uint8_t* original_data = source.getData();

  EXPECT_EQ(RawImage::get_live_count(), 1);

  RawImage destination = std::move(source); // Invoke move constructor

  // Verify destination has source's original state
  EXPECT_EQ(destination.getSize(), original_size);
  EXPECT_EQ(destination.getData(), original_data);
  if (destination.getData()) {
    EXPECT_EQ(destination.getData()[0], 123);
  }
  
  // Verify source is in a valid, empty state
  EXPECT_EQ(source.getSize(), 0);
  EXPECT_EQ(source.getData(), nullptr);

  // Live object count should remain the same (ownership transferred, not new object created)
  EXPECT_EQ(RawImage::get_live_count(), 1);
}

TEST_F(RawImageTests, MoveAssignmentOperator) {
  RawImage source(10, 10, 3);
  // Fill with some data to verify transfer
  if (source.getData()) {
    source.getData()[0] = 123;
  }
  size_t original_size = source.getSize();
  uint8_t* original_data = source.getData();

  RawImage destination(5, 5, 3); // Existing object
  EXPECT_EQ(RawImage::get_live_count(), 2);
  
  destination = std::move(source); // Invoke move assignment operator

  // Verify destination has source's original state
  EXPECT_EQ(destination.getSize(), original_size);
  EXPECT_EQ(destination.getData(), original_data);
  if (destination.getData()) {
    EXPECT_EQ(destination.getData()[0], 123);
  }
  
  // Verify source is in a valid, empty state
  EXPECT_EQ(source.getSize(), 0);
  EXPECT_EQ(source.getData(), nullptr);

  // Live object count should remain the same (ownership transferred, original destination object destroyed)
  EXPECT_EQ(RawImage::get_live_count(), 1); 
}

// Helper function to create and return a RawImage by value
RawImage createAndReturnRawImage(int width, int height, int channels) {
    return RawImage(width, height, channels);
}

TEST_F(RawImageTests, PerformanceOfReturningRawImage) {
    const int num_iterations = 1000;
    const int img_width = 100;
    const int img_height = 100;
    const int img_channels = 3;

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < num_iterations; ++i) {
        RawImage img = createAndReturnRawImage(img_width, img_height, img_channels);
        // Do something with img to prevent compiler optimizing it away
        EXPECT_EQ(img.getWidth(), img_width);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end_time - start_time;

    std::cout << "\nRawImage creation/return (" << num_iterations << " iterations): "
              << elapsed.count() << " ms" << std::endl;
}
