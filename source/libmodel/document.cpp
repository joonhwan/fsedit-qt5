#include "document.h"

#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include <stdexcept>

struct LibModel::Document::DocumentPrivate {
	QStringList lines;
	QString filename;
};

LibModel::Document::Document(QObject* parent) : QObject(parent) {
	d = new DocumentPrivate;

	// Insert an empty string
	d->lines << QString();
}

LibModel::Document::~Document() {
	delete d;
	d = 0;
}

bool LibModel::Document::isEmpty() const {
	return lineCount() <= 1 && lineAt(0).isEmpty();
}

int LibModel::Document::lineCount() const {
	return d->lines.count();
}

QChar LibModel::Document::charAt(const QPoint& pos) const {
	const QString& line(lineAt(pos.y()));
	if (pos.x() < 0 || pos.y() >= line.length()) {
		throw std::out_of_range("Bad position");
	}

	return line.at(pos.x());
}

const QString& LibModel::Document::lineAt(int lineno) const {
	QString& line(const_cast<Document*>(this)->line(lineno));
	return line;
}

QChar LibModel::Document::deleteCharAt(const QPoint& pos) {
	const QChar c = charAt(pos);
	line(pos.y()).remove(pos.x(), 1);
	emit textChanged();
	return c;
}

QString LibModel::Document::deleteLineAt(int lineno) {
	const QString line = lineAt(lineno);
	d->lines.removeAt(lineno);

	if (lineCount() == 0) {
		// There must always be at least a single line
		d->lines << QString();
	}

	emit textChanged();
	return line;
}

void LibModel::Document::insertCharAt(const QPoint& pos, const QChar& c) {
	line(pos.y()).insert(pos.x(), c);
	emit textChanged();
}

void LibModel::Document::insertLineBreakAt(const QPoint& pos) {
	QString& l1(line(pos.y()));
	QString l2(l1.mid(pos.x()));
	l1.truncate(pos.x());
	d->lines.insert(pos.y() + 1, l2);
	emit textChanged();
}

void LibModel::Document::setLineText(int lineno, const QString& text) {
	line(lineno) = text;
	emit textChanged();
}

QPoint LibModel::Document::moveCursor(QPoint& pos, int leftRight, int upDown) const {
	// Move the cursor up or down
	pos.ry() += upDown;
	if (d->lines.isEmpty() || pos.y() < 0 || (pos.y() == 0 && pos.x() < 0)) {
		// Before the beginning
		pos = QPoint(0, 0);
		return pos;
	}
	if (pos.y() >= lineCount() || (pos.y() == lineCount() - 1 && pos.x() > d->lines.back().length())) {
		// After the end
		pos = QPoint(d->lines.back().length(), lineCount() - 1);
		return pos;
	}

	pos.setX(qBound(0, pos.x(), lineAt(pos.y()).length()));

	// Move the cursor left or right
	const int x = pos.x() + leftRight;
	const int maxX = lineAt(pos.y()).length();
	if (x < 0) {
		if (pos.y() == 0) {
			// Before the beginning
			pos.setX(0);
		} else {
			// Move the cursor up at the end of the last line and try again
			pos.setX(lineAt(pos.y() - 1).length());
			moveCursor(pos, x + 1, -1);
		}
	} else if (x > maxX) {
		// Move the cursor up at the beginning of the next line and try again
		pos.setX(0);
		moveCursor(pos, x - maxX - 1, 1);
	} else {
		pos.setX(x);
	}
	return pos;
}

void LibModel::Document::setFilename(const QString& filename) {
	if (filename != d->filename) {
		d->filename = filename;
		emit filenameChanged(filename);
	}
}

QString LibModel::Document::filename() const {
	return d->filename;
}

bool LibModel::Document::load(const QString& filename) {
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

	d->lines.clear();
	QTextStream in(&file);
	while (!in.atEnd()) {
		d->lines << in.readLine();
	}

	if (lineCount() == 0) {
		// There must always be at least a single line
		d->lines << QString();
	}

	setFilename(filename);

	emit fileClean();

	return true;
}

bool LibModel::Document::save(const QString& f) {
	const QString filename = f.isEmpty() ? d->filename : f;

	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return false;

	QTextStream out(&file);
	out.setCodec("UTF-8");
	bool first = true;
	Q_FOREACH(const QString& line, d->lines) {
		if (first) {
			first = false;
		} else {
#ifdef Q_WS_WIN
			out << "\r\n";
#elif defined Q_WS_MAC
			out << '\r';
#else
			out << '\n';
#endif
		}
		out << line;
	}
	file.close();

	if (filename != d->filename) {
		d->filename = filename;
		emit filenameChanged(filename);
	}

	emit fileClean();

	return true;
}

QString& LibModel::Document::line(int lineno) {
	if (lineCount() == 0) {
		// There must always be at least a single line
		d->lines << QString();
	}

	if (lineno < 0 || lineno >= d->lines.count()) {
		throw std::out_of_range("Bad position");
	}

	return d->lines[lineno];
}
