#ifndef FLIECENTDLG_H
#define FLIECENTDLG_H

#include <QDialog>

namespace Ui {
class FileCentDlg;
}

class FileCentDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FileCentDlg(QWidget *parent = nullptr);
    ~FileCentDlg();

private:
    Ui::FileCentDlg *ui;
};

#endif // FLIECENTDLG_H
