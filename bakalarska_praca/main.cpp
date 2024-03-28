#include "FileManager.h"
#include "BPMDetector.h"
#include "SMMaker.h"
#include "bakalarska_praca.h"

int main(int argc, char *argv[])
{
    // kód ku grafickému rozhraniu
    QApplication a(argc, argv);
    bakalarska_praca w;
    w.show();
    return a.exec();
    //FileManager fileManager;
    /*
    *   Prejde priecinok s pesnièkami a pošle všetky .sm subory do DB
    */
    //fileManager.processFilesInDirectory("D:/songs-for-stepmania");
    /*
    BPMDetector detector;
    std::string path = "D:/Jakub/Bakalarka/HVIEZDA  _Kam mam prist__.mp3";
    //std::tuple<double, double, double> result;
    std::tuple<double, double, double> result = detector.detectBPM(path);
    SMMaker maker;
    Song song = maker.bestFit(result);
    maker.createSMfile(song, path);
    return 0;*/
}
