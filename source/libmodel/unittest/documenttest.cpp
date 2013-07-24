#include "documenttest.h"

#include <libmodel/document.h>
#include <libfstest/fstest.h>

#include <stdexcept>

void DocumentTest::testIsEmpty() {
	try {
		LibModel::Document doc;
		QVERIFY(doc.isEmpty());

		doc.insertCharAt(QPoint(0, 0), 'q');
		QVERIFY(!doc.isEmpty());
	} catch(...) {
		QFAIL("Unexpected exception during test");
	}
}

void DocumentTest::testLineCount() {
	try {
		LibModel::Document doc;
		QCOMPARE(doc.lineCount(), 1);

		doc.insertCharAt(QPoint(0, 0), 'q');
		QCOMPARE(doc.lineCount(), 1);

		doc.insertLineBreakAt(QPoint(1, 0));
		QCOMPARE(doc.lineCount(), 2);

		doc.insertLineBreakAt(QPoint(0, 1));
		QCOMPARE(doc.lineCount(), 3);

		QCOMPARE(doc.lineAt(0), QString("q"));
		QCOMPARE(doc.lineAt(1), QString(""));
		QCOMPARE(doc.lineAt(2), QString(""));
	} catch(...) {
		QFAIL("Unexpected exception during test");
	}
}

void DocumentTest::testCharAt() {
	try {
		LibModel::Document doc;
		FS_ASSERT_THROW(doc.charAt(QPoint(0, 0)), std::out_of_range);
	} catch(...) {
		QFAIL("Unexpected exception during test");
	}
}
