#pragma once

#include <string>
#include <vector>

class WeightFileReader
{
private:
	FILE* fileHandle;
	std::vector<float> weights;

public:
	WeightFileReader() = delete;
	WeightFileReader(std::string filepath);
	WeightFileReader(const char* filepath, char** p = nullptr);
	~WeightFileReader();

	std::vector<float> getWeights();
};

