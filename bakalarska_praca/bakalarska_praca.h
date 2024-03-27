#pragma once

#include <QtWidgets/QWidget>
#include "ui_bakalarska_praca.h"

class bakalarska_praca : public QWidget
{
    Q_OBJECT

public:
    bakalarska_praca(QWidget *parent = nullptr);
    ~bakalarska_praca();

private slots:
    void on_closeBtn_clicked();

private:
    Ui::bakalarska_pracaClass ui;
};
