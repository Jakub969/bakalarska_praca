#include "bakalarska_praca.h"

int main(int argc, char *argv[])
{
    // kód ku grafickému rozhraniu
    QApplication a(argc, argv);
    bakalarska_praca w;
    w.show();
    return a.exec();
    /*
    *   Prejde priecinok s pesnièkami a pošle všetky .sm subory do DB
    */
    //FileManager fileManager;
    //fileManager.processFilesInDirectory("D:/songs-for-stepmania");
}
