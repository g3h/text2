#include "fliecentdlg.h"
#include "ui_fliecentdlg.h"

fliecentdlg::fliecentdlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fliecentdlg)
{
    ui->setupUi(this);
}

fliecentdlg::~fliecentdlg()
{
    delete ui;
}
