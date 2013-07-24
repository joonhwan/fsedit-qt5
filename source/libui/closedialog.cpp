#include "closedialog.h"
#include "ui_closedialog.h"

LibUI::CloseDialog::CloseDialog(QWidget* parent, Qt::WindowFlags f) : QDialog(parent, f), choice(Cancel) {
	ui = new Ui::CloseDialog;
	ui->setupUi(this);
}

LibUI::CloseDialog::~CloseDialog() {
	delete ui;
	ui = 0;
}

LibUI::CloseDialog::Choice LibUI::CloseDialog::choose(QWidget* parent) {
	CloseDialog dialog(parent);
	dialog.exec();
	return dialog.choice;
}

void LibUI::CloseDialog::on_buttonBox_clicked(QAbstractButton* button) {
	switch (ui->buttonBox->standardButton(button)) {
		case QDialogButtonBox::Ok:
			choice = Close;
			break;
		case QDialogButtonBox::Save:
			choice = Save;
			break;
		default:
			choice = Cancel;
			break;
	}

	// No matter which one, we should close the dialog
	close();
}
