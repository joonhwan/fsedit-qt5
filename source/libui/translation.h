#ifndef TRANSLATION_H
#define TRANSLATION_H

#include <QtCore/QObject>
#include <QtCore/QList>

class QAction;

namespace LibUI {
	class Translation : public QObject {
		Q_OBJECT

	public:
		static Translation& instance();

		void installDefaultTranslation();

		QList<QAction*> translationActions();

		void languageChange();

	private Q_SLOTS:
		void installTranslator(const QString& code);

	private:
		Translation();
		~Translation();
	};
}

#endif // TRANSLATION_H
