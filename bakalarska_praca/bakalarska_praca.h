#pragma once

#include <QtWidgets/QWidget>
#include "ui_bakalarska_praca.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/qfiledialog.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qboxlayout.h>

class bakalarska_praca : public QWidget
{
    Q_OBJECT

public:
    bakalarska_praca(QWidget *parent = nullptr);
    ~bakalarska_praca();

private slots:
    void choosePath();

private:
    //Ui::bakalarska_pracaClass ui;
    QLineEdit* pathLineEdit;
};
