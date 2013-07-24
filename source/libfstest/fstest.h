#ifndef FSTEST_FSTEST_H
#define FSTEST_FSTEST_H

#include <QTest>

namespace FSTest {
	class TestFactoryBase {
	public:
		virtual ~TestFactoryBase() {}

		virtual QObject* createTest() = 0;
	};

	void addTest(TestFactoryBase* test, const char* testName);

	template <typename T> class TestFactory : public TestFactoryBase {
	public:
		TestFactory(const char* testName) {
			FSTest::addTest(this, testName);
		}

		virtual QObject* createTest() { return new T; }
	};
}

// This one is missing from the Qt test lib
#define FS_ASSERT_THROW(code, exc) \
	try { \
		code; \
		QFAIL("Exception #exc not thrown"); \
	} catch (exc&) { \
		/* All is well */ \
	}

#endif // FSTEST_FSTEST_H
