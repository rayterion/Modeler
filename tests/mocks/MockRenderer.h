#include <modeler/core/IRenderer.h>
#include <gmock/gmock.h>

/// @brief Google Mock implementation of IRenderer for use in unit tests.
///
/// Inject a MockRenderer wherever an IRenderer* is required to verify
/// that the correct renderer methods are called without starting a GUI.
class MockRenderer : public IRenderer
{
public:
    MOCK_METHOD(void, initRender, (), (override));
    MOCK_METHOD(void, drawProjectScene, (const std::string& file_name), (override));
    MOCK_METHOD(void, sendProjectTo, (const std::string& folder_path), (override));
    MOCK_METHOD(void, undo, (), (override));
    MOCK_METHOD(void, redo, (), (override));
    MOCK_METHOD(void, cut, (), (override));
    MOCK_METHOD(void, copy, (), (override));
    MOCK_METHOD(void, paste, (), (override));
};
