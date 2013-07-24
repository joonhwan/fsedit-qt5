#include "editwidget.h"

#include <libmodel/fsconnect.h>
#include <libmodel/document.h>
#include <libmodelcommands/editcommands.h>

#include <QFont>
#include <QPainter>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QUndoStack>

struct LibUI::EditWidget::EditWidgetPrivate {
	EditWidgetPrivate() : doc(0) {}

	LibModel::Document* doc;
	QPoint cursor;
	QUndoStack undoStack;
};

LibUI::EditWidget::EditWidget(LibModel::Document* doc, QWidget* parent) : QWidget(parent) {
	d = new EditWidgetPrivate;

	// Load a monospaced font
	QFont editFont("Monospace", font().pointSize());
	editFont.setStyleHint(QFont::TypeWriter);
	setFont(editFont);

	setFocusPolicy(Qt::StrongFocus);

	d->doc = doc;
	fsConnect(d->doc, SIGNAL(textChanged()), this, SLOT(update()));
	fsConnect(d->doc, SIGNAL(filenameChanged(const QString&)), this, SIGNAL(filenameChanged(const QString&)));

	// Handle the modified flag
	fsConnect(d->doc, SIGNAL(fileClean()), &d->undoStack, SLOT(setClean()));
}

LibUI::EditWidget::~EditWidget() {
	delete d;
	d = 0;
}

void LibUI::EditWidget::setCursorPosition(const QPoint& pos) {
	d->cursor = pos;
	update();
}

LibModel::Document* LibUI::EditWidget::document() {
	return d->doc;
}

QSize LibUI::EditWidget::sizeHint() const {
	return QSize(300, 400);
}

QUndoStack* LibUI::EditWidget::undoStack() {
	return &d->undoStack;
}

void LibUI::EditWidget::keyPressEvent(QKeyEvent* event) {
	switch (event->key()) {
		case Qt::Key_Left:
			d->doc->moveCursor(d->cursor, -1, 0);
			update();
			break;
		case Qt::Key_Right:
			d->doc->moveCursor(d->cursor, 1, 0);
			update();
			break;
		case Qt::Key_Up:
			d->doc->moveCursor(d->cursor, 0, -1);
			update();
			break;
		case Qt::Key_Down:
			d->doc->moveCursor(d->cursor, 0, 1);
			update();
			break;
		case Qt::Key_Delete:
			if (d->cursor.y() < d->doc->lineCount() || d->cursor.x() < d->doc->lineAt(d->cursor.y()).length()) {
				d->undoStack.push(new LibModelCommands::DelCharCommand(d->cursor, false, this));
			}
			break;
		case Qt::Key_Enter:
		case Qt::Key_Return:
			d->undoStack.push(new LibModelCommands::LineBreakCommand(d->cursor, this));
			break;
		case Qt::Key_Backspace:
			if (d->cursor != QPoint(0, 0)) {
				d->undoStack.push(new LibModelCommands::DelCharCommand(d->cursor, true, this));
			}
			break;
		default:
			if (!event->text().isEmpty()) {
				d->undoStack.push(new LibModelCommands::InsertCharCommand(event->text()[0], d->cursor, this));
			} else {
				QWidget::keyPressEvent(event);
			}
	}
}

// This is not optimized in any way. This code is not an attempt
// to write a perfect editor, so there will be no optimizations
// done here.
// Generating resize events inside the paint event is a performance
// problem. And even worse, this will always paint the entire text.
void LibUI::EditWidget::paintEvent(QPaintEvent* event) {
	if (d->doc == 0) {
		return;
	}

	QPainter painter(this);
	painter.fillRect(rect(), Qt::white);
	painter.setPen(Qt::black);

	const QFontMetrics fm(font());
	const int margin = 2;
	int y = margin;
	int maxX = 0;
	for (int lineno = 0; lineno < d->doc->lineCount(); ++lineno) {
		QString line(d->doc->lineAt(lineno));
		const QString cursorLine = line + ' ';

		if (d->cursor.y() == lineno) {
			// Draw the cursor
			const int left = fm.width(cursorLine, d->cursor.x()) + margin;
			const int width = fm.width(cursorLine[d->cursor.x()]);
			if (hasFocus()) {
				painter.fillRect(left, y, width, fm.height(), Qt::red);
			} else {
				painter.setPen(Qt::red);
				painter.drawRect(left, y, width, fm.height());
				painter.setPen(Qt::black);
			}
		}

		maxX = qMax(maxX, fm.width(cursorLine));
		painter.drawText(margin, y + fm.ascent(), line);
		y += fm.lineSpacing();
	}

	const QSize newWidgetSize(maxX, y + margin);
	if (newWidgetSize != size()) {
		setMinimumSize(newWidgetSize);
	}
}
