#include <gtest/gtest.h>
#include <chrono>
#include <thread>
#include "raw_image.hpp"
#include "ascii_image.hpp"

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

class AsciiImageTests : public ::testing::Test {
  protected:
  void SetUp() override {
  }
  void TearDown() override {
  }
};

TEST_F(AsciiImageTests, getGrayscaleValueTest) {
  EXPECT_NEAR(getGrayscaleValue(100, 100, 100), (29.9 + 58.7 + 11.4), 1);
}

TEST_F(AsciiImageTests, pixelToAsciiTest) {
  EXPECT_EQ(pixelToAscii(getGrayscaleValue(0, 0, 0)), ' ');
  EXPECT_EQ(pixelToAscii(getGrayscaleValue(255, 255, 255)), '$');
}

TEST_F(AsciiImageTests, OuputGrayAsciiToFile) {
  RawImage raw_img(TOSTRING(IMAGE_FILE_PATH));
  EXPECT_NE(raw_img.getData(), nullptr);
  EXPECT_EQ(raw_img.getWidth(), 100);
  EXPECT_EQ(raw_img.getHeight(), 100);
  EXPECT_EQ(raw_img.getChannels(), 3);
  EXPECT_EQ(raw_img.getSize(), raw_img.getHeight() * raw_img.getWidth() * raw_img.getChannels());

  RawImage gray_img = convertToAscii(raw_img);
  EXPECT_NO_THROW(ouputAsciiToFile(gray_img, TOSTRING(OUTPUT_GRAY_TXT_FILE_PATH)));
}

TEST_F(AsciiImageTests, OuputColoredAsciiToFile) {
  RawImage raw_img(TOSTRING(IMAGE_FILE_PATH));
  RawImage colored_img = convertToColoredAscii(raw_img);
  EXPECT_NO_THROW(ouputAsciiToFile(colored_img, TOSTRING(OUTPUT_COLORED_TXT_FILE_PATH)));
}

TEST_F(AsciiImageTests, OuputGrayAsciiToTerminal) {
  RawImage raw_img(TOSTRING(IMAGE_FILE_PATH));
  RawImage gray_img = convertToAscii(raw_img);
  EXPECT_NO_THROW(std::cout << gray_img.getData());
}

TEST_F(AsciiImageTests, OuputColoredAsciiToTerminal) {
  RawImage raw_img(TOSTRING(IMAGE_FILE_PATH));
  RawImage colored_img = convertToColoredAscii(raw_img);
  EXPECT_NO_THROW(std::cout << colored_img.getData());
}

TEST_F(AsciiImageTests, OuputTerminalRainbowAscii) {
  RawImage raw_img(TOSTRING(IMAGE_FILE_PATH));
  RawImage rainbow_img = convertToRainbowAscii(raw_img);
  EXPECT_NO_THROW(std::cout << rainbow_img.getData());
}

// TEST_F(AsciiImageTests, DatasetLoadingStressTest) {
//   RawImage raw_img(TOSTRING(IMAGE_FILE_PATH));
//   const int NUM_ITERATION = 200;
//   auto start = std::chrono::high_resolution_clock::now();
//   int offset = 0;
//   for (int i = 0; i <NUM_ITERATION; i++) {
//     // ANSI command to move cursor to top-left (prevents flickering compared to "clear")
//     std::cout << "\033[H";

//     std::cout << convertToRainbowAscii(raw_img, offset).getData();
//     offset++;
//     // std::this_thread::sleep_for(std::chrono::milliseconds(50));
//   }
//   auto end = std::chrono::high_resolution_clock::now();
//   std::chrono::duration<double, std::milli> elapsed = end - start;

//   // Log the processing time
//   std::cerr << "\n[Benchmarks] Processing Time: " << elapsed.count() << " ms\n";
// }

TEST_F(AsciiImageTests, WebcamLiveStream) {
  size_t FRAMES_TO_PROCESS = 100;
  outputWebcameAsciiStream(FRAMES_TO_PROCESS);
}