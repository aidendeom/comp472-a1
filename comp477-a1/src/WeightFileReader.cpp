#include "WeightFileReader.h"

#include <string>
#include <vector>

#include "aiden_utils.h"

using std::string;
using std::vector;

WeightFileReader::WeightFileReader(string filepath)
	: WeightFileReader(filepath.c_str())
{ /* Do nothing...*/ }

WeightFileReader::WeightFileReader(const char* filepath, char** p)
{
	auto errorCode = fopen_s(&fileHandle, filepath, "r");

	if (errorCode != 0 || fileHandle == nullptr)
	{
		string errorMsg = aiden_utils::getErrorMessage(errorCode);
		string errorStr{ "Failure to open weight file " };

		errorStr.append(filepath)
			.append(": ")
			.append(errorMsg);

		throw std::runtime_error(errorStr);
	}
}

WeightFileReader::~WeightFileReader()
{
	auto errorCode = fclose(fileHandle);

	if (errorCode != 0)
	{
		string errorMsg = aiden_utils::getErrorMessage(errorCode);
		string errorStr{ "Failure to close weight file " };

		errorStr.append(": ")
			.append(errorMsg);

		throw std::runtime_error(errorStr);
	}
}

vector<float> WeightFileReader::getWeights()
{
	float f{ 0.0f };
	while (fscanf_s(fileHandle, "%f", &f) != EOF)
		weights.push_back(f);

	return weights;
}