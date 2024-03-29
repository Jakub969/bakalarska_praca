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
#include <qevent.h>
#include "FileManager.h"
#include "BPMDetector.h"
#include "SMMaker.h"

class bakalarska_praca : public QWidget
{
    Q_OBJECT

public:
    bakalarska_praca(QWidget *parent = nullptr);
    ~bakalarska_praca();

protected:
    void closeEvent(QCloseEvent* event) override {
        closeFunction();
        event->accept();
    }

private slots:
    void choosePath();
    void runProgram();
    void updateProgressBar();
    void closeFunction();

private:
    QFuture<void> future;
    QLineEdit* pathLineEdit1;
    QLineEdit* pathLineEdit2;
    QPushButton* pathButton1;
    QPushButton* pathButton2;
    QPushButton* runButton;
    QProgressBar* progressBar;
    BPMDetector detector;
};
