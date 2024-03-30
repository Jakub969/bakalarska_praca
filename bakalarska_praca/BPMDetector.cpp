#include "BPMDetector.h"

BPMDetector::BPMDetector() {
}

double BPMDetector::detectBPM(const std::string& songPath) {
    shouldStop = false;
    double bpm = 0.0;

    // Inicializ�cia BASS kni�nice
    if (!BASS_Init(-1, 44100, 0, 0, NULL)) {
        std::cerr << "Nepodarilo sa inicializova� BASS kni�nicu.\n";
        return 0.0;
    }
    else {
        std::cout << "Podarilo sa nainicializovat kniznicu.\n";
    }

    // Na��tanie skladby
    HSAMPLE sample = BASS_SampleLoad(false, songPath.c_str(), 0, 0, 3, BASS_SAMPLE_MONO);
    if (!sample) {
        std::cerr << "Nepodarilo sa na��ta� skladbu.\n";
        return 0.0;
    }
    else {
        std::cout << "Podarilo sa nacitat skladbu.\n";
    }

    // Vytvorenie kan�la
    HCHANNEL channel = BASS_SampleGetChannel(sample, false);

    // Spustenie prehr�vania
    BASS_ChannelPlay(channel, false);

    // Vytvorenie objektu BPMDetect
    soundtouch::BPMDetect bpmDetect(1, 44100);

    std::cout << "Prebieha detekcia BPM" << std::endl;
    // Dek�dovanie skladby a detekcia BPM
    while (BASS_ChannelIsActive(channel) == BASS_ACTIVE_PLAYING) {
        if (shouldStop)
        {
            BASS_ChannelStop(channel);
            BASS_SampleFree(sample);
            BASS_Free();
            return 0.0;
        }

        short bufferShort[8192];
        DWORD bytesRead = BASS_ChannelGetData(channel, bufferShort, sizeof(bufferShort));
        if (bytesRead == -1) {
            std::cerr << "Chyba pri ��tan� d�t: " << BASS_ErrorGetCode() << "\n";
            break;
        }

        if (bytesRead == 0) {
            std::cerr << "Nepre��tali sa �iadne d�ta.\n";
            break;
        }

        // Konverzia short na float
        float bufferFloat[8192];
        
        for (int i = 0; i < bytesRead / 2; i++) {
            bufferFloat[i] = bufferShort[i] / 32767.0f;  // Normaliz�cia hodn�t na rozsah -1.0 a� 1.0
            //std::cout << "bufferShort[" << i << "]" << bufferShort[i] << std::endl;
            if (shouldStop)
            {
                BASS_ChannelStop(channel);
                BASS_SampleFree(sample);
                BASS_Free();
                return 0.0;
            }
        }
        // Pridanie d�t do BPMDetect
        bpmDetect.inputSamples(bufferFloat, bytesRead / 2);

        if (bytesRead < sizeof(bufferShort)) break;  // Koniec skladby
    }
    bpm = bpmDetect.getBpm();

    std::cout << "BPM: " << bpm << std::endl;

    BASS_SampleFree(sample);
    BASS_Free();
    return bpm;
}

double BPMDetector::getLengthInSeconds(const std::string& songPath)
{
    double lengthInSeconds = 0.0;
    // Inicializ�cia BASS kni�nice
    if (!BASS_Init(-1, 44100, 0, 0, NULL)) {
        std::cerr << "Nepodarilo sa inicializova� BASS kni�nicu.\n";
        return lengthInSeconds;
    }
    else {
        std::cout << "Podarilo sa nainicializovat kniznicu.\n";
    }

    // Na��tanie skladby
    HSAMPLE sample = BASS_SampleLoad(false, songPath.c_str(), 0, 0, 3, BASS_SAMPLE_MONO);
    if (!sample) {
        std::cerr << "Nepodarilo sa na��ta� skladbu.\n";
        return lengthInSeconds;
    }
    else {
        std::cout << "Podarilo sa nacitat skladbu.\n";
    }

    // Vytvorenie kan�la
    HCHANNEL channel = BASS_SampleGetChannel(sample, false);
    lengthInSeconds = BASS_ChannelBytes2Seconds(channel, BASS_ChannelGetLength(channel, BASS_POS_BYTE));
    BASS_SampleFree(sample);
    BASS_Free();
    return lengthInSeconds;
}

void BPMDetector::setShouldStopToTrue()
{
    shouldStop = true;
}

