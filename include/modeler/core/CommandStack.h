#pragma once
#include <modeler/core/ICommand.h>
#include <memory>
#include <vector>

/// @brief Manages a linear undo/redo history of @ref ICommand instances.
///
/// Pushing a new command after an undo discards the redo stack (standard
/// linear history model).
class CommandStack
{
public:
    /// @brief Execute a command and push it onto the undo stack.
    ///
    /// Any commands currently on the redo stack are discarded.
    /// @param command The command to execute. Must not be null.
    void push(std::unique_ptr<ICommand> command);

    /// @brief Undo the most recent command.
    ///
    /// No-op if the undo stack is empty.
    void undo();

    /// @brief Redo the most recently undone command.
    ///
    /// No-op if the redo stack is empty.
    void redo();

    /// @brief Returns true if there are commands available to undo.
    [[nodiscard]] bool canUndo() const noexcept;

    /// @brief Returns true if there are commands available to redo.
    [[nodiscard]] bool canRedo() const noexcept;

    /// @brief Discard all undo and redo history.
    void clear() noexcept;

private:
    std::vector<std::unique_ptr<ICommand>> undo_stack_;
    std::vector<std::unique_ptr<ICommand>> redo_stack_;
};
