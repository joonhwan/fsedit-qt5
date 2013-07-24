#include "mainwindow.h"
#include "editwidget.h"
#include "closedialog.h"
#include "translation.h"
#include "ui_mainwindow.h"
#include "ui_helpdialog.h"

#include <libmodel/fsconnect.h>
#include <libmodel/document.h>
#include <libmodel/documentmanager.h>

#include <QDir>
#include <QFileInfo>
#include <QSharedPointer>
#include <QCloseEvent>
#include <QMenuBar>
#include <QMenu>
#include <QApplication>
#include <QToolBar>
#include <QUndoStack>
#include <QUndoGroup>
#include <QFileDialog>
#include <QMessageBox>
#include <QScrollArea>

struct LibUI::MainWindow::MainWindowPrivate : public Ui::MainWindow {
	EditWidget* editorAt(int index) {
		QScrollArea* scrollArea = qobject_cast<QScrollArea*>(tabWidget->widget(index));
		return qobject_cast<EditWidget*>(scrollArea->widget());
	}

	EditWidget* currentEditor() {
		QScrollArea* scrollArea = qobject_cast<QScrollArea*>(tabWidget->currentWidget());
		return qobject_cast<EditWidget*>(scrollArea->widget());
	}

	EditWidget* documentEditor(LibModel::Document* doc) {
		for (int i=0; i<tabWidget->count(); ++i) {
			EditWidget* editor = editorAt(i);
			if (editor->document() == doc) {
				// Found it
				return editor;
			}
		}

		// Not found
		return 0;
	}

	QToolBar* fileTools;
	QToolBar* editTools;

	QUndoGroup undoGroup;
	QAction* undoAction;
	QAction* redoAction;
};

LibUI::MainWindow::MainWindow(QWidget* parent, Qt::WindowFlags f) : QMainWindow(parent, f) {
	d = new MainWindowPrivate;
	d->setupUi(this);

	// Set up the menus and toolbar
	d->fileTools = addToolBar("");
	d->editTools = addToolBar("");

	d->fileTools->addAction(d->fileNew);
	d->fileTools->addAction(d->fileOpen);
	d->fileTools->addAction(d->fileSave);

	d->undoAction = d->undoGroup.createUndoAction(this);
	d->undoAction->setIcon(QIcon(":/images/undo.png"));
	d->menu_Edit->addAction(d->undoAction);
	d->editTools->addAction(d->undoAction);
	d->redoAction = d->undoGroup.createRedoAction(this);
	d->redoAction->setIcon(QIcon(":/images/redo.png"));
	d->menu_Edit->addAction(d->redoAction);
	d->editTools->addAction(d->redoAction);

	QAction* action = d->fileTools->toggleViewAction();
	action->setShortcut(QKeySequence("F5"));
	d->menu_View->addAction(action);
	action = d->editTools->toggleViewAction();
	action->setShortcut(QKeySequence("F6"));
	d->menu_View->addAction(action);

	const QList<QAction*> trActions(Translation::instance().translationActions());
	Q_FOREACH(action, trActions) {
		d->menu_Translation->addAction(action);
	}

	fsConnect(d->helpAboutQt, SIGNAL(triggered()), QApplication::instance(), SLOT(aboutQt()));

	fsConnect(&LibModel::DocumentManager::instance(), SIGNAL(documentAdded(LibModel::Document*)), this, SLOT(documentAdded(LibModel::Document*)));
	fsConnect(&LibModel::DocumentManager::instance(), SIGNAL(documentClosed(LibModel::Document*)), this, SLOT(documentClosed(LibModel::Document*)));

	// Create the first document
	LibModel::Document* doc = LibModel::DocumentManager::instance().newDocument();

	// Set all strings
	languageChange();
}

LibUI::MainWindow::~MainWindow() {
	delete d;
	d = 0;
}

void LibUI::MainWindow::closeEvent(QCloseEvent* event) {
	if (testIfClose()) {
		event->accept();
	} else {
		event->ignore();
	}
}

