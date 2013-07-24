#include <libui/mainwindow.h>
#include <libui/translation.h>

#include <QApplication>

int main(int argc, char** argv) {
	QApplication app(argc, argv);

	QCoreApplication::setOrganizationName("Fionia Software");
	QCoreApplication::setOrganizationDomain("www.fioniasoftware.dk");
	QCoreApplication::setApplicationName("FS Editor");

	// Initialize the icon resource
	Q_INIT_RESOURCE(libui);

	LibUI::Translation::instance().installDefaultTranslation();

	LibUI::MainWindow* window = new LibUI::MainWindow;
	window->show();

	return app.exec();
}
