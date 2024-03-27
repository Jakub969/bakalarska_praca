#pragma once
#include "Song.h"
class SMMaker
{
public:
	SMMaker() = default;
	Song bestFit(std::tuple<double, double, double> result);
	void createSMfile(Song song, std::string path);
};

