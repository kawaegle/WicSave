#include "unit.hpp"

TEST_F(SaveDataTest, SaveAndLoadInteger)
{
    // Save data
    {
        wicsave::SaveData saver(test_file);
        int value = 42;
        saver.AddData("number", &value, sizeof(value));
        ASSERT_TRUE(saver.Save());
    }

    // Load data
    {
        wicsave::SaveData loader(test_file);
        ASSERT_TRUE(loader.Load());

        const void *data = loader.GetData("number");
        ASSERT_NE(data, nullptr);

        int loaded_value = *static_cast<const int *>(data);
        EXPECT_EQ(loaded_value, 42);
    }
}

TEST_F(SaveDataTest, SaveAndLoadString)
{
    std::string original = "Hello, World!";

    // Save
    {
        wicsave::SaveData saver(test_file);
        saver.AddData("message", (void *)original.c_str(), original.size() + 1); // +1 for null terminator
        ASSERT_TRUE(saver.Save());
    }

    // Load
    {
        wicsave::SaveData loader(test_file);
        ASSERT_TRUE(loader.Load());

        const char *loaded = static_cast<const char *>(loader.GetData("message"));
        ASSERT_NE(loaded, nullptr);
        EXPECT_STREQ(loaded, original.c_str());
    }
}

TEST_F(SaveDataTest, SaveAndLoadMultipleValues)
{
    int health = 100;
    float score = 99.5f;
    std::string name = "Player1";

    // Save
    {
        wicsave::SaveData saver(test_file);
        saver.AddData("health", &health, sizeof(health));
        saver.AddData("score", &score, sizeof(score));
        saver.AddData("name", (void *)name.c_str(), name.size() + 1);
        ASSERT_TRUE(saver.Save());
    }

    // Load
    {
        wicsave::SaveData loader(test_file);
        ASSERT_TRUE(loader.Load());

        int loaded_health = *static_cast<const int *>(loader.GetData("health"));
        float loaded_score = *static_cast<const float *>(loader.GetData("score"));
        const char *loaded_name = static_cast<const char *>(loader.GetData("name"));

        EXPECT_EQ(loaded_health, 100);
        EXPECT_FLOAT_EQ(loaded_score, 99.5f);
        EXPECT_STREQ(loaded_name, "Player1");
    }
}

TEST_F(SaveDataTest, SaveAndLoadBinaryData)
{
    std::vector<unsigned char> binary_data = {0x01, 0x02, 0x03, 0xFF, 0xFE, 0xFD};

    // Save
    {
        wicsave::SaveData saver(test_file);
        saver.AddData("binary", binary_data.data(), binary_data.size());
        ASSERT_TRUE(saver.Save());
    }

    // Load
    {
        wicsave::SaveData loader(test_file);
        ASSERT_TRUE(loader.Load());

        const unsigned char *loaded = static_cast<const unsigned char *>(loader.GetData("binary"));
        ASSERT_NE(loaded, nullptr);

        // Note: You'll need to add a GetDataSize() method or store size separately
        for (size_t i = 0; i < binary_data.size(); ++i) {
            EXPECT_EQ(loaded[i], binary_data[i]);
        }
    }
}

TEST_F(SaveDataTest, OverwriteExistingKey)
{
    wicsave::SaveData saver(test_file);

    int value1 = 42;
    int value2 = 100;

    saver.AddData("key", &value1, sizeof(value1));
    saver.AddData("key", &value2, sizeof(value2)); // Overwrite

    ASSERT_TRUE(saver.Save());

    wicsave::SaveData loader(test_file);
    ASSERT_TRUE(loader.Load());

    int loaded = *static_cast<const int *>(loader.GetData("key"));
    EXPECT_EQ(loaded, 100); // Should have the second value
}

TEST_F(SaveDataTest, SaveAndLoadZeroValue)
{
    int zero = 0;

    {
        wicsave::SaveData saver(test_file);
        saver.AddData("zero", &zero, sizeof(zero));
        ASSERT_TRUE(saver.Save());
    }

    {
        wicsave::SaveData loader(test_file);
        ASSERT_TRUE(loader.Load());

        int loaded = *static_cast<const int *>(loader.GetData("zero"));
        EXPECT_EQ(loaded, 0);
    }
}

TEST_F(SaveDataTest, SaveAndLoadNegativeValue)
{
    int negative = -999;

    {
        wicsave::SaveData saver(test_file);
        saver.AddData("negative", &negative, sizeof(negative));
        ASSERT_TRUE(saver.Save());
    }

    {
        wicsave::SaveData loader(test_file);
        ASSERT_TRUE(loader.Load());

        int loaded = *static_cast<const int *>(loader.GetData("negative"));
        EXPECT_EQ(loaded, -999);
    }
}

TEST_F(SaveDataTest, SaveAndLoadLargeData)
{
    std::vector<int> large_data(10000);
    for (int i = 0; i < 10000; ++i) {
        large_data[i] = i;
    }

    {
        wicsave::SaveData saver(test_file);
        saver.AddData("large", large_data.data(), large_data.size() * sizeof(int));
        ASSERT_TRUE(saver.Save());
    }

    {
        wicsave::SaveData loader(test_file);
        ASSERT_TRUE(loader.Load());

        const int *loaded = static_cast<const int *>(loader.GetData("large"));
        ASSERT_NE(loaded, nullptr);

        for (int i = 0; i < 10000; ++i) {
            EXPECT_EQ(loaded[i], i);
        }
    }
}

TEST_F(SaveDataTest, SaveAndLoadEmptyString)
{
    std::string empty = "";

    {
        wicsave::SaveData saver(test_file);
        saver.AddData("empty", (void *)empty.c_str(), empty.size() + 1);
        ASSERT_TRUE(saver.Save());
    }

    {
        wicsave::SaveData loader(test_file);
        ASSERT_TRUE(loader.Load());

        const char *loaded = static_cast<const char *>(loader.GetData("empty"));
        ASSERT_NE(loaded, nullptr);
        EXPECT_STREQ(loaded, "");
    }
}

// Struct test
struct PlayerData {
    int health;
    int mana;
    float x, y, z;
};

TEST_F(SaveDataTest, SaveAndLoadStruct)
{
    PlayerData player = {100, 50, 1.5f, 2.5f, 3.5f};

    {
        wicsave::SaveData saver(test_file);
        saver.AddData("player", &player, sizeof(player));
        ASSERT_TRUE(saver.Save());
    }

    {
        wicsave::SaveData loader(test_file);
        ASSERT_TRUE(loader.Load());

        const PlayerData *loaded = static_cast<const PlayerData *>(loader.GetData("player"));
        ASSERT_NE(loaded, nullptr);

        EXPECT_EQ(loaded->health, 100);
        EXPECT_EQ(loaded->mana, 50);
        EXPECT_FLOAT_EQ(loaded->x, 1.5f);
        EXPECT_FLOAT_EQ(loaded->y, 2.5f);
        EXPECT_FLOAT_EQ(loaded->z, 3.5f);
    }
}