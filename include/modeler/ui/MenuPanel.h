#pragma once

/// @brief Identifies which top-level menu panel is currently open.
///
/// Replaces the legacy magic-integer `child_window_info` field in
/// MenuButtons and its dependents.
enum class MenuPanel : int
{
    None = -1,  ///< No panel is open.
    File = 0,   ///< FILE menu panel.
    Modify = 1, ///< MODIFY menu panel.
    Window = 2, ///< WINDOW menu panel.
    Help = 3,   ///< HELP menu panel.
};
