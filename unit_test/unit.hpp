#include "WicSave.hpp"
#include "gtest/gtest.h"

class SaveDataTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        // Create a temporary test file path
        test_file = std::filesystem::temp_directory_path() / "test_save.dat";

        // Clean up any existing test file
        if (std::filesystem::exists(test_file)) {
            std::filesystem::remove(test_file);
        }
    }

    void TearDown() override
    {
        // Clean up test file after each test
        if (std::filesystem::exists(test_file)) {
            std::filesystem::remove(test_file);
        }
    }

    std::filesystem::path test_file;
};