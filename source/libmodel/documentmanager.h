#ifndef LIBMODEL_DOCUMENTMANAGER_H
#define LIBMODEL_DOCUMENTMANAGER_H

#include <QtCore/QObject>

namespace LibUI { class EditWidget; }

namespace LibModel {
	class Document;

	class DocumentManager : public QObject {
		Q_OBJECT

	public:
		static DocumentManager& instance();

		Document* newDocument();

		/**
		 * Load a file, or create a new one with this filename.
		 * If the file is already loaded, return the same copy.
		 */
		Document* loadDocument(const QString& filename);

		/// Returns 0, if the document isn't loaded
		Document* findDocument(const QString& filename);

		/**
		 * Close a document even if there are still editors on it.
		 * The documentClosed signal is emitted and then the object is destroyed.
		 */
		void closeDocument(Document* doc);

		/// Get the number of open documents
		int documentCount() const;

	Q_SIGNALS:
		void documentAdded(LibModel::Document* doc);
		void documentClosed(LibModel::Document* doc);

	private:
		DocumentManager();
		~DocumentManager();

		struct DocumentManagerPrivate;
		DocumentManagerPrivate* d;
	};
}

#endif // LIBMODEL_DOCUMENTMANAGER_H
