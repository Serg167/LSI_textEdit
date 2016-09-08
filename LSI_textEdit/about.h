#pragma once

#include <QtWidgets>

class about: public QDialog
{
    Q_OBJECT
    QLabel *lbl;
public:
    about(QWidget* wgt = 0);
};
