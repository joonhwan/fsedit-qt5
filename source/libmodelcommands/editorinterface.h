#ifndef LIBMODELCOMMANDS_EDITORINTERFACE_H
#define LIBMODELCOMMANDS_EDITORINTERFACE_H

class QPoint;

namespace LibModel { class Document; }

namespace LibModelCommands {
	class EditorInterface {
	public:
		virtual LibModel::Document* document() = 0;

		virtual void setCursorPosition(const QPoint& pos) = 0;

	protected:
		virtual ~EditorInterface() {}
	};
}

#endif // LIBMODELCOMMANDS_EDITORINTERFACE_H
