#include "bakalarska_praca.h"

int main(int argc, char *argv[])
{
    // k�d ku grafick�mu rozhraniu
    QApplication a(argc, argv);
    bakalarska_praca w;
    w.show();
    return a.exec();
    /*
    *   Prejde priecinok s pesni�kami a po�le v�etky .sm subory do DB
    */
    //FileManager fileManager;
    //fileManager.processFilesInDirectory("D:/songs-for-stepmania");
}
