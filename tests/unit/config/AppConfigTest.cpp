#include <modeler/config/AppConfig.h>

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <string>

// ── Fixture ───────────────────────────────────────────────────────────────────

class AppConfigTest : public ::testing::Test
{
protected:
    std::filesystem::path ini_path_;

    void SetUp() override
    {
        ini_path_ = std::filesystem::temp_directory_path() / "modeler_test_prefs.ini";
        writeTestFile(
            "[DEFAULT]\n"
            "window_colour = 60\n"
            "auto_save = enabled\n"
            "editor_colour_theme = modern\n"
        );
    }

    void TearDown() override
    {
        std::filesystem::remove(ini_path_);
    }

    void writeTestFile(const std::string& content)
    {
        std::ofstream f(ini_path_);
        f << content;
    }
};

// ── getValue ──────────────────────────────────────────────────────────────────

TEST_F(AppConfigTest, GetValue_ExistingKey_ReturnsCorrectValue)
{
    AppConfig cfg(ini_path_);
    EXPECT_EQ(cfg.getValue("DEFAULT", "window_colour"), "60");
}

TEST_F(AppConfigTest, GetValue_BooleanKey_ReturnsValue)
{
    AppConfig cfg(ini_path_);
    EXPECT_EQ(cfg.getValue("DEFAULT", "auto_save"), "enabled");
}

TEST_F(AppConfigTest, GetValue_MissingKey_ReturnsEmptyString)
{
    AppConfig cfg(ini_path_);
    EXPECT_EQ(cfg.getValue("DEFAULT", "nonexistent_key"), "");
}

TEST_F(AppConfigTest, GetValue_MissingFile_ReturnsEmptyString)
{
    AppConfig cfg(std::filesystem::temp_directory_path() / "does_not_exist.ini");
    EXPECT_EQ(cfg.getValue("DEFAULT", "window_colour"), "");
}

// ── setValue ──────────────────────────────────────────────────────────────────

TEST_F(AppConfigTest, SetValue_ExistingKey_ValueIsUpdated)
{
    AppConfig cfg(ini_path_);
    cfg.setValue("DEFAULT", "window_colour", "80");
    EXPECT_EQ(cfg.getValue("DEFAULT", "window_colour"), "80");
}

TEST_F(AppConfigTest, SetValue_DoesNotCorruptOtherKeys)
{
    AppConfig cfg(ini_path_);
    cfg.setValue("DEFAULT", "auto_save", "disabled");
    EXPECT_EQ(cfg.getValue("DEFAULT", "window_colour"), "60");
    EXPECT_EQ(cfg.getValue("DEFAULT", "editor_colour_theme"), "modern");
}

TEST_F(AppConfigTest, SetValue_RoundTrip_PersistsToDisk)
{
    {
        AppConfig cfg(ini_path_);
        cfg.setValue("DEFAULT", "editor_colour_theme", "dark");
    }
    // Re-open to verify the change survived the write
    AppConfig cfg2(ini_path_);
    EXPECT_EQ(cfg2.getValue("DEFAULT", "editor_colour_theme"), "dark");
}

TEST_F(AppConfigTest, PathAccessor_ReturnsCorrectPath)
{
    AppConfig cfg(ini_path_);
    EXPECT_EQ(cfg.path(), ini_path_);
}
