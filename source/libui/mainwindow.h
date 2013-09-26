#ifndef LIBUI_MAINWINDOW_H
#define LIBUI_MAINWINDOW_H

#include <QMainWindow>

class QUndoCommand;
class QScrollArea;

namespace LibModel { class Document; }

namespace LibUI {

class EditWidget;
class EditorArea : public QWidget
{
	Q_OBJECT
public:
	EditorArea(LibModel::Document* doc, QWidget* parent = 0);
	QScrollArea* scrollArea() {
		return m_scrollArea;
	}
	EditWidget* editor() {
		return m_editor;
	}
protected:
	QScrollArea* m_scrollArea;
	EditWidget* m_editor;
};

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent=0, Qt::WindowFlags f=0);
	virtual ~MainWindow();

Q_SIGNALS:
	void showScriptWindow();

protected:
	void closeEvent(QCloseEvent*);
	void changeEvent(QEvent*);

private Q_SLOTS:
	void filenameChanged(const QString&);

	void documentAdded(LibModel::Document*);
	void documentClosed(LibModel::Document*);

	void on_tabWidget_currentChanged(int);
	void on_fileNew_triggered();
	void on_fileOpen_triggered();
	void on_fileSave_triggered();
	void on_fileSaveAs_triggered();
	void on_fileClose_triggered();
	void on_fileQuit_triggered();
	void on_helpAboutDialog_triggered();

	void stackCleanChanged(bool clean);

private:
	/// If doc is 0, all documents are considered
	bool testIfClose(LibModel::Document* doc = 0);

	/**
	 * This will save the current file. If it doesn't have a filename, it will call saveAs.
	 * Returns true, if the file was saved.
	 */
	bool save(LibModel::Document* doc);

	/**
	 * This opens a file dialog for the filename and saves the file.
	 * Returns true, if the file was saved.
	 */
	bool saveAs(LibModel::Document* doc);

	void languageChange();

	struct MainWindowPrivate;
	MainWindowPrivate* d;
};

}

#endif // LIBUI_MAINWINDOW_H
