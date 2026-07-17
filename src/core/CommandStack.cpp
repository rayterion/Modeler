#include <modeler/core/CommandStack.h>

#include <cassert>

void CommandStack::push(std::unique_ptr<ICommand> command)
{
    assert(command != nullptr);
    command->execute();
    undo_stack_.push_back(std::move(command));
    redo_stack_.clear(); // new action discards the redo history
}

void CommandStack::undo()
{
    if (undo_stack_.empty())
        return;
    undo_stack_.back()->undo();
    redo_stack_.push_back(std::move(undo_stack_.back()));
    undo_stack_.pop_back();
}

void CommandStack::redo()
{
    if (redo_stack_.empty())
        return;
    redo_stack_.back()->execute();
    undo_stack_.push_back(std::move(redo_stack_.back()));
    redo_stack_.pop_back();
}

bool CommandStack::canUndo() const noexcept
{
    return !undo_stack_.empty();
}

bool CommandStack::canRedo() const noexcept
{
    return !redo_stack_.empty();
}

void CommandStack::clear() noexcept
{
    undo_stack_.clear();
    redo_stack_.clear();
}
