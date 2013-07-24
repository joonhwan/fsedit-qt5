#ifndef TRANSACTIONTEST_H
#define TRANSACTIONTEST_H

#include <QObject>

class DocumentTest: public QObject {
	Q_OBJECT

private Q_SLOTS:
	void testIsEmpty();
	void testLineCount();
	void testCharAt();
};

#endif // TRANSACTIONTEST_H
