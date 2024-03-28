#include "bakalarska_praca.h"

bakalarska_praca::bakalarska_praca(QWidget *parent)
    : QWidget(parent)
{
    //ui.setupUi(this);
    setFixedSize(750, 500);

    pathLineEdit = new QLineEdit;
    pathLineEdit->setReadOnly(true);

    pathLineEdit->setText("Zadajte mp3 subor alebo priecinok s pesnickami");

    pathLineEdit->setFixedWidth(500);
    pathLineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QPushButton* pathButton = new QPushButton("...");
    connect(pathButton, &QPushButton::clicked, this, &bakalarska_praca::choosePath);

    pathButton->setFixedWidth(50);
    pathButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(pathLineEdit);
    layout->addWidget(pathButton);

    setLayout(layout);
}

void bakalarska_praca::choosePath() {
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose Directory"),
        "/home",
        QFileDialog::ShowDirsOnly
        | QFileDialog::DontResolveSymlinks);

    pathLineEdit->setText(dir);
}

bakalarska_praca::~bakalarska_praca()
{}
