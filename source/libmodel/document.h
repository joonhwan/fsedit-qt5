#ifndef LIBMODEL_DOCUMENT_H
#define LIBMODEL_DOCUMENT_H

#include <QtCore/QPoint>
#include <QtCore/QObject>

namespace LibModel {
	class Document : public QObject {
		Q_OBJECT

	public:
		explicit Document(QObject* parent = 0);
		virtual ~Document();

		bool isEmpty() const;

		int lineCount() const;

		QChar charAt(const QPoint& pos) const;
		const QString& lineAt(int lineno) const;

		QChar deleteCharAt(const QPoint& pos);
		QString deleteLineAt(int lineno);

		void insertCharAt(const QPoint& pos, const QChar& c);

		void insertLineBreakAt(const QPoint& pos);

		void setLineText(int lineno, const QString& text);

		/**
		 * This calculates a cursor move from a given position. The code first
		 * considers the upDown (moving between lines), where negative numbers
		 * are up. Then leftRight, where negative numbers are left.
		 */
		QPoint moveCursor(QPoint& pos, int leftRight, int upDown) const;

		void setFilename(const QString& filename);
		QString filename() const;

		/**
		 * Discard whatever we previously had and load the file contents.
		 * Returns true, if the file was loaded.
		 */
		bool load(const QString& filename);

		/**
		 * Attempt to save the file. If the argument filename is empty, it will use the last
		 * set filename. This will fail (and return false) if the filename is invalid or the user doesn't
		 * have save access on the file or directory.
		 */
		bool save(const QString& filename = QString());

	Q_SIGNALS:
		void textChanged();
		void fileClean();
		void filenameChanged(const QString& filename);

	private:
		QString& line(int lineno);

		struct DocumentPrivate;
		DocumentPrivate* d;
	};
}

#endif // LIBMODEL_DOCUMENT_H
