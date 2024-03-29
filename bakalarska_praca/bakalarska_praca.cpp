#include "bakalarska_praca.h"

bakalarska_praca::bakalarska_praca(QWidget *parent)
    : QWidget(parent)
{
    //ui.setupUi(this);
    //setFixedSize(750, 500);

    pathLineEdit1 = new QLineEdit;
    pathLineEdit1->setReadOnly(true);
    pathLineEdit1->setText("Zadajte .mp3 subor alebo .ogg subor");
    pathLineEdit1->setFixedWidth(500);
    pathLineEdit1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    pathButton1 = new QPushButton("...");
    connect(pathButton1, &QPushButton::clicked, this, &bakalarska_praca::choosePath);
    pathButton1->setFixedWidth(50);
    pathButton1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    pathLineEdit2 = new QLineEdit;
    pathLineEdit2->setReadOnly(true);
    pathLineEdit2->setText("Zadajte cestu kde sa ma vytvorit priecinok (nepovinne)");
    pathLineEdit2->setFixedWidth(500);
    pathLineEdit2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    pathButton2 = new QPushButton("...");
    connect(pathButton2, &QPushButton::clicked, this, &bakalarska_praca::choosePath);
    pathButton2->setFixedWidth(50);
    pathButton2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    runButton = new QPushButton("Spustit program");
    connect(runButton, &QPushButton::clicked, this, &bakalarska_praca::runProgram);

    QHBoxLayout* layout1 = new QHBoxLayout;
    layout1->addWidget(pathLineEdit1);
    layout1->addWidget(pathButton1);

    QHBoxLayout* layout2 = new QHBoxLayout;
    layout2->addWidget(pathLineEdit2);
    layout2->addWidget(pathButton2);

    progressBar = new QProgressBar;
    progressBar->setVisible(false);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(layout1);
    mainLayout->addLayout(layout2);
    mainLayout->addWidget(runButton);
    mainLayout->addWidget(progressBar);
    setLayout(mainLayout);
}

void bakalarska_praca::choosePath() {
    QString dir;

    if (sender() == pathButton1) {
        dir = QFileDialog::getOpenFileName(this, tr("Vyberte subor"),
            "/home",
            tr("Audio Files (*.mp3 *.ogg)"));
    }
    else if (sender() == pathButton2) {
        dir = QFileDialog::getExistingDirectory(this, tr("Vyberte priecinok"),
            "/home",
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    }

    if (dir.isEmpty())
        return;

    if (sender() == pathButton1) {
        pathLineEdit1->setText(dir);
    }
    else if (sender() == pathButton2) {
        pathLineEdit2->setText(dir);
    }
}


void bakalarska_praca::runProgram() {
    QString path1 = pathLineEdit1->text();
    QString path2 = pathLineEdit2->text();

    std::string path1Str = path1.toStdString();
    std::string path2Str = path2.toStdString();

    if (path1Str == "" || path1Str == "Zadajte .mp3 subor alebo .ogg subor") {
        return;
    }

    pathButton1->setEnabled(false);
    pathButton2->setEnabled(false);
    runButton->setEnabled(false);

    progressBar->setVisible(true);
    progressBar->setValue(0);

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &bakalarska_praca::updateProgressBar);

    future = QtConcurrent::run([=] {
        double songLengthInSeconds = detector.getLengthInSeconds(path1Str);
        QMetaObject::invokeMethod(progressBar, "setRange", Q_ARG(int, 0), Q_ARG(int, songLengthInSeconds));

        QMetaObject::invokeMethod(timer, "start", Q_ARG(int, 1000));
        std::tuple<double, double, double> result = detector.detectBPM(path1Str);
        SMMaker maker;
        Song song = maker.bestFit(result);
        std::cout << song.getArtist() << song.getTitle();
        // skontrolova èi je path2 plne inak pouzi path1
        if (path2Str == "" || path2Str == "Zadajte cestu kde sa ma vytvorit priecinok (nepovinne)") {
            maker.createSMfile(song, path1Str, path1Str);
        }
        else
        {
            maker.createSMfile(song, path1Str, path2Str);
        }
        });
}


void bakalarska_praca::updateProgressBar() {
    int value = progressBar->value();

    if (value >= progressBar->maximum()) {
        QTimer* timer = qobject_cast<QTimer*>(sender());
        timer->stop();
        pathButton1->setEnabled(true);
        pathButton2->setEnabled(true);
        runButton->setEnabled(true);
        progressBar->setValue(0);
        progressBar->setVisible(false);
    }
    else {
        progressBar->setValue(value + 1);
    }
}

void bakalarska_praca::closeFunction()
{
    detector.setShouldStopToTrue();
    if (future.isRunning())
    {
        future.cancel();
    }
    this->close();
}


bakalarska_praca::~bakalarska_praca()
{
    delete pathLineEdit1;
    delete pathLineEdit2;
    delete pathButton1;
    delete pathButton2;
    delete runButton;
    delete progressBar;
}
