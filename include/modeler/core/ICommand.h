#pragma once

/// @brief Interface representing a single undoable/redoable scene operation.
///
/// Concrete command classes implement this interface and encapsulate all
/// state needed to execute and reverse a single user action.
class ICommand
{
  public:
    virtual ~ICommand() = default;

    /// @brief Apply the operation to the scene.
    virtual void execute() = 0;

    /// @brief Reverse the operation previously applied by execute().
    virtual void undo() = 0;
};
