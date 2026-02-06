#include "WicSave.hpp"
#include "unit.hpp"

TEST_F(SaveDataTest, SaveEmptyData)
{
    {
        wicsave::SaveData saver(test_file);

        ASSERT_TRUE(saver.Save());
    }

    {
        wicsave::SaveData loader(test_file);
        ASSERT_TRUE(loader.Load());

        EXPECT_THROW(loader.GetData("nonexistent"), std::runtime_error);
    }
}

TEST_F(SaveDataTest, LoadNonExistentFile)
{
    wicsave::SaveData loader;
    EXPECT_THROW(loader.Load("nonexistent.dat"), std::runtime_error);
}

TEST_F(SaveDataTest, GetDataWithInvalidKey)
{
    {
        wicsave::SaveData saver(test_file);
        int value = 42;
        saver.AddData("valid_key", &value, sizeof(value));
        ASSERT_TRUE(saver.Save());
    }

    {
        wicsave::SaveData loader(test_file);
        ASSERT_TRUE(loader.Load());

        EXPECT_THROW(loader.GetData("invalid_key"), std::runtime_error);
    }
}

TEST_F(SaveDataTest, DetectCorruptedFile)
{
    int value = 42;

    {
        wicsave::SaveData saver(test_file);
        saver.AddData("number", &value, sizeof(value));
        ASSERT_TRUE(saver.Save());
    }

    {
        std::fstream file(test_file, std::ios::in | std::ios::out | std::ios::binary);
        file.seekp(5);
        char corrupt = 0xFF;
        file.write(&corrupt, 1);
        file.close();
    }

    {
        wicsave::SaveData loader(test_file);
        EXPECT_THROW(loader.Load(), std::runtime_error);
    }
}