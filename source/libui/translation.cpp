#include "translation.h"

#include <libmodel/fsconnect.h>

#include <QLocale>
#include <QTranslator>
#include <QDir>
#include <QSignalMapper>
#include <QDebug>
#include <QScopedPointer>
#include <QCoreApplication>
#include <QAction>

static QDir findTranslationsPath() {
	QDir appPath(QCoreApplication::applicationDirPath());

	if (appPath.cd("../translations")) {
		// This is installed mode
		return appPath;
	}

	if (appPath.cd("../../translations")) {
		// This is installed mode
		return appPath;
	}

	if (appPath.cd("translations")) {
		// Someone moved the binary
		return appPath;
	}

	// We give up
	return QDir("hopefullyThisDoesNotExistOrWeWillReadTranslationsFromIt");
}

LibUI::Translation& LibUI::Translation::instance() {
	static Translation* t = 0;
	if (t == 0) {
		t = new Translation;
	}
	return *t;
}

void addLanguageAction(const QString& languageCode, QSignalMapper* mapper, QObject* parent) {
	// Create a new action for this translation
	QAction* action = new QAction(parent);
	action->setObjectName(languageCode);

	// Attach the action to the signal mapper
	mapper->setMapping(action, languageCode);
	fsConnect(action, SIGNAL(triggered(bool)), mapper, SLOT(map()));
}

LibUI::Translation::Translation() {
	const QDir translationDir(findTranslationsPath());
	const QStringList translationFiles(translationDir.entryList(QStringList() << "fseditor_*.qm"));

	// This signal mapper is used to signal a language change
	QSignalMapper* mapper = new QSignalMapper(this);
	fsConnect(mapper, SIGNAL(mapped(const QString&)), this, SLOT(installTranslator(const QString&)));

	// Create the one for english
	addLanguageAction("ko", mapper, this); //addLanguageAction("en", mapper, this);

	Q_FOREACH(const QString& filename, translationFiles) {
		// This code translations "fseditor_xy.qm" to "xy"
		QString languageCode = filename.mid(9);
		languageCode.truncate(languageCode.length() - 3);
		qDebug() << "Adding language code:" << languageCode;
		addLanguageAction(languageCode, mapper, this);
	}

	languageChange();
}

LibUI::Translation::~Translation() {
}

void LibUI::Translation::installDefaultTranslation() {
	// Load the default system language
	const QString language = QLocale::system().name();
	installTranslator(language);
}

QList<QAction*> LibUI::Translation::translationActions() {
	return findChildren<QAction*>();
}

void LibUI::Translation::languageChange() {
	const QList<QAction*> actions = translationActions();

	Q_FOREACH(QAction* action, actions) {
		if (action->objectName() == "en") {
			action->setText(tr("&English"));
		} else if (action->objectName() == "da") {
			action->setText(tr("&Danish"));
		} else if (action->objectName() == "ko") {
			action->setText(tr("&Korean"));
		} else {
			qWarning("Untranslated language code");
			action->setText(action->objectName());
		}
	}
}

void LibUI::Translation::installTranslator(const QString& code) {
	QDir translationsPath(findTranslationsPath());

	const QList<QTranslator*> oldTranslators(findChildren<QTranslator*>());

	if (code == "ko") {
		// English means no translator installed
		qDeleteAll(oldTranslators);
	} else if (translationsPath.isReadable()) {
		// Try loading the new translation
		QScopedPointer<QTranslator> appTranslator(new QTranslator(this));
		if (appTranslator->load("fseditor_" + code, translationsPath.absolutePath()))
		{
			QScopedPointer<QTranslator> qtTranslator;
			if(code!="en") {
				qtTranslator.reset(new QTranslator(this));
				qtTranslator->load("qt_" + code, translationsPath.absolutePath());
			};
			// Delete the old translators
			qDeleteAll(oldTranslators);
			// Install the new ones
			QCoreApplication::instance()->installTranslator(appTranslator.take());
			if(qtTranslator) {
				QCoreApplication::instance()->installTranslator(qtTranslator.take());
			}
		}
	}
}