void LibUI::MainWindow::changeEvent(QEvent* event) {
	if (event->type() == QEvent::LanguageChange) {
		languageChange();
	}
	QWidget::changeEvent(event);
}

void LibUI::MainWindow::filenameChanged(const QString& filename) {
	EditWidget* editor = qobject_cast<EditWidget*>(sender());
	for (int i=0; i<d->tabWidget->count(); ++i) {
		if (d->editorAt(i) == editor) {
			// Found it
			d->tabWidget->setTabToolTip(i, QDir::toNativeSeparators(filename));
			d->tabWidget->setTabText(i, QFileInfo(filename).fileName());
			break;
		}
	}
}

void LibUI::MainWindow::on_tabWidget_currentChanged(int currentIndex) {
	if (currentIndex >= 0) {
		EditWidget* editor = d->editorAt(currentIndex);
		d->undoGroup.setActiveStack(editor->undoStack());
		editor->setFocus(Qt::OtherFocusReason);

		if (editor->document()->filename().isEmpty()) {
			setWindowTitle(tr("Editor [*]"));
		} else {
			setWindowTitle(tr("Editor: %1 [*]").arg(editor->document()->filename()));
		}
	}
}

void LibUI::MainWindow::on_fileNew_triggered() {
	LibModel::Document* doc = LibModel::DocumentManager::instance().newDocument();
}

void LibUI::MainWindow::on_fileOpen_triggered() {
	const QString filename = QFileDialog::getOpenFileName(this);
	if (!filename.isEmpty()) {
		LibModel::Document* doc = d->currentEditor()->document();
		if (!doc->filename().isEmpty() || !doc->isEmpty()) {
			// The current document is in use, create a new one
			doc = LibModel::DocumentManager::instance().newDocument();
		}
		doc->load(filename);
	}
}

void LibUI::MainWindow::on_fileSave_triggered() {
	save(d->currentEditor()->document());
}

void LibUI::MainWindow::on_fileSaveAs_triggered() {
	saveAs(d->currentEditor()->document());
}

void LibUI::MainWindow::on_fileClose_triggered() {
	LibModel::Document* doc = d->currentEditor()->document();
	if (testIfClose(doc)) {
		// The documentClosed slot will handle things from here
		LibModel::DocumentManager::instance().closeDocument(doc);
	}
}

void LibUI::MainWindow::on_fileQuit_triggered() {
	close();
}

void LibUI::MainWindow::on_helpAboutDialog_triggered() {
	QDialog helpDialog;
	Ui::HelpDialog ui;
	ui.setupUi(&helpDialog);
	helpDialog.exec();
}

void LibUI::MainWindow::documentAdded(LibModel::Document* doc) {
	QScrollArea* scrollArea = new QScrollArea;
	EditWidget* editor = new EditWidget(doc);

	fsConnect(editor, SIGNAL(filenameChanged(const QString&)), this, SLOT(filenameChanged(const QString&)));

	fsConnect(editor->undoStack(), SIGNAL(cleanChanged(bool)), this, SLOT(stackCleanChanged(bool)));
	d->undoGroup.addStack(editor->undoStack());

	scrollArea->setWidget(editor);
	scrollArea->setWidgetResizable(true);
	scrollArea->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	d->tabWidget->addTab(scrollArea, tr("New file"));
	d->tabWidget->setCurrentWidget(scrollArea);
}

void LibUI::MainWindow::documentClosed(LibModel::Document* doc) {
	for (int i=0; i<d->tabWidget->count(); ++i) {
		EditWidget* editor = d->editorAt(i);
		if (editor->document() == doc) {
			// Found it
			d->tabWidget->removeTab(i);

			// If there are no more tabs, close the window
			if (d->tabWidget->count() == 0) {
				close();
			}

			break;
		}
	}
}

