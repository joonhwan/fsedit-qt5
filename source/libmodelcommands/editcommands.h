#ifndef LIBMODELCOMMANDS_EDITCOMMANDS_H
#define LIBMODELCOMMANDS_EDITCOMMANDS_H

#include "editorinterface.h"

#include <QUndoCommand>
#include <QPoint>

namespace LibModelCommands {
	class EditCommand : public QUndoCommand {
	public:
		explicit EditCommand(EditorInterface* editor);

		EditorInterface* editor();

	private:
		EditorInterface* mEditor;
	};

	class InsertCharCommand : public EditCommand {
	public:
		InsertCharCommand(const QChar& c, const QPoint& pos, EditorInterface* editor);

		virtual void undo();
		virtual void redo();

	private:
		const QChar mChar;
		const QPoint mPos;
	};

	class DelCharCommand : public EditCommand {
	public:
		DelCharCommand(const QPoint& pos, bool backspace, EditorInterface* editor);

		virtual void undo();
		virtual void redo();

	private:
		QChar mChar;
		QPoint mPos;
		const bool mBackspace;
	};

	class LineBreakCommand : public EditCommand {
	public:
		LineBreakCommand(const QPoint& pos, EditorInterface* editor);

		virtual void undo();
		virtual void redo();

	private:
		const QString mLine;
		const QPoint mPos;
	};
}

#endif // LIBMODELCOMMANDS_EDITCOMMANDS_H
