#include "documentmanagertest.h"

#include <libmodel/documentmanager.h>
#include <libmodel/fsconnect.h>

#include <libfstest/fstest.h>

void DocumentManagerTest::testNewDocument() {
	DocumentReceiver receiver;
	fsConnect(&LibModel::DocumentManager::instance(), SIGNAL(documentAdded(LibModel::Document*)),
		&receiver, SLOT(receive(LibModel::Document*)));

	QCOMPARE(LibModel::DocumentManager::instance().documentCount(), 0);

	LibModel::Document* doc = 0;
	QCOMPARE(receiver.getDocument(), doc);

	doc = LibModel::DocumentManager::instance().newDocument();
	QCOMPARE(LibModel::DocumentManager::instance().documentCount(), 1);
	QCOMPARE(receiver.getDocument(), doc);

	LibModel::Document* doc2 = 0;
	receiver.reset();
	QCOMPARE(receiver.getDocument(), doc2);

	doc2 = LibModel::DocumentManager::instance().newDocument();
	QVERIFY(doc != doc2);
	QCOMPARE(LibModel::DocumentManager::instance().documentCount(), 2);
	QCOMPARE(receiver.getDocument(), doc2);
}

DocumentReceiver::DocumentReceiver() : mDocument(0) {
}

LibModel::Document* DocumentReceiver::getDocument() {
	return mDocument;
}

void DocumentReceiver::reset() {
	mDocument = 0;
}

void DocumentReceiver::receive(LibModel::Document* doc) {
	mDocument = doc;
}
