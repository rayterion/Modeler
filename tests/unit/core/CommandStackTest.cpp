#include <modeler/core/CommandStack.h>
#include <modeler/core/ICommand.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <vector>

// ── Test double ───────────────────────────────────────────────────────────────

/// Records how many times execute() and undo() have been called.
class SpyCommand : public ICommand
{
public:
    int execute_count = 0;
    int undo_count    = 0;

    void execute() override { ++execute_count; }
    void undo()    override { ++undo_count; }
};

// ── Helper ────────────────────────────────────────────────────────────────────

static std::pair<CommandStack, SpyCommand*> makeStackWithSpy()
{
    CommandStack stack;
    auto spy = std::make_unique<SpyCommand>();
    SpyCommand* raw = spy.get();
    stack.push(std::move(spy));
    return {std::move(stack), raw};
}

// ── push ──────────────────────────────────────────────────────────────────────

TEST(CommandStackTest, Push_ExecutesCommandImmediately)
{
    auto [stack, spy] = makeStackWithSpy();
    EXPECT_EQ(spy->execute_count, 1);
}

TEST(CommandStackTest, Push_MakesUndoAvailable)
{
    auto [stack, spy] = makeStackWithSpy();
    EXPECT_TRUE(stack.canUndo());
}

TEST(CommandStackTest, Push_DiscardsRedoStack)
{
    CommandStack stack;

    auto cmd1 = std::make_unique<SpyCommand>();
    stack.push(std::move(cmd1));
    stack.undo();
    EXPECT_TRUE(stack.canRedo());

    auto cmd2 = std::make_unique<SpyCommand>();
    stack.push(std::move(cmd2));
    EXPECT_FALSE(stack.canRedo());
}

// ── undo ──────────────────────────────────────────────────────────────────────

TEST(CommandStackTest, Undo_CallsUndoOnCommand)
{
    auto [stack, spy] = makeStackWithSpy();
    stack.undo();
    EXPECT_EQ(spy->undo_count, 1);
}

TEST(CommandStackTest, Undo_MakesRedoAvailable)
{
    auto [stack, spy] = makeStackWithSpy();
    stack.undo();
    EXPECT_TRUE(stack.canRedo());
}

TEST(CommandStackTest, Undo_EmptyStack_IsNoOp)
{
    CommandStack stack;
    EXPECT_NO_THROW(stack.undo());
}

TEST(CommandStackTest, Undo_RemovesCommandFromUndoStack)
{
    auto [stack, spy] = makeStackWithSpy();
    stack.undo();
    EXPECT_FALSE(stack.canUndo());
}

// ── redo ──────────────────────────────────────────────────────────────────────

TEST(CommandStackTest, Redo_CallsExecuteAgain)
{
    auto [stack, spy] = makeStackWithSpy();
    stack.undo();
    stack.redo();
    EXPECT_EQ(spy->execute_count, 2);
}

TEST(CommandStackTest, Redo_RestoresUndoAvailability)
{
    auto [stack, spy] = makeStackWithSpy();
    stack.undo();
    stack.redo();
    EXPECT_TRUE(stack.canUndo());
}

TEST(CommandStackTest, Redo_EmptyStack_IsNoOp)
{
    CommandStack stack;
    EXPECT_NO_THROW(stack.redo());
}

// ── clear ─────────────────────────────────────────────────────────────────────

TEST(CommandStackTest, Clear_DiscardsAllHistory)
{
    CommandStack stack;
    auto cmd = std::make_unique<SpyCommand>();
    stack.push(std::move(cmd));
    stack.undo();

    stack.clear();

    EXPECT_FALSE(stack.canUndo());
    EXPECT_FALSE(stack.canRedo());
}

// ── multi-step sequence ───────────────────────────────────────────────────────

TEST(CommandStackTest, MultiStep_UndoRedoSequence_Correct)
{
    CommandStack stack;
    std::vector<SpyCommand*> spies;

    for (int i = 0; i < 3; ++i)
    {
        auto cmd = std::make_unique<SpyCommand>();
        spies.push_back(cmd.get());
        stack.push(std::move(cmd));
    }

    // Undo all three
    for (int i = 0; i < 3; ++i)
        stack.undo();

    EXPECT_FALSE(stack.canUndo());
    EXPECT_TRUE(stack.canRedo());

    // Redo two
    stack.redo();
    stack.redo();

    EXPECT_TRUE(stack.canUndo());
    EXPECT_TRUE(stack.canRedo());

    EXPECT_EQ(spies[0]->undo_count, 1);
    EXPECT_EQ(spies[1]->undo_count, 1);
    EXPECT_EQ(spies[2]->undo_count, 1);
    EXPECT_EQ(spies[0]->execute_count, 2); // initial + redo
    EXPECT_EQ(spies[1]->execute_count, 2); // initial + redo
    EXPECT_EQ(spies[2]->execute_count, 1); // initial only (not redone yet)
}
