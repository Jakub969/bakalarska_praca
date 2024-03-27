#pragma once
#include <iostream>
#include "BPMDetect.h"
#include "bass.h"
#include <tuple>

class BPMDetector
{
public:
	BPMDetector();
	std::tuple<double, double, double> detectBPM(const std::string& songPath);
};

