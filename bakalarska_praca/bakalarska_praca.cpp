#include "bakalarska_praca.h"

bakalarska_praca::bakalarska_praca(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

void bakalarska_praca::on_closeBtn_clicked() {
    close();
}

bakalarska_praca::~bakalarska_praca()
{}
