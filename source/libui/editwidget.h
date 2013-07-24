#ifndef LIBUI_EDITWIDGET_H
#define LIBUI_EDITWIDGET_H

#include <libmodelcommands/editorinterface.h>

#include <QWidget>

class QUndoStack;

namespace LibUI {
	class EditWidget : public QWidget, private LibModelCommands::EditorInterface {
		Q_OBJECT

	public:
		explicit EditWidget(LibModel::Document* doc, QWidget* parent = 0);
		virtual ~EditWidget();

		virtual void setCursorPosition(const QPoint& pos);

		LibModel::Document* document();

		virtual QSize sizeHint() const;

		QUndoStack* undoStack();

	Q_SIGNALS:
		void filenameChanged(const QString&);

	protected:
		void keyPressEvent(QKeyEvent* event);
		void paintEvent(QPaintEvent* event);

	private:
		struct EditWidgetPrivate;
		EditWidgetPrivate* d;
	};
}

#endif // LIBUI_EDITWIDGET_H
