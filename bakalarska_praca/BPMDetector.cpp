#include "BPMDetector.h"

BPMDetector::BPMDetector() {
}

std::tuple<double, double, double> BPMDetector::detectBPM(const std::string& songPath) {
    double bpm = 0.0;
    double offset = 0.0;
    double lengthInSeconds = 0.0;

    // Inicializ�cia BASS kni�nice
    if (!BASS_Init(-1, 44100, 0, 0, NULL)) {
        std::cerr << "Nepodarilo sa inicializova� BASS kni�nicu.\n";
        return std::make_tuple(0.0, 0.0, 0.0);
    }
    else {
        std::cout << "Podarilo sa nainicializovat kniznicu.\n";
    }

    // Na��tanie skladby
    HSAMPLE sample = BASS_SampleLoad(false, songPath.c_str(), 0, 0, 3, BASS_SAMPLE_MONO);
    if (!sample) {
        std::cerr << "Nepodarilo sa na��ta� skladbu.\n";
        return std::make_tuple(0.0, 0.0, 0.0);
    }
    else {
        std::cout << "Podarilo sa nacitat skladbu.\n";
    }

    // Vytvorenie kan�la
    HCHANNEL channel = BASS_SampleGetChannel(sample, false);

    // Spustenie prehr�vania
    BASS_ChannelPlay(channel, false);

    // Z�skanie d�ky skladby v sekund�ch
    lengthInSeconds = BASS_ChannelBytes2Seconds(channel, BASS_ChannelGetLength(channel, BASS_POS_BYTE));

    // Vytvorenie objektu BPMDetect
    soundtouch::BPMDetect bpmDetect(1, 44100);


    // Dek�dovanie skladby a detekcia BPM
    while (BASS_ChannelIsActive(channel) == BASS_ACTIVE_PLAYING) {
        short bufferShort[8192];
        DWORD bytesRead = BASS_ChannelGetData(channel, bufferShort, sizeof(bufferShort));
        if (bytesRead == -1) {
            std::cerr << "Chyba pri ��tan� d�t: " << BASS_ErrorGetCode() << "\n";
            break;
        }
        else {
            std::cout << "bytesRead: " << bytesRead << std::endl;
        }

        if (bytesRead == 0) {
            std::cerr << "Nepre��tali sa �iadne d�ta.\n";
            break;
        }

        // Konverzia short na float
        float bufferFloat[8192];
        for (int i = 0; i < bytesRead / 2; i++) {
            bufferFloat[i] = bufferShort[i] / 32767.0f;  // Normaliz�cia hodn�t na rozsah -1.0 a� 1.0
            std::cout << "bufferShort[" << i << "]" << bufferShort[i] << std::endl;
        }

        offset = BASS_ChannelBytes2Seconds(channel, BASS_ChannelGetPosition(channel, BASS_POS_BYTE));

        // Pridanie d�t do BPMDetect
        bpmDetect.inputSamples(bufferFloat, bytesRead / 2);

        if (bytesRead < sizeof(bufferShort)) break;  // Koniec skladby
    }
    bpm = bpmDetect.getBpm();

    std::cout << "BPM: " << bpm << " offset: " << offset << " sampleLength: " << lengthInSeconds << std::endl;

    BASS_SampleFree(sample);
    BASS_Free();
    return std::make_tuple(bpm, offset, lengthInSeconds);
}

