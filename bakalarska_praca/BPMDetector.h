#pragma once
#include <iostream>
#include "BPMDetect.h"
#include "bass.h"

class BPMDetector
{
public:
	BPMDetector();
	double detectBPM(const std::string& songPath);
	double getLengthInSeconds(const std::string& songPath);
	void setShouldStopToTrue();
private:
	std::atomic<bool> shouldStop = false;
};

