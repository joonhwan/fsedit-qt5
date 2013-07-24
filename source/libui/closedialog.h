#ifndef LIBUI_CLOSEDIALOG_H
#define LIBUI_CLOSEDIALOG_H

#include <QDialog>

namespace Ui { class CloseDialog; }

class QAbstractButton;

namespace LibUI {
	class CloseDialog : public QDialog {
		Q_OBJECT

	public:
		enum Choice { Close, Save, Cancel };

		explicit CloseDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);
		virtual ~CloseDialog();

		static Choice choose(QWidget* parent = 0);

	private slots:
		void on_buttonBox_clicked(QAbstractButton* button);

	private:
		Choice choice;
		Ui::CloseDialog* ui;
	};
}

#endif // LIBUI_CLOSEDIALOG_H
