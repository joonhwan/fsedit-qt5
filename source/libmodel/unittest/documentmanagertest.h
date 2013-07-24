#ifndef DOCUMENTMANAGERTEST_H
#define DOCUMENTMANAGERTEST_H

#include <libmodel/document.h>

#include <QtCore/QObject>

class DocumentManagerTest : public QObject {
	Q_OBJECT

private Q_SLOTS:
	void testNewDocument();
};

// Helper class that connects to signals from DocumentManager
class DocumentReceiver : public QObject {
	Q_OBJECT

public:
	DocumentReceiver();

	LibModel::Document* getDocument();

	// Set the current document to 0
	void reset();

public Q_SLOTS:
	void receive(LibModel::Document* doc);

private:
	LibModel::Document* mDocument;
};

#endif // DOCUMENTMANAGERTEST_H
