#include "bakalarska_praca.h"
#include <stdlib.h>  
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC  

int main(int argc, char *argv[])
{
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
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
