#include "fstest.h"

#include <QDir>
#include <QHash>
#include <QDebug>
#include <QApplication>

static QHash<QString, FSTest::TestFactoryBase*>& getTests() {
	static QHash<QString, FSTest::TestFactoryBase*> tests;
	return tests;
}

void FSTest::addTest(TestFactoryBase* test, const char* testName) {
	getTests().insert(QString(testName), test);
}

int main(int argc, char** argv) {
	QApplication app(argc, argv);

	// Default locale for the test is US English
	QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));

	bool runAllTests = true;
	Q_FOREACH(const QString& arg, app.arguments()) {
		if (arg.startsWith("-dir=")) {
			// Run in this directory
			QDir::setCurrent(arg.mid(5));
		} else if (arg.startsWith("-test=")) {
			// Run this test
			QString testName(arg.mid(6));

			if (!getTests().contains(testName)) {
				qDebug() << "Test" << testName << "not registered";
				return -1;
			}

			QObject* test = getTests().value(testName)->createTest();
			const int ret = QTest::qExec(test);
			delete test;
			if (ret != 0)
				return ret;

			// We have run a test manually, so don't run them all
			runAllTests = false;
		}
	}

	if (runAllTests) {
		QHash<QString, FSTest::TestFactoryBase*>& tests(getTests());
		for (QHash<QString, FSTest::TestFactoryBase*>::const_iterator i = tests.constBegin(); i != tests.constEnd(); ++i) {
			QObject* test = i.value()->createTest();
			const int ret = QTest::qExec(test);
			delete test;
			if (ret != 0) {
				return ret;
			}
		}
	}

	return 0;
}