bool LibUI::MainWindow::testIfClose(LibModel::Document* doc) {
	QList<EditWidget*> editors;

	if (doc != 0) {
		// Only test the argument doc for modification
		EditWidget* editor = d->documentEditor(doc);
		if (editor != 0 && !editor->undoStack()->isClean()) {
			editors << editor;
		}
	} else if (isWindowModified()) {
		// Test all editors with modifications
		for (int i=0; i<d->tabWidget->count(); ++i) {
			EditWidget* editor = d->editorAt(i);
			if (!editor->undoStack()->isClean()) {
				editors << editor;
			}
		}
	}

	if (editors.isEmpty()) {
		// No modifications
		return true;
	}

	// Ask the user what to do
	CloseDialog::Choice choice = CloseDialog::choose(this);
	if (choice == CloseDialog::Cancel) {
		// Cancel closing
		return false;
	} else if (choice == CloseDialog::Close) {
		// Close without saving
		return true;
	}

	// Save the files
	bool ok = true;
	QList<EditWidget*>::const_iterator it;
	for (it = editors.begin(); ok && it != editors.end(); ++it) {
		LibModel::Document* doc = (*it)->document();
		ok &= save(doc);
	}

	return ok;
}

bool LibUI::MainWindow::save(LibModel::Document* doc) {
	if (doc->filename().isEmpty()) {
		return saveAs(doc);
	} else {
		const bool saved = doc->save();
		if (saved) {
			statusBar()->showMessage(tr("Document saved"), 5000);
		} else {
			QMessageBox::critical(this, tr("Error while saving"), tr("Failed to save the document"));
		}
		return saved;
	}
}

bool LibUI::MainWindow::saveAs(LibModel::Document* doc) {
	const QString filename = QFileDialog::getSaveFileName(this, QString(), doc->filename());
	if (filename.isEmpty()) {
		// Cancelled
		return false;
	}

	doc->setFilename(filename);
	return save(doc);
}

void LibUI::MainWindow::stackCleanChanged(bool clean) {
	if (clean == isWindowModified()) {
		// Figure out if it's really clean or not
		clean = true;
		for (int i=0; clean && i<d->tabWidget->count(); ++i) {
			EditWidget* editor = d->editorAt(i);
			clean &= editor->undoStack()->isClean();
		}

		setWindowModified(!clean);
	}
}

void LibUI::MainWindow::languageChange() {
	// Update the names of the toolbars
	d->fileTools->setWindowTitle(tr("File Toolbar"));
	d->editTools->setWindowTitle(tr("Edit Toolbar"));

	// Update the undo/redo action text
	// AFAIK, this hack is the only way to do it properly
	QSharedPointer<QAction> action(d->undoGroup.createUndoAction(0));
	d->undoAction->setText(action->text());
	action = QSharedPointer<QAction>(d->undoGroup.createRedoAction(0));
	d->redoAction->setText(action->text());

	// Update the designer parts
	d->retranslateUi(this);

	// Set the action shortcuts
	d->fileNew->setShortcut(QKeySequence::New);
	d->fileOpen->setShortcut(QKeySequence::Open);
	d->fileSave->setShortcut(QKeySequence::Save);
	d->fileSaveAs->setShortcut(QKeySequence::SaveAs);
	d->fileClose->setShortcut(QKeySequence::Close);
	d->fileQuit->setShortcut(QKeySequence::Quit);
	d->undoAction->setShortcut(QKeySequence::Undo);
	d->redoAction->setShortcut(QKeySequence::Redo);

	// Update the no-name tabs
	for (int i=0; i<d->tabWidget->count(); ++i) {
		EditWidget* editor = d->editorAt(i);
		if (editor->document()->filename().isEmpty()) {
			d->tabWidget->setTabText(i, tr("New file"));
		}
	}

	// Set the strings on the language actions
	Translation::instance().languageChange();

	// Update the title on the window
	on_tabWidget_currentChanged(d->tabWidget->currentIndex());
}
