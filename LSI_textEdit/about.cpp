#include "about.h"
#include <QtWidgets>


about::about(QWidget *wgt):QDialog(wgt,Qt::WindowTitleHint)
{
    QFont LSI("Times", 11, QFont::Bold);
    lbl = new QLabel("LSI_TextEdit\nBased on Qt 5.6.1\nAugust, 2016");
    lbl->setFont(LSI);

    QPushButton* cmdOk = new QPushButton("&Ok");
    connect(cmdOk, SIGNAL(clicked()), SLOT(accept()));

    QVBoxLayout* lay = new QVBoxLayout;
    lay->addWidget(lbl);
    lay->addWidget(cmdOk);
    this->setLayout(lay);
}

