#pragma once

#include <QtWidgets/QWidget>
#include "ui_bakalarska_praca.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/qfiledialog.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qprogressbar.h>
#include <qtimer.h>
#include <QtConcurrent/qtconcurrentrun.h>

class bakalarska_praca : public QWidget
{
    Q_OBJECT

public:
    bakalarska_praca(QWidget *parent = nullptr);
    ~bakalarska_praca();

private slots:
    void choosePath();
    void runProgram();
    void updateProgressBar();

private:
    QFuture<void> future;
    QLineEdit* pathLineEdit1;
    QLineEdit* pathLineEdit2;
    QPushButton* pathButton1;
    QPushButton* pathButton2;
    QPushButton* runButton;
    QProgressBar* progressBar;
};
