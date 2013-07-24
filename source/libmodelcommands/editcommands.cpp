#include "editcommands.h"

#include <libmodel/document.h>

LibModelCommands::EditCommand::EditCommand(EditorInterface* editor) : mEditor(editor) {
}

LibModelCommands::EditorInterface* LibModelCommands::EditCommand::editor() {
	return mEditor;
}

LibModelCommands::InsertCharCommand::InsertCharCommand(const QChar& c, const QPoint& pos, EditorInterface* editor)
	: EditCommand(editor), mChar(c), mPos(pos)
{
}

void LibModelCommands::InsertCharCommand::undo() {
	editor()->document()->deleteCharAt(mPos);
	editor()->setCursorPosition(mPos);
}

void LibModelCommands::InsertCharCommand::redo() {
	editor()->document()->insertCharAt(mPos, mChar);
	editor()->setCursorPosition(QPoint(1, 0) + mPos);
}

LibModelCommands::DelCharCommand::DelCharCommand(const QPoint& pos, bool backspace, EditorInterface* editor)
	: EditCommand(editor), mPos(pos), mBackspace(backspace)
{
	if (backspace) {
		editor->document()->moveCursor(mPos, -1, 0);
	}
	mChar = editor->document()->charAt(mPos);
}

void LibModelCommands::DelCharCommand::undo() {
	editor()->document()->insertCharAt(mPos, mChar);

	QPoint pos(mPos);
	if (mBackspace) {
		editor()->document()->moveCursor(pos, 1, 0);
	}
	editor()->setCursorPosition(pos);
}

void LibModelCommands::DelCharCommand::redo() {
	editor()->document()->deleteCharAt(mPos);
	editor()->setCursorPosition(mPos);
}

LibModelCommands::LineBreakCommand::LineBreakCommand(const QPoint& pos, EditorInterface* editor)
	: EditCommand(editor), mLine(editor->document()->lineAt(pos.y())), mPos(pos)
{
}

void LibModelCommands::LineBreakCommand::undo() {
	editor()->document()->setLineText(mPos.y(), mLine);
	editor()->document()->deleteLineAt(mPos.y() + 1);
	editor()->setCursorPosition(mPos);
}

void LibModelCommands::LineBreakCommand::redo() {
	editor()->document()->insertLineBreakAt(mPos);

	QPoint pos(mPos);
	editor()->document()->moveCursor(pos, 1, 0);
	editor()->setCursorPosition(pos);
}
