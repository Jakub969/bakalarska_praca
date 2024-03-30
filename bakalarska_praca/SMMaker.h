#pragma once
#include "Song.h"
class SMMaker
{
public:
	SMMaker() = default;
	Song bestFit(double result);
	void createSMfile(Song song, std::string path1, std::string path2);
};

