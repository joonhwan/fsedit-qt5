#include "documentmanager.h"
#include "document.h"

#include <QtCore/QList>

struct LibModel::DocumentManager::DocumentManagerPrivate {
	QList<LibModel::Document*> docs;
};

LibModel::DocumentManager& LibModel::DocumentManager::instance() {
	static DocumentManager* manager = 0;
	if (manager == 0)
		manager = new DocumentManager;
	return *manager;
}

LibModel::DocumentManager::DocumentManager() {
	d = new DocumentManagerPrivate;
}

LibModel::DocumentManager::~DocumentManager() {
	// All documents have this as parent, so they are deleted automatically
	delete d;
	d = 0;
}

LibModel::Document* LibModel::DocumentManager::newDocument() {
	Document* doc = new Document(this);
	d->docs << doc;
	emit documentAdded(doc);
	return doc;
}

LibModel::Document* LibModel::DocumentManager::loadDocument(const QString& filename) {
	Q_FOREACH(LibModel::Document* doc, d->docs) {
		if (doc->filename() == filename) {
			// Found it
			return doc;
		}
	}

	// Not found
	Document* doc = new Document(this);
	doc->load(filename);
	d->docs << doc;
	emit documentAdded(doc);
	return doc;
}

void LibModel::DocumentManager::closeDocument(Document* doc) {
	d->docs.removeAll(doc);
	emit documentClosed(doc);
	delete doc;
}

int LibModel::DocumentManager::documentCount() const {
	return d->docs.count();
}
